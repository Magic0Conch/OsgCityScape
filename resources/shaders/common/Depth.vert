#version 450 compatibility

// uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 osg_NormalMatrix;
in vec3 osg_Vertex;
out vec4 out_Position;
void main()
{    
    out_Position = osg_ModelMatrix * vec4(osg_Vertex,1.0);
    gl_Position = modelViewProjectionMatrix * vec4(osg_Vertex,1.0);
}