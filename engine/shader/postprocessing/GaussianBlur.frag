#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D image;
uniform bool horizontal;
float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main(){
    vec2 deltaTexCoord = 1.0 / textureSize(image, 0);
    vec3 result = texture(image,TexCoords).rgb * weight[0];
    if(horizontal){
        for(int i = 1;i<5;++i){
            result += texture(image,vec2(TexCoords.x + deltaTexCoord.x*i,TexCoords.y)).rgb*weight[i];
            result += texture(image,vec2(TexCoords.x - deltaTexCoord.x*i,TexCoords.y)).rgb*weight[i];
        }
    }
    else{
        for(int i = 1;i<5;++i){
            result += texture(image,vec2(TexCoords.x,TexCoords.y + deltaTexCoord.y*i)).rgb*weight[i];
            result += texture(image,vec2(TexCoords.x ,TexCoords.y- deltaTexCoord.y*i)).rgb*weight[i];
        }
    }
    FragColor = vec4(result,1.0);
}