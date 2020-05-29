#version 410 core
out vec4 FragColor;

uniform sampler2D ComTexture;

void main()
{
	FragColor = texture(ComTexture, vec2(gl_FragCoord.xy) / vec2(textureSize(ComTexture, 0)));
}