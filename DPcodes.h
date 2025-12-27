#include <iostream>
#include<vector>

using namespace std;

struct DPcode
{
	int N;
	int n;
	int t;
	vector<vector<bool>> words;
	bool found;
};

ostream& operator<<(ostream& out, const DPcode& code);

class DPcodeBuilder {
private:
	bool generateLittle(DPcode& code);
	bool generateCode(vector<vector<bool>>& words, int n, int t, int N, int pos);
	bool checkIsDP(const vector<vector<bool>>& words, int t, int pos);
	int hamming(const vector<bool>& a, const vector<bool>& b);
	DPcode thresholdAddition(const DPcode& u, const DPcode& v);
	bool genrateThrAdd(DPcode& code);
public:
	DPcode builCode(int N, int n, int t);
	void out(DPcode& code);
};	

bool readAndWrite(vector<DPcode>& DP, char* inp);
bool writeOut(vector<DPcode>& DP, char* ot);