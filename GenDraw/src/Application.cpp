#include "Application.hpp"
#include "Image.hpp"
#include "Texture.hpp"
#include "PngCodec.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"
#include "Sprite.hpp"
#include "GeneticDraw.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/vec2.hpp>
using namespace std;

static const size_t specCount{ 300 };
static const size_t bestCount{ 2 };


Application::Application()
{
}


Application::~Application()
{
}

void Application::passArgs(const std::vector<std::string>& argv)
{
	mArgv = argv;
}

void Application::passArgs(std::vector<std::string>&& argv)
{
	swap(mArgv, argv);
}

void Application::start()
{
	if (mArgv.size() < 2)
	{
		showUsage();
		return;
	}

	ifstream inputFile{ mArgv[1], ios_base::binary };

	if (!inputFile.is_open())
		return;

	mInputImage = make_shared<Image>();
	PngCodec{}.decode(inputFile, *mInputImage);
	inputFile.close();

	mWidth = mInputImage->getWidth();
	mHeight = mInputImage->getHeight();

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	mWnd = glfwCreateWindow(mWidth, mHeight, mArgv[1].c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(mWnd);
	mRenderer.init();
	mRenderer.setViewport(glm::tvec2<int32_t>{ mWidth, mHeight });
	glfwSwapInterval(0);
	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mCurrentImage = *mInputImage;
	mCurrentImageTexture = make_shared<Texture>();
	setTexture();
	
	mVaoSprite = make_shared<VertexArray>(
		make_shared<VertexBuffer<TextureVertexLayout>>(1024, mRenderer), mRenderer);

	glm::vec2 spriteSize{ static_cast<float>(mWidth), static_cast<float>(mHeight) };
	mOrthoMatrix = glm::ortho(-(mWidth / 2.0f), mWidth / 2.0f, -(mHeight / 2.0f), mHeight / 2.0f);

	mImageDisplay = make_shared<Sprite>(spriteSize, glm::vec2{ 0.0f, 0.0f }, 
		glm::vec2{ 1.0f, 1.0f }, 1.0f, mCurrentImageTexture, mVaoSprite, mRenderer);
	
	mWorker = make_unique<thread>(Worker, ref(*this));

	run();
}

void Application::showUsage()
{
	cout << "Usage: " << endl;
}

void Application::setTexture()
{
	mCurrentImageTexture->create(mCurrentImage, mRenderer);
}

void Application::run()
{
	const double updateTimeStep = 1.0 / 60.0;
	double accumulator = 0.0;

	auto previous = chrono::steady_clock::now();
	auto current = chrono::steady_clock::now();
	chrono::duration<double> diff;


	while (!glfwWindowShouldClose(mWnd))
	{
		current = chrono::steady_clock::now();
		diff = current - previous;
		previous = current;
		accumulator += mRealDeltaTime = diff.count();

		grabInput();

		while (accumulator > updateTimeStep)
		{
			accumulator -= updateTimeStep;

			calculateLogic(updateTimeStep);
		}
		
		renderScene();
		glfwSwapBuffers(mWnd);
	}

	mWorkerStop = true;
	mWorker->join();
}

void Application::grabInput()
{
	glfwWaitEvents();

	{
		lock_guard<mutex> lg(mCurrentImageMutex);
		setTexture();
	}
}

void Application::calculateLogic(double deltaTime)
{
}

void Application::renderScene()
{
	gl::Clear(gl::COLOR_BUFFER_BIT);
	mRenderer.bindOrthoMatrix(mOrthoMatrix);
	mRenderer.prepareDrawSprite();
	mRenderer.drawSprite(*mImageDisplay, 
	 	glm::translate(glm::vec3{ -(mWidth / 2.0f), -(mHeight / 2.0f), 0.0f }));
}

void Application::Worker(Application& app)
{
	GeneticDraw gd{ specCount, bestCount, app.mWidth, app.mHeight, app.mInputImage };
	int step = 0;

	while (!app.mWorkerStop)
	{
		// std::this_thread::sleep_for(2s);
		gd.mutate();
		gd.select();
		gd.dumpBest();
		
		{
			lock_guard<mutex> lg(app.mCurrentImageMutex);
			app.mCurrentImage = gd.pickRandomBest();
		}

		gd.prepareNextGen();
		step++;
		glfwPostEmptyEvent();
	}
}
