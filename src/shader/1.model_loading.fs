#version 330 core

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    // sampler2D texture_diffuse1;
    // sampler2D texture_specular1; 
    // sampler2D texture_ambient1;
    float shininess;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

out vec4 FragColor;

in vec3 FragPos;
// in vec2 TexCoords;
// in vec3 TexColor;
in vec3 Normal;  

uniform vec3 viewPos;
uniform Material material ;
uniform Light light ;

void main()
{    
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(light.position);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}