// Replace this script with your ROOT file

#include <TCanvas.h>
#include <TH1.h>

void yourRootScript(){
	TH1* hist = new TH1D("hist", "hist", 10, 0, 10);
	hist->FillRandom("gaus",10000);
	hist->Draw();
}
