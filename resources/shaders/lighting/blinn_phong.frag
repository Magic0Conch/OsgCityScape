#version 330 core

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;


struct Light {
    vec3 lightDir;
    vec3 ambient;
};

struct Material {
    sampler2D diffuse;
	float shininess;  
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos; 
void main()
{
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,-light.lightDir), 0.0);
    
    vec3 diffuseColor = texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = diff * diffuseColor;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(viewDir - light.lightDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    
    vec3 specular = light.specular * (spec * diffuseColor);
    
    vec3 result = (light.ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
