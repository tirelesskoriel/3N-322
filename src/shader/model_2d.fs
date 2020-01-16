#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct MaterialColor {
    float shininess;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

in vec3 FragPos;
in vec3 Normal;  

uniform vec3 viewPos;
uniform MaterialColor materialColor;
uniform Light light;

out vec4 FragColor;
void main()
{    
    // ambient
    vec3 ambient = light.ambient * materialColor.ambient;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(light.position);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * materialColor.diffuse;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialColor.shininess);
    vec3 specular = light.specular * (spec * materialColor.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}