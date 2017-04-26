#pragma once
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include "Renderer.hpp"
#include "Image.hpp"

class Application
{
public:
	Application();
	~Application();

	void passArgs(const std::vector<std::string>& argv);
	void passArgs(std::vector<std::string>&& argv);

	void start();
	void showUsage();
	void setTexture();
	
	void run();
	void grabInput();
	void calculateLogic(double deltaTime);
	void renderScene();

private: 
	Renderer mRenderer;
	std::vector<std::string> mArgv;
	std::shared_ptr<Image> mInputImage;
	std::mutex mCurrentImageMutex;
	Image mCurrentImage;
	std::shared_ptr<class Texture> mCurrentImageTexture;
	std::shared_ptr<class VertexArray> mVaoSprite;
	std::shared_ptr<class Sprite> mImageDisplay;
	glm::mat4 mOrthoMatrix;
	uint32_t mWidth;
	uint32_t mHeight;
	GLFWwindow *mWnd;
	double mRealDeltaTime;
	std::unique_ptr<std::thread> mWorker;
	bool mWorkerStop;

	static void Worker(Application& app);
};

