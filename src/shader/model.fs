#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_ambient1; 
    sampler2D texture_specular1; 
};

struct MaterialColor {
    float shininess;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;  
in vec2 TexCoords;
in vec3 TexColor;

uniform vec3 viewPos;
uniform Material material;
uniform MaterialColor materialColor;
uniform Light light;
uniform bool hasTexture;

void main()
{    
    if (hasTexture)
    {
        vec3 ambient = light.ambient * texture(material.texture_ambient1, TexCoords).rgb * materialColor.ambient;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        // vec3 lightDir = normalize(light.position - FragPos);
        vec3 lightDir = normalize(light.position);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb * materialColor.diffuse;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  

        float shininess = materialColor.shininess;
        if(materialColor.shininess == 0)
            shininess = 1;

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb * materialColor.specular;  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
        // FragColor = texture(material.texture_diffuse1, TexCoords);
    }
    else
    {
        // ambient
        vec3 ambient = light.ambient * materialColor.ambient;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        // vec3 lightDir = normalize(light.position);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * materialColor.diffuse;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialColor.shininess);
        vec3 specular = light.specular * (spec * materialColor.specular);  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);

    }
    
}