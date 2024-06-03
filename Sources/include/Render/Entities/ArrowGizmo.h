#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Vec3>
#include <osg/Vec4>
class ArrowGizmo : public osg::Geometry
{
public:
    ArrowGizmo(const osg::Vec3& start, const osg::Vec3& end, const osg::Vec4& color)
    {
        createArrow(start, end, color);
    }

private:
    void createArrow(const osg::Vec3& start, const osg::Vec3& end, const osg::Vec4& color)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;

        // Define the arrow body
        vertices->push_back(start);
        vertices->push_back(end);

        // Set color for the arrow body
        colors->push_back(color);
        colors->push_back(color);

        // Define the arrow head (a cone)
        osg::Vec3 direction = end - start;
        float length = direction.length();
        direction.normalize();

        osg::Vec3 coneBaseCenter = end - direction * 0.2f * length;
        osg::Vec3 up = osg::Vec3(0.0f, 0.0f, 1.0f);
        if (fabs(direction * up) > 0.99f) // Handle the case where direction is parallel to up
        {
            up = osg::Vec3(0.0f, 1.0f, 0.0f);
        }
        osg::Vec3 side = direction ^ up;
        side.normalize();
        up = side ^ direction;

        const int segments = 20;
        float radius = 0.05f * length;

        for (int i = 0; i < segments; ++i)
        {
            float angle = 2.0f * osg::PI * float(i) / float(segments);
            float nextAngle = 2.0f * osg::PI * float(i + 1) / float(segments);
            osg::Vec3 offset(cos(angle) * radius, sin(angle) * radius, 0.0f);
            osg::Vec3 nextOffset(cos(nextAngle) * radius, sin(nextAngle) * radius, 0.0f);

            osg::Vec3 vertex1 = coneBaseCenter + side * offset.x() + up * offset.y();
            osg::Vec3 vertex2 = coneBaseCenter + side * nextOffset.x() + up * nextOffset.y();

            vertices->push_back(end);
            vertices->push_back(vertex1);
            vertices->push_back(vertex2);

            colors->push_back(color);
            colors->push_back(color);
            colors->push_back(color);
        }

        setVertexArray(vertices);
        setColorArray(colors, osg::Array::BIND_PER_VERTEX);

        addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 2)); // Arrow body
        addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 2, segments * 3)); // Arrow head
    }
};
