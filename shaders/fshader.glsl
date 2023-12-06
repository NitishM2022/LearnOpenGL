#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform float timeColor; // we set this variable in the OpenGL code.

void main()
{
    const float eps = 0.00001;
    if (abs(ourColor.r - 1.0) < eps) {
        // If the red channel is 1.0, modify the color based on time
        FragColor = vec4(timeColor, 0.0, 0.0, 1.0);
    } else if (abs(ourColor.r - 1.0) < eps) {
        // If the green channel is 1.0, modify the color differently (you can adjust this logic)
        FragColor = vec4(0.0, timeColor, 0.0, 1.0);
    } else if (abs(ourColor.r - 1.0) < eps) {
        // If the blue channel is 1.0, modify the color differently (you can adjust this logic)
        FragColor = vec4(0.0, 0.0, timeColor, 1.0);
    } else {
        FragColor = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0);
    }
}
