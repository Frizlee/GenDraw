#version 330 core
in vec2 uvInterpolated;
layout (location = 0) out vec4 fragmentColor;

uniform vec4 textColor;
uniform sampler2D sampler;

void main()
{
	fragmentColor = textColor * texture(sampler, uvInterpolated).x;
}
