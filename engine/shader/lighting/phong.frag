#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 lightPosition;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;

uniform float ambientStrength;
uniform float specularStrength;


void main(){
    vec3 ambientColor = ambientStrength*lightColor;
    
    vec3 lightDir = normalize(lightPosition - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * lightColor;
    
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 h = normalize(lightDir + viewDir);
    float spec = pow(max(dot(h,norm),0.0) ,32);
    vec3 specularColor = spec*specularStrength*lightColor;

    vec3 resultColor = (ambientColor+diffuseColor+specularColor)*objectColor;


    FragColor=vec4(resultColor,1.0f);

}