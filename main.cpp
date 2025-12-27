#include "DPcodes.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int arg, char* pi[]) {
	DPcodeBuilder builder;
	vector<DPcode> enters;
	if (readAndWrite(enters, pi[1])) {
		for (auto& i : enters) {
			i = builder.builCode(i.N, i.n, i.t);
		}
		writeOut(enters, pi[2]);
	}
	else {
		cout << "not Write";
	}
}

