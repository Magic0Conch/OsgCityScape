#version 330 core

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelMatrix;
uniform mat3 osg_NormalMatrix;

in vec4 osg_Vertex;
in vec3 osg_Normal;
in vec4 osg_Color;

out vec3 v_normal;
out vec4 v_color;
out vec3 v_position;

void main()
{
    vec4 worldPos = osg_ModelMatrix * osg_Vertex;
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
    v_normal = normalize(osg_NormalMatrix * osg_Normal);
    v_color = osg_Color;
    v_position = worldPos.xyz;
}