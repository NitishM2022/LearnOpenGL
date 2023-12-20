#version 330 core

struct Material {
    // vec3 ambient;//what color the surface reflects (basically the color of the cube)
    // vec3 diffuse;//color of the surface under diffuse lighting
    // vec3 specular;//color of the highlight
    sampler2D diffuse;
    sampler2D specular;
    float shininess;//radius of the highlight
}; 
uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;  
  
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  
out vec4 FragColor;

void main()
{
    // ambient
    vec3 ambient  = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);//Frag to Light
    float diff = max(dot(norm, lightDir), 0.0);//basically cos theta (direct hit = 1, perpindicular = 0)
    vec3 diffuse  = light.diffuse * texture(material.diffuse, TexCoords).rgb; 
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  //-lightdir = light to Frag
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // calculate do product between view and reflected light. Closer the stronger (last param = shininess)
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}