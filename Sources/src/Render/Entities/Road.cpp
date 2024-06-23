#include "Render/Entities/Road.h"
#include "osg/Vec3f"

using namespace CSEditor::Render;


inline bool IsInferiorAngle(osg::Vec3f point0,osg::Vec3f point1,osg::Vec3f point2,osg::Vec3f planeNormal)
{
    auto lastToCurrentDirection = point1 - point0;
    lastToCurrentDirection.normalize();
    auto currentToNextDirection =point2 - point1;
    currentToNextDirection.normalize();
    bool isInferiorAngle = ((-lastToCurrentDirection^currentToNextDirection)*planeNormal) > 0;
    return isInferiorAngle;
}
inline bool isLeftSide(osg::Vec3f origin, osg::Vec3f target, osg::Vec3f checkPoint,osg::Vec3f planeNormal)
{
    osg::Vec3f direction = target - origin;
    osg::Vec3f checkDirection = checkPoint - origin;
    return (checkDirection^direction)*planeNormal>0;
}
Road::Road(std::unique_ptr<std::vector<osg::Vec3f>> pathKeyPoints,osg::Vec3f planeNormal,float pathWidth,float innerRadius,int segment):
m_pathKeyPoints(std::move(pathKeyPoints)),m_planeNormal(planeNormal),BaseGeometry(segment){
    setPathWidth(pathWidth);
    setInnerRadius(innerRadius);
}

void Road::setPathWidth(float rhs){
    setFloatProperty("pathWidth", rhs);
}

void Road::setInnerRadius(float rhs){
    setFloatProperty("innerRadius", rhs);
}

void Road::setPlaneNormal(const osg::Vec3f& planeNormal){
    m_planeNormal = planeNormal;
}

osg::Vec3f Road::getPlaneNormal() const{
    return m_planeNormal;
}

void Road::update(){
    if(m_isDirty){
        if(m_pathKeyPoints->size() < 2) return;
        float pathWidth = std::any_cast<float>(m_properties["pathWidth"]);
        float innerRadius = std::any_cast<float>(m_properties["innerRadius"]);
        int segments = std::any_cast<int>(m_properties["segments"]);
        int vertexCountPerCorner = 2 + segments*2;
        int vertexSize = 4 + (m_pathKeyPoints->size()-2)*vertexCountPerCorner;
        //路径上的2个 + 拐弯处的segment
        int trianglesCountPerCycle = (2 + segments*2);
        //总三角形数需要加上开头的2个
        int trianglesCountTotal = 2 + trianglesCountPerCycle*(m_pathKeyPoints->size()-2);
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
        vertices->resizeArray(vertexSize);
        osg::ref_ptr<osg::Vec2Array> uvs = new osg::Vec2Array();
        uvs->resizeArray(vertexSize);
        osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
        indices->resize(trianglesCountTotal*3);

        float accumulatePathLength = 0;
        auto normal = m_planeNormal^((*m_pathKeyPoints)[1] - (*m_pathKeyPoints)[0]);
        normal.normalize();
        bool isInferiorAngle = isLeftSide((*m_pathKeyPoints)[0], (*m_pathKeyPoints)[1], (*m_pathKeyPoints)[0] + normal * pathWidth,m_planeNormal);
        normal = isInferiorAngle? normal : -normal;
        (*vertices)[0] = (*m_pathKeyPoints)[0] + normal * pathWidth;
        (*vertices)[1] = (*m_pathKeyPoints)[0] - normal * pathWidth;
        (*uvs)[0] = osg::Vec2f(0,0);
        (*uvs)[1] = osg::Vec2f(1,0);

        for (int i = 1; i < m_pathKeyPoints->size() - 1; i++)
        {
            osg::Vec3f lastToCurrentDirection = (*m_pathKeyPoints)[i] - (*m_pathKeyPoints)[i - 1];
            lastToCurrentDirection.normalize();
            osg::Vec3f currentToNextDirection = (*m_pathKeyPoints)[i + 1] - (*m_pathKeyPoints)[i];
            currentToNextDirection.normalize();

            isInferiorAngle = IsInferiorAngle((*m_pathKeyPoints)[i-1], (*m_pathKeyPoints)[i], (*m_pathKeyPoints)[i+1],m_planeNormal);
            osg::Vec3f normalBeforeInner = isInferiorAngle? -m_planeNormal^lastToCurrentDirection: m_planeNormal^lastToCurrentDirection;                
            normalBeforeInner.normalize();
            osg::Vec3f normalAfterInner = isInferiorAngle? -m_planeNormal^currentToNextDirection: m_planeNormal^currentToNextDirection;                
            normalAfterInner.normalize();

            osg::Vec3f innerBeforePoint = (*m_pathKeyPoints)[i] + normalBeforeInner * pathWidth;
            osg::Vec3f innerAfterPoint = (*m_pathKeyPoints)[i] + normalAfterInner * pathWidth;
            
            //ray1:innerBeforePoint - lastToCurrentDirection*t1;ray2: innerAfterPoint + currentToNextDirection*t2; 
            osg::Vec3f crossProductd1d2 = -lastToCurrentDirection^currentToNextDirection;
            float t1 = (((innerAfterPoint - innerBeforePoint)^ currentToNextDirection)* crossProductd1d2) / (crossProductd1d2.length() * crossProductd1d2.length());

            osg::Vec3f innerIntersection = innerBeforePoint - lastToCurrentDirection * t1;
            osg::Vec3f h = ((-lastToCurrentDirection + currentToNextDirection)/2);
            h.normalize();
            float theta = acos(-lastToCurrentDirection*currentToNextDirection);
            osg::Vec3f cornerCenter = innerIntersection + h * innerRadius / sin(theta/2);
            float cornerAngle = osg::PI - theta;


            osg::Quat q ((isInferiorAngle?1:-1) * cornerAngle / 2,m_planeNormal); 
            //Matrix4x4 rotateMatrix = Matrix4x4.Rotate(q);
            osg::Vec3f leftBorderRadialDirection = q*-h;
            
            
            for(int j = 0; j <= segments; j++)
            {
                q = osg::Quat((isInferiorAngle ? -1 : 1) * cornerAngle / segments * j, m_planeNormal);                    
                osg::Vec3f currentRadialDirection = q * leftBorderRadialDirection;
                
                osg::Vec3f innerPoint = cornerCenter + currentRadialDirection * innerRadius;
                osg::Vec3f outerPoint = cornerCenter + currentRadialDirection * (innerRadius + pathWidth * 2);

                int vertexIndex = (i - 1) * vertexCountPerCorner + 2 + 2 * j;

                (*vertices)[vertexIndex] = isInferiorAngle?innerPoint:outerPoint;
                (*vertices)[vertexIndex+1] = isInferiorAngle?outerPoint:innerPoint;
                
                accumulatePathLength += ((*vertices)[vertexIndex+1] - (*vertices)[vertexIndex-1]).length()*0.5f+ ((*vertices)[vertexIndex] - (*vertices)[vertexIndex - 2]).length()*0.5f;
                
                (*uvs)[vertexIndex] = osg::Vec2f(0, accumulatePathLength);
                (*uvs)[vertexIndex+1] = osg::Vec2f(1,accumulatePathLength);
            }
            ////剪掉多加的一段
            //accumulatePathLength -= (innerRadius + pathWidth) * cornerAngle / segment;
        }
        normal = (m_planeNormal^ ((*m_pathKeyPoints)[m_pathKeyPoints->size() - 1] - (*m_pathKeyPoints)[m_pathKeyPoints->size() - 2]));
        isInferiorAngle = isLeftSide((*m_pathKeyPoints)[m_pathKeyPoints->size() - 2], (*m_pathKeyPoints)[m_pathKeyPoints->size() - 1], (*m_pathKeyPoints)[m_pathKeyPoints->size() - 1] + normal * pathWidth,m_planeNormal);
        normal = isInferiorAngle? normal : -normal;
        normal.normalize();
        (*vertices)[(m_pathKeyPoints->size() - 2) * vertexCountPerCorner + 2] = (*m_pathKeyPoints)[m_pathKeyPoints->size() - 1] + normal * pathWidth;
        (*vertices)[(m_pathKeyPoints->size() - 2) * vertexCountPerCorner + 3] = (*m_pathKeyPoints)[m_pathKeyPoints->size() - 1] - normal * pathWidth;
        
        accumulatePathLength += ((*vertices)[vertexSize - 1] - (*vertices)[vertexSize - 3]).length() * 0.5f + ((*vertices)[vertexSize - 2] - (*vertices)[vertexSize - 4]).length()*0.5f;
        (*uvs)[(m_pathKeyPoints->size() - 2) * vertexCountPerCorner + 2] = osg::Vec2f(0, accumulatePathLength);
        (*uvs)[(m_pathKeyPoints->size() - 2) * vertexCountPerCorner + 3] = osg::Vec2f(1, accumulatePathLength);

        int curIndex = 0;
        for (int i = 0; i < trianglesCountTotal/2; i++)
        {
            (*indices)[i * 6] = curIndex;
            (*indices)[i * 6 + 1] = curIndex + 1;
            (*indices)[i * 6 + 2] = curIndex + 3;
            (*indices)[i * 6 + 3] = curIndex;
            (*indices)[i * 6 + 4] = curIndex + 2;
            (*indices)[i * 6 + 5] = curIndex + 3;
            curIndex += 2;
        }
        setVertexArray(vertices.get());
        setTexCoordArray(0,uvs.get());
        setVertexAttribArray(0,getVertexArray());
        setVertexAttribArray(1,getTexCoordArray(0));
        setPrimitiveSet(0,indices);
        m_isDirty = false;
    }
}
