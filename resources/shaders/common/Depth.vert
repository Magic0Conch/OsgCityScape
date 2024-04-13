#version 450 compatibility

// uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 osg_NormalMatrix;
in vec3 osg_Vertex;
out vec4 out_Position;
mat4 customMatrix = mat4(
    vec4(1, 0, 0, 0), 
    vec4(0, 0, -1, 0), 
    vec4(0, 1, 0, 0),
    vec4(0, 0, 0, 1)
);
void main()
{    
    out_Position = osg_ModelMatrix * vec4(osg_Vertex,1.0);
    gl_Position = viewProjectionMatrix* customMatrix * vec4(osg_Vertex,1.0);
}