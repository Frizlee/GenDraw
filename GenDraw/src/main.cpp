#include "Prerequisites.hpp"
#include "Application.hpp"
using namespace std;


int main(int argc, char** argv)
{
	vector<string> args;

	for (size_t i = 0; i < static_cast<size_t>(argc); i++)
		args.push_back(string{ argv[i] });

	Application app;
	app.passArgs(move(args));
	app.start();

	return 0;
}