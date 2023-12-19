//vertex shader is a vector of 4 length x,y,z and w
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model; //global model transformation
uniform mat4 view; //camera
uniform mat4 projection; //perspective

out vec3 FragPos;  
out vec3 Normal;
  
void main()
{
   FragPos = vec3(model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(model))) * aNormal;  //use a normal matrix to prevent non uniform scaling from messing up the perpindiculaity of normal vectors


   gl_Position = projection * view * model * vec4(aPos, 1.0);
}