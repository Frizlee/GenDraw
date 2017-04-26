#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 instancePos;

out vec2 uvInterpolated;

uniform mat4 orthographicMatrix;

void main()
{
	gl_Position = orthographicMatrix * vec4(pos + instancePos, 1.0f);
	uvInterpolated = uv;
}
