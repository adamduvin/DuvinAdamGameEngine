#version 430

in vec3 color;

layout (location = 20) uniform vec3 lightLoc;
layout (location = 30) uniform vec3 cameraLoc;

in vec3 fragLoc;
in vec3 fragNorm;

in vec2 fragUV;

uniform sampler2D myTexture;

void main() {
	vec3 L = normalize(lightLoc - fragLoc);
	vec3 V = normalize(cameraLoc - fragLoc);
	vec3 H = normalize(V + L);
	vec3 N = normalize(fragNorm);

	float alpha = 16.0f;
	float ambient = 0.1f;
	float diffuse = max(dot(L, N), 0);
	float specular = pow(max(dot(H, N), 0), alpha);

	float brightness = ambient + diffuse + specular;

	//gl_FragColor = vec4(brightness, brightness, brightness, 1);
	gl_FragColor = brightness * texture(myTexture, fragUV);
}