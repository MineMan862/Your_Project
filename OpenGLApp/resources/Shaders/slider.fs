#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 spriteColor;
uniform float fillPercentage; // 0.0 to 1.0

void main()
{
    if (TexCoords.x <= fillPercentage) {
        FragColor = vec4(spriteColor, 1.0);
    } else {
        FragColor = vec4(0.3, 0.3, 0.3, 1.0); // Sfondo grigio scuro per la parte vuota
    }
}
