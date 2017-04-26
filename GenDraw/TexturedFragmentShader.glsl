#version 330 core
in vec2 uvInterpolated;

layout (location = 0) out vec4 fragmentColor;

uniform sampler2D sampler;

void main()
{
	vec4 sampledColor = texture(sampler, uvInterpolated);

	fragmentColor.r = sampledColor.r;
	fragmentColor.g = sampledColor.r;
	fragmentColor.b = sampledColor.r;
	fragmentColor.a = 1.0f;
}