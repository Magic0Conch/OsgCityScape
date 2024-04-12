#version 450 compatibility

// uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 osg_NormalMatrix;
in vec4 osg_Vertex;
out vec4 out_Position;

void main()
{    
    out_Position = osg_ModelMatrix * osg_Vertex;
    gl_Position = viewProjectionMatrix * osg_Vertex;
}