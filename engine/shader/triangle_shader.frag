#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec4 ourColor;


uniform sampler2D texture0;
//uniform sampler2D texture1;
uniform float horizontalOffset;

void main()
{
	float currentX = TexCoord.x+horizontalOffset;
	vec4 col0 = texture(texture0, vec2(currentX,TexCoord.y));
//	vec4 col1 = texture(texture1, vec2(currentX-1,TexCoord.y));
	
//	FragColor = mix(col0,col1,0.2);
	FragColor = col0;

}