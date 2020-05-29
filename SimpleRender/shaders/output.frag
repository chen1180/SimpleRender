#version 410 core
out vec4 FragColor;

uniform sampler2D ComTexture;
uniform float invSamples;
vec4 ToneMap(in vec4 c, float limit)
{
	float luminance = 0.3*c.x + 0.6*c.y + 0.1*c.z;

	return c * 1.0 / (1.0 + luminance / limit);
}
void main()
{
	vec4 color= texture(ComTexture, vec2(gl_FragCoord.xy) / vec2(textureSize(ComTexture, 0)))*invSamples;
	FragColor =pow(ToneMap(color, 1.5), vec4(1.0 / 2.2));
}