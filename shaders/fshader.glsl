#version 330 core
#define PI 3.1415926535897932384626433832795
out vec4 FragColor;

in vec3 ourColor;
uniform float timeValue; // we set this variable in the OpenGL code.

void main()
{
    float timeColor = (sin(timeValue) / 2.0f) + 0.5f;
    float timeColor1 = (sin(timeValue + 2 * PI / 3) / 2.0f) + 0.5f;
    float timeColor2 = (sin(timeValue + 4 * PI / 3) / 2.0f) + 0.5f;
    FragColor = vec4(ourColor.r*timeColor, ourColor.g*timeColor1, ourColor.b*timeColor2, 1.0f);      
    //FragColor = vec4(ourColor, 1.0f)
}