#version 430

layout (location = 0) in vec3 modelPos;

layout (location = 1) in vec2 modelUV;

layout (location = 2) in vec3 fragNormIn;

layout (location = 4) uniform mat4 worldView;

layout (location = 5) uniform mat4 modelWorld;

out vec3 color;

out vec3 fragLoc;
out vec3 fragNorm;

out vec2 fragUV;

void main() {
	fragUV = modelUV;

	gl_Position = worldView * modelWorld * vec4(modelPos, 1);
	color = modelPos;

	fragLoc = modelPos;
	fragNorm = fragNormIn;
}