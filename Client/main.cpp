#include "App.h"

int main()
{
	CApp* app = new CApp();

	if (app->Connect())
	{
		app->Run();
	}

	delete app;
}