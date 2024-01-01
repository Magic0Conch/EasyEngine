#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

// uniform sampler2D texture_diffuse1;
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;


void main()
{    
    FragColor = texture(material.texture_diffuse1, TexCoords);
    FragColor = vec4(0.2,0.8,0.6,1.0);
}