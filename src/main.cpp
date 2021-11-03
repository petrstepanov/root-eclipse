#include <TApplication.h>

// TODO: Include your ROOT script below
#include "yourRootScript.C"

int main(int argc, char **argv) {
	// Instantiate TApplication
	TApplication* app = new TApplication("rootEclipse", &argc, argv);

	// TODO: Call your ROOT script entry fiunction
	yourRootScript();

	// Enter the event loop
	app->Run();
	// Return success
	return 0;
}
