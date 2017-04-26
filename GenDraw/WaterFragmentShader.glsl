#version 330 core
in vec2 uvInterpolated;

layout (location = 0) out vec4 fragmentColor;

uniform sampler2D sampler;
uniform vec2 displacement;

void main()
{

	fragmentColor = texture(sampler, uvInterpolated * displacement);
	fragmentColor.a = 0.5f;
	// fragmentColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}