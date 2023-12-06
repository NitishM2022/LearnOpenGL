#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform float timeColor; // we set this variable in the OpenGL code.

void main()
{
    // float timeColor = (sin(timeValue) / 2.0f) + 0.5f;
    FragColor = vec4(ourColor.r*timeColor, ourColor.g*timeColor, ourColor.b*timeColor, 1.0f);      
    //FragColor = vec4(ourColor, 1.0f)
}