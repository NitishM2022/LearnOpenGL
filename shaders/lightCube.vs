//vertex shader is a vector of 4 length x,y,z and w
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model; //global model transformation
uniform mat4 view; //camera
uniform mat4 projection; //perspective

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
}