#version 330 core


struct Material {
    // vec3 ambient; //what color the surface reflects (basically the color of the cube)
    sampler2D diffuse; //color of the surface under lighting
    sampler2D specular; //color of the highlight
    float shininess; //radius of the highlight
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    //attenuation
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct SpotLight {
    vec3 position; //not neccesary for directional lights that are infintely far away (like the sun)
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight flashLight;
uniform bool flashLightOn;
uniform Material material;

out vec4 FragColor;

// we specifically pass in light structs since there may be more than one
// we dont pass in material or texture since there is one per fragment shader
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{ 	
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //Directional Light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    //Point Light
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    //Camera Spot Light
    if(flashLightOn){
        result += CalcSpotLight(flashLight, norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);//negate to get frag to light

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0); //cos of the angle between surface normal and lightDir (if negative that mean light behind it)
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); //diffuse map

    //specular
    vec3 reflectDir = reflect(-lightDir, normal); //flip lightDir to simulate real life light reflection
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //cos of angle between reflection and view determine how bright specularity is
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);//Frag to Light
    
    // ambient
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
 
    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);//basically cos theta (direct hit = 1, perpindicular = 0)
    vec3 diffuse  = light.diffuse * vec3(texture(material.diffuse, TexCoords)); 
        
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  //-lightdir = light to Frag
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // calculate do product between view and reflected light. Closer the stronger (last param = shininess)
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);//Frag to Light
    
    // ambient
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
 
    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);//basically cos theta (direct hit = 1, perpindicular = 0)
    vec3 diffuse  = light.diffuse * vec3(texture(material.diffuse, TexCoords)); 
        
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  //-lightdir = light to Frag
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // calculate do product between view and reflected light. Closer the stronger (last param = shininess)
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    //clamped spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
