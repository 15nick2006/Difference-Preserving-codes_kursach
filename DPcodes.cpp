#include "DPcodes.h"
#include <iostream>
#include <vector>
#include <fstream>	
#include <string>

using namespace std;


ostream& operator<<(ostream& out, const DPcode& code) {
    if (code.found) {
        out << "N:" << code.N << " n:" << code.n << " t:" << code.t << "\n";
        for (int i = 0; i < code.words.size(); i++) {
            for (auto j : code.words[i]) {
                out << j;
            }
            out << "\n";
        }
        return out;
    }
    else {
        out << "N:" << code.N << " n:" << code.n << " t:" << code.t << "\n";
        out << "Fail no search";
        out << "\n";
        return out;
    }
}



int DPcodeBuilder::hamming(const vector<bool>& a, const vector<bool>& b) {
	int dis = 0;
	for (int i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) dis++;
	}
	return dis;
}
bool DPcodeBuilder::checkIsDP(const vector<vector<bool>>& code, int t, int pos) {
    if (pos > 0) {
        for (int i = 0; i <= pos - 1; i++) {
            for (int j = 1; j <= pos; j++) {
                int dist = hamming(code[i], code[j]);
                int indDiff = abs(i - j);
                if (indDiff <= t) {
                    if (dist != indDiff) return false;
                }
                else {
                    if (dist <= t) return false;
                }
            }
        }
    }
    return true;
}
bool DPcodeBuilder::generateCode(vector<vector<bool>>& words, int n, int t, int N, int pos) {
    if (pos == N) return true;

    int totalComb = 1 << n;
    for (int comb = 0; comb < totalComb; comb++) {
        vector<bool> word(n);
        for (int i = 0; i < n; i++) {
            word[i] = (comb >> i) & 1;
        }
        words[pos] = word;
        if (checkIsDP(words, t, pos)) {
            if (generateCode(words, n, t, N, pos + 1) && checkIsDP(words, t, pos))
            {
                return true;
            }
        }
    }
    return false;
}
bool DPcodeBuilder::generateLittle(DPcode& code) {
    code.words.resize(code.N, vector<bool>(code.n));
    return generateCode(code.words, code.n, code.t, code.N, 0);
}
bool DPcodeBuilder::genrateThrAdd(DPcode& code) {
    code.N += 1;
    int N1 = code.N/2;
    int N2 = code.N - N1;
    int n1 = code.n / 2;
    int n2 = code.n - n1;
    int t1 = code.t / 2;
    int t2 = code.t - t1;
    DPcode r1,r2;
    r1.N = N1; r1.n = n1; r1.t = t1;
    r2.N = N2; r2.n = n2; r2.t = t2;
    if (generateLittle(r1) && generateLittle(r2)) {

        code = thresholdAddition(r1, r2);
        return 1;
    }
    else {
        code.N--;
        return 0;
    }
}
DPcode DPcodeBuilder::builCode(int N, int n, int t) {
    DPcode result;
    result.N = N;
    result.n = n;
    result.t = t;
    DPcode fail;
    fail.found = false; fail.N = N; fail.n = n; fail.t = t;
    if (N<7 && n<5) {
        result.found = generateLittle(result);
        return result;
    }   
    else {
        result.found = genrateThrAdd(result);
        return result;
    }
}
void DPcodeBuilder::out(DPcode& code) {
    for (int i = 0; i < code.words.size(); i++) {
        for (auto j : code.words[i]) {
            cout << j;
        }
        if (i != 0) {
            cout << "    " << "haming: " << hamming(code.words[i], code.words[i - 1]);
        }
        cout << "\n";
    }
    cout << endl;
}
DPcode DPcodeBuilder::thresholdAddition(const DPcode& u, const DPcode& v) { 
    DPcode result;
    if (u.N < v.N) {
        result.N = u.N * 2;
        result.n = u.n + v.n;
        result.t = u.t + v.t;
        result.words.resize(result.N, vector<bool>(result.n));
        int k = 0;
        for (int i = 0; i < u.N; i++) {
            for (int j = i; j < i + 2; j++) {
                copy(u.words[i].begin(), u.words[i].end(), result.words[k].begin());
                copy(v.words[j].begin(), v.words[j].end(), result.words[k].begin()+u.n);
                k++;
            }
        }
        if (checkIsDP(result.words, result.t, result.N - 1)) {
            return result;
        }
        else {
            DPcode fail;
            return fail;
        }
    }
    else
    {
        result.N = (u.N * 2)-1;
        result.n = u.n + v.n;
        result.t = u.t + v.t;
        result.words.resize(result.N, vector<bool>(result.n));
        int k = 0;
        for (int i = 0; i < u.N-1; i++) {
            for (int j = i; j < i + 2; j++) {
                copy(u.words[i].begin(), u.words[i].end(), result.words[k].begin());
                copy(v.words[j].begin(), v.words[j].end(), result.words[k].begin() + u.n);
                k++;
            }
        }
        copy(u.words[u.words.size()-1].begin(), u.words[u.words.size() - 1].end(), result.words[k].begin());
        copy(v.words[v.words.size() - 1].begin(), v.words[v.words.size() - 1].end(), result.words[k].begin() + u.n);
        if (checkIsDP(result.words, result.t, result.N - 1)) {
            return result;
        }
        else {
            DPcode fail;
            return fail;
        }
    }

}
bool readAndWrite(vector<DPcode>& DP, char* inp) {
    string input = inp;
    ifstream in(input);
    if (!in.is_open()) {
        std::cerr << "errror file open!" << std::endl;
        return 0;
    }

    DPcode buf;
    while (in >> buf.N >> buf.n >> buf.t) {
        DP.push_back(buf);
    }

    in.close();
    return 1;
}
  
bool writeOut(vector<DPcode>& DP, char* ot) {
    string buf = ot;
    ofstream out;
    out.open(buf);
    if (out.is_open()) {
        for (auto& i : DP) {
            out << i;
            out <<  "\n" << "\n" << endl;
        }
        out.close();
        cout << "programm is complete" << endl;
        return 1;
    }
    else {
        return 0;
    }
}
