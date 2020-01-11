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
    float shininess;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

in vec3 FragPos;
in vec3 Normal;  
in vec2 TexCoords;
in vec3 TexColor;
in vec3 Tangent;
in vec3 Bitangent;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool hasTexture;

out vec4 FragColor;
void main()
{    
    if (hasTexture)
    {
        vec3 ambient = light.ambient * texture(material.texture_ambient1, TexCoords).rgb * material.ambient;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        // vec3 lightDir = normalize(light.position - FragPos);
        vec3 lightDir = normalize(light.position);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * texture(material.texture_diffuse1, TexCoords).rgb * material.diffuse);  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * texture(material.texture_specular1, TexCoords).rgb * material.specular);  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        // ambient
        vec3 ambient = light.ambient * material.ambient;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        // vec3 lightDir = normalize(light.position);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * material.diffuse;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular);  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);

        // if(light.ambient.x == 0 || light.ambient.y == 0 || light.ambient.z == 0 
        // || light.specular.x == 0 || light.specular.y == 0 || light.specular.z == 0 
        // || light.diffuse.x == 0 || light.diffuse.y == 0 || light.diffuse.z == 0)
        // {
        //     FragColor = vec4(result, 1.0);
        // }else{
        //     FragColor = vec4(0.0,0.0,0.0, 1.0);
        // }
    }
    
}