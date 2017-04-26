#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
using namespace std;

Renderer::Renderer() 
	: mInitialized{ false }, mSetup{ Setup::NONE }
{
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
	if (gl::sys::LoadFunctions())
		mInitialized = true;

	// Create 2d sampler
	gl::GenSamplers(1, &mSamplerMipmapLinear);
	gl::SamplerParameteri(mSamplerMipmapLinear, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSamplerMipmapLinear, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSamplerMipmapLinear, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
	gl::SamplerParameteri(mSamplerMipmapLinear, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	

	gl::GenSamplers(1, &mSamplerLinear);
	gl::SamplerParameteri(mSamplerLinear, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSamplerLinear, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::SamplerParameteri(mSamplerLinear, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::SamplerParameteri(mSamplerLinear, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	

	// Load text shader
	ifstream vertexShaderFile{ "TextVertexShader.glsl" };
	string vertexShaderData{ istreambuf_iterator<char>(vertexShaderFile),
		istreambuf_iterator<char>() };
	ifstream fragmentShaderFile{ "TextFragmentShader.glsl" };
	string fragmentShaderData{ istreambuf_iterator<char>(fragmentShaderFile),
		istreambuf_iterator<char>() };

	mTextShader.create(vertexShaderData, fragmentShaderData);
	mOrthographicMatrixLocationText = mTextShader.getLocation("orthographicMatrix");
	mTextColorLocationText = mTextShader.getLocation("textColor");
	mSamplerLocationText = mTextShader.getLocation("sampler");

	// Load sprite shader
	vertexShaderFile.close();
	vertexShaderFile.open("TexturedVertexShader.glsl");
	vertexShaderData.assign(istreambuf_iterator<char>(vertexShaderFile),
		istreambuf_iterator<char>());
	fragmentShaderFile.close();
	fragmentShaderFile.open("TexturedFragmentShader.glsl");
	fragmentShaderData.assign(istreambuf_iterator<char>(fragmentShaderFile),
		istreambuf_iterator<char>());

	mSpriteShader.create(vertexShaderData, fragmentShaderData);
	mOrthographicMatrixLocationSprite = mSpriteShader.getLocation("orthographicMatrix");
	mSamplerLocationSprite = mSpriteShader.getLocation("sampler");

	// Load instanced sprite shader
	vertexShaderFile.close();
	vertexShaderFile.open("TexturedInstancedVertexShader.glsl");
	vertexShaderData.assign(istreambuf_iterator<char>(vertexShaderFile),
		istreambuf_iterator<char>());
	fragmentShaderFile.close();
	fragmentShaderFile.open("TexturedInstancedFragmentShader.glsl");
	fragmentShaderData.assign(istreambuf_iterator<char>(fragmentShaderFile),
		istreambuf_iterator<char>());

	mSpriteInstancedShader.create(vertexShaderData, fragmentShaderData);
	mOrthographicMatrixLocationSpriteInstanced = 
		mSpriteInstancedShader.getLocation("orthographicMatrix");
	mSamplerLocationSpriteInstanced = mSpriteInstancedShader.getLocation("sampler");

	// Load water shader
	vertexShaderFile.close();
	vertexShaderFile.open("WaterVertexShader.glsl");
	vertexShaderData.assign(istreambuf_iterator<char>(vertexShaderFile),
		istreambuf_iterator<char>());
	fragmentShaderFile.close();
	fragmentShaderFile.open("WaterFragmentShader.glsl");
	fragmentShaderData.assign(istreambuf_iterator<char>(fragmentShaderFile),
		istreambuf_iterator<char>());

	mWaterShader.create(vertexShaderData, fragmentShaderData);
	mOrthographicMatrixLocationWater = mWaterShader.getLocation("orthographicMatrix");
	mDisplacementLocationWater = mWaterShader.getLocation("displacement");
	mSamplerLocationWater = mWaterShader.getLocation("sampler");
}

void Renderer::bindShader(Shader& shader)
{
	gl::UseProgram(shader.getID());
}

void Renderer::bindTexture(Texture& tex, int32_t slot)
{
	GLenum target;
	
	switch (tex.getType())
	{
	case Texture::Type::COMPRESSED_2D:
	case Texture::Type::UNCOMPRESSED_2D:
		target = gl::TEXTURE_2D;
		break;

	case Texture::Type::COMPRESSED_2D_ARRAY:
	case Texture::Type::UNCOMPRESSED_2D_ARRAY:
		target = gl::TEXTURE_2D_ARRAY;
		break;
	}

	gl::ActiveTexture(gl::TEXTURE0 + slot);
	gl::BindTexture(target, tex.getID());
}

void Renderer::bindVertexArray(VertexArray &arr)
{
	gl::BindVertexArray(arr.getID());
}

void Renderer::bindBuffer(GpuBuffer &buf)
{
	GpuBufferType t = buf.getType();
	bindBuffer(buf, t);
}

void Renderer::bindBuffer(GpuBuffer &buf, GpuBufferType &type)
{
	GLenum target;

	switch (type)
	{
	case GpuBufferType::VERTEX_BUFFER:
		target = gl::ARRAY_BUFFER;
		break;

	case GpuBufferType::COPY_READ_BUFFER:
		target = gl::COPY_READ_BUFFER;
		break;

	case GpuBufferType::COPY_WRITE_BUFFER:
		target = gl::COPY_WRITE_BUFFER;
		break;

	default:
		return;
	}

	gl::BindBuffer(target, buf.getID());
}

void Renderer::bindOrthoMatrix(const glm::mat4& orthoMatrix)
{
	mOrthoMatrix = orthoMatrix;
}

void Renderer::setDepthTest(bool state)
{
	if (state)
		gl::Enable(gl::DEPTH_TEST);
	else
		gl::Disable(gl::DEPTH_TEST);
}

void Renderer::setViewport(const glm::tvec2<int32_t>& dimensions)
{
	gl::Viewport(0, 0, dimensions.x, dimensions.y);
}

void Renderer::prepareDrawSprite()
{
	if (mSetup == Setup::SPRITE)
		return;

	bindShader(mSpriteShader);
	

	mSetup = Setup::SPRITE;
}

void Renderer::prepareDrawText()
{
	if (mSetup == Setup::TEXT)
		return;

	bindShader(mTextShader);
	mSetup = Setup::TEXT;
}

void Renderer::prepareDrawSpriteInstanced()
{
	if (mSetup == Setup::SPRITE_INSTANCED)
		return;

	bindShader(mSpriteInstancedShader);
	mSetup = Setup::SPRITE_INSTANCED;
}

void Renderer::drawSprite(const Sprite& sprite, const glm::mat4& transformMatrix)
{
	bindShader(mSpriteShader);

	gl::Uniform1i(mSamplerLocationSprite, 0); // Slot 0 for base images
	gl::BindSampler(0, sprite.mTexture->hasMipmap() ? mSamplerMipmapLinear : mSamplerLinear);
	gl::UniformMatrix4fv(mOrthographicMatrixLocationSprite, 1, gl::FALSE_,
		&(mOrthoMatrix * transformMatrix)[0][0]);
	bindTexture(*sprite.mTexture, 0); // Slot 0 for base images
	bindVertexArray(*sprite.mVao);
	gl::DrawArrays(gl::TRIANGLES, sprite.mVboPosition / sprite.mVao->getVertexSize(),
		sprite.mBuffer.size());
}

void Renderer::drawText(const Text& text, const glm::mat4& transformMatrix)
{
	gl::Uniform1i(mSamplerLocationText, 0); // Slot 0 for base images
	gl::BindSampler(0, text.mTexture->hasMipmap() ? mSamplerMipmapLinear : mSamplerLinear);
	gl::UniformMatrix4fv(mOrthographicMatrixLocationText, 1, gl::FALSE_,
		&(mOrthoMatrix * transformMatrix)[0][0]);
	gl::Uniform4fv(mTextColorLocationText, 1, &text.getColor()[0]);
	bindTexture(*text.mTexture, 0); // Slot 0 for base images
	bindVertexArray(*text.mVao);
	gl::DrawArrays(gl::TRIANGLES, text.mVboPosition / text.mVao->getVertexSize(),
		text.mBuffer.size());
}

void Renderer::drawSpriteInstanced(const Sprite& sprite, const glm::mat4& transformMatrix, 
	uint32_t instances)
{
	gl::Uniform1i(mSamplerLocationSpriteInstanced, 0); // Slot 0 for base images
	gl::BindSampler(0, sprite.mTexture->hasMipmap() ? mSamplerMipmapLinear : mSamplerLinear);
	gl::UniformMatrix4fv(mOrthographicMatrixLocationSpriteInstanced, 1, gl::FALSE_,
		&(mOrthoMatrix * transformMatrix)[0][0]);
	bindTexture(*sprite.mTexture, 0); // Slot 0 for base images
	bindVertexArray(*sprite.mVao);
	gl::DrawArraysInstanced(gl::TRIANGLES, sprite.mVboPosition / sprite.mVao->getVertexSize(),
		sprite.mBuffer.size(), instances);
}

bool Renderer::isInitialized() const
{
	return mInitialized;
}

