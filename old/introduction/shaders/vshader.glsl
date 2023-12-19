//vertex shader is a vector of 4 length x,y,z and w
#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
 layout (location = 1) in vec2 aTexCoord;

//out vec3 ourColor;//send ourColor to next shader in pipeline
out vec2 TexCoord;

uniform mat4 model; //global model transformation
uniform mat4 view; //camera
uniform mat4 projection; //perspective

void main()
{
   // note that we read the multiplication from right to left
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   //ourColor = aColor;
   TexCoord = aTexCoord;
}