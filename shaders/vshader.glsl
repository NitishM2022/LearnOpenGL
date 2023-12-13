//vertex shader is a vector of 4 length x,y,z and w
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;//send ourColor to next shader in pipeline
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
   gl_Position = transform * vec4(aPos, 1.0);
   ourColor = aColor;
   TexCoord = aTexCoord;
}