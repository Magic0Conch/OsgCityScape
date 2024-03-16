#version 330 core

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform float shininess;

in vec3 v_normal;
in vec4 v_color;
in vec3 v_position;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(v_normal);
    vec3 lightDirNorm = normalize(lightDir);
    vec3 viewDir = normalize(viewPos - v_position);
    vec3 halfwayDir = normalize(lightDirNorm + viewDir);

    float diffuse = max(dot(normal, lightDirNorm), 0.0);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    vec3 diffuseColor = lightColor * diffuse * v_color.rgb;
    vec3 specularColor = lightColor * specular;
    vec3 ambientColor = ambientColor * v_color.rgb;

    fragColor = vec4(ambientColor + diffuseColor + specularColor, v_color.a);
}