#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 powerupColor;
uniform float time;

void main() {
    float ambientStrenght = 0.6;
    vec3 ambient = ambientStrenght * powerupColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(-0.5, 1.0, 0.5));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * powerupColor;
    float pulse = (sin(time * 5.0)+1.0) / 2.0;
    vec3 color = powerupColor * pulse * 0.4;

    vec3 result = ambient + diffuse + color;

    FragColor = vec4(result, 1.0);
}