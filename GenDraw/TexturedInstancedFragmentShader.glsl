#version 330 core
in vec2 uvInterpolated;

layout (location = 0) out vec4 fragmentColor;

uniform sampler2D sampler;

void main()
{
	fragmentColor = texture(sampler, uvInterpolated);
	// fragmentColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}