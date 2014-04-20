#include "App.h"

int main()
{
	CApp* app = new CApp();

	app->Run();

	delete app;
}