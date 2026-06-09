#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

// Texture
uniform sampler2D texture_diffuse1;
uniform bool useTexture;
uniform vec3 objectColor;
uniform float alpha;

// Directional Light
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

// Camera
uniform vec3 viewPos;
uniform float shininess;

void main()
{
    vec3 color;
    if (useTexture) {
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        if (texColor.a < 0.1)
            discard;
        color = texColor.rgb;
    } else {
        color = objectColor;
    }

    // Ambient
    vec3 ambient = dirLight.ambient * color;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * color;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = dirLight.specular * spec;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, alpha);
}