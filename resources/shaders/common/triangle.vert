#version 450 compatibility
in vec4 osg_Vertex;
in vec4 osg_MultiTexCoord0;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 _ProjectionMatrix;

out vec2 TexCoords;

void main(){
    gl_Position =  _ProjectionMatrix*osg_ModelViewMatrix*osg_Vertex;
    TexCoords = osg_MultiTexCoord0.xy;
}
