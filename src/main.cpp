#include <TApplication.h>
#include <TCanvas.h>
#include <TH1.h>

// Paste your ROOT script code here
void yourRootScritCode(){
	TH1* hist = new TH1D("hist", "hist", 10, 0, 10);
	hist->FillRandom("gaus",10000);
	hist->Draw();
}

// Below code is for standalone build. Not visible for ROOT interpreter.
#ifndef __CINT__

int main(int argc, char **argv) {
	// Instantiate TApplication
	TApplication* app = new TApplication("rootEclipse", &argc, argv);
	// Execute your ROOT script
	yourRootScritCode();
	// Enter the event loop
	app->Run();
	// Return success
	return 0;
}

#endif
