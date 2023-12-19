#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

in vec3 Normal;  
in vec3 FragPos;  
out vec4 FragColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//Frag to Light
    float diff = max(dot(norm, lightDir), 0.0);//basically cos theta (direct hit = 1, perpindicular = 0)
    vec3 diffuse = diff * lightColor; 
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  //-lightdir = light to Frag
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // calculate do product between view and reflected light. Closer the stronger (last param = shininess)
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 