//https://github.com/spite/Wagner/blob/master/fragment-shaders/denoise-fs.glsl

uniform sampler2D ComTexture;
uniform float exponent;
uniform float strength;
uniform vec2 resolution;

void main() {

	vec4 center = texture(ComTexture, vec2(gl_FragCoord.xy) / vec2(textureSize(ComTexture, 0)));
	vec4 color = vec4(0.0);
	float total = 0.0;
	for (float x = -4.0; x <= 4.0; x += 1.0) {
		for (float y = -4.0; y <= 4.0; y += 1.0) {
			vec4 sample = texture2D(ComTexture, vec2(gl_FragCoord.xy) / vec2(textureSize(ComTexture, 0)) + vec2(x, y) / resolution);
			float weight = 1.0 - abs(dot(sample.rgb - center.rgb, vec3(0.25)));
			weight = pow(weight, exponent);
			color += sample * weight;
			total += weight;
		}
	}
	gl_FragColor = color / total;

}