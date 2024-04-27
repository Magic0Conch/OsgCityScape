#version 330 core
in vec4 osg_Vertex;
in vec3 osg_Normal;
uniform mat3 osg_NormalMatrix;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ProjectionMatrix;
uniform mat4 osg_ModelViewProjectionMatrix;
uniform float _Outline;

// out normal;
void main()
{
    vec3 pos = osg_Vertex.xyz + normalize(osg_Normal) * _Outline;
    // vec4 pos = osg_ModelViewMatrix * scaleMatrix * osg_Vertex;
    // texCoord = osg_MultiTexCoord0.xy;
    // vec3 normal = normalize(osg_NormalMatrix * osg_Normal);
    // normal.z = -0.5;
    // pos = pos + vec4(normalize(normal), 0.0) * _Outline;
    gl_Position = osg_ModelViewProjectionMatrix * vec4(pos,1.0);
}