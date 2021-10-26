#ifndef __CINT__
#include <RooGlobalFunc.h>
#endif

#include <TCanvas.h>
#include <TH1.h>

void myFunc(){
  ;
}

int main() {
	TH1* hist = new TH1D("hist", "hist", 10, 0, 10);
	hist->FillRandom("gaus",10000);
	hist->Draw();	
	myFunc();
	return 0;
}
