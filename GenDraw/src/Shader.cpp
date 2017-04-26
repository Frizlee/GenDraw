#include "Shader.hpp"
using namespace std;

Shader::Shader()
{
}

Shader::Shader(Shader &&rhs)
	: GpuResource(move(rhs))
{
}

Shader& Shader::operator=(Shader &&rhs)
{
	GpuResource::operator=(move(rhs));
	return *this;
}

Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
	create(vertexShader, fragmentShader);
}

void Shader::create(std::string vertexShader, std::string fragmentShader)
{
	GLuint vertexShaderID = gl::CreateShader(gl::VERTEX_SHADER);
	GLuint fragmentShaderID = gl::CreateShader(gl::FRAGMENT_SHADER);
	GLint result;
	GLint logLength;
	string log;

	mID = gl::CreateProgram();
	const char *str = vertexShader.data();
	GLint len = vertexShader.length();
	gl::ShaderSource(vertexShaderID, 1, &str, &len);
	gl::CompileShader(vertexShaderID);
	gl::GetShaderiv(vertexShaderID, gl::COMPILE_STATUS, &result);

	if (result == gl::FALSE_)
	{
		gl::GetShaderiv(vertexShaderID, gl::INFO_LOG_LENGTH, &logLength);
		log.resize(logLength);
		gl::GetShaderInfoLog(vertexShaderID, logLength, nullptr, &log[0]);

		cout << log << std::endl;
		log.clear();
	}

	str = fragmentShader.data();
	len = fragmentShader.length();
	gl::ShaderSource(fragmentShaderID, 1, &str, &len);
	gl::CompileShader(fragmentShaderID);
	gl::GetShaderiv(fragmentShaderID, gl::COMPILE_STATUS, &result);

	if (result == gl::FALSE_)
	{
		gl::GetShaderiv(fragmentShaderID, gl::INFO_LOG_LENGTH, &logLength);
		log.resize(logLength);
		gl::GetShaderInfoLog(fragmentShaderID, logLength, nullptr, &log[0]);

		cout << log << std::endl;
		log.clear();
	}

	gl::AttachShader(mID, vertexShaderID);
	gl::AttachShader(mID, fragmentShaderID);
	gl::LinkProgram(mID);
	gl::GetProgramiv(mID, gl::LINK_STATUS, &result);

	if (result == gl::FALSE_)
	{
		gl::GetShaderiv(mID, gl::INFO_LOG_LENGTH, &logLength);
		log.resize(logLength);
		gl::GetShaderInfoLog(mID, logLength, nullptr, &log[0]);

		cout << log << std::endl;
		log.clear();
	}

	gl::DeleteShader(vertexShaderID);
	gl::DeleteShader(fragmentShaderID);
}

void Shader::clear()
{
	gl::DeleteProgram(mID);
}

Shader::~Shader()
{
	clear();
}

GLint Shader::getLocation(std::string name)
{
	return gl::GetUniformLocation(mID, name.c_str());
}

std::uint32_t Shader::getSize() const
{
	return 0;
}
