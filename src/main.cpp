#ifndef __CINT__
#include <RooGlobalFunc.h>
#endif

#include <TCanvas.h>
#include <TH1.h>

int main() {
	TH1* hist = new TH1D("hist", "hist", 10, 0, 10);
	hist->FillRandom("gaus",10000);
	hist->Draw();	
	return 0;
}
