#version 450 compatibility

uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 viewMatrix;
uniform mat4 osg_ProjectionMatrix;

in vec3 osg_Vertex;
out vec4 out_Position;
void main()
{    
    out_Position =  vec4(osg_Vertex,1.0);
    gl_Position = viewMatrix * vec4(osg_Vertex,1.0);
}