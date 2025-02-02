//============================================================================
// Name        : ELCompiler.cpp
// Author      : no
// Version     :
// Copyright   : Your copyright notice
// Description : ELCompiler, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>
#include "ELCompilerNasm.h"

using namespace std;

void ReadFile(string FullFilePath);

int main() {
	cout << ">> program started <<" << endl;
	string FilePath = "/home/er/Desktop/Test.el";

	ReadFile(FilePath);

	cout << "end";
	return 0;
}


void ReadFile(string FullFilePath){
	ifstream FileSt(FullFilePath);
	if(! FileSt.is_open() ){
		cout << "file opening failed" << endl;
		return;
	}else{
		cout<<"file opening success >>>" << endl << endl;
	};


	string s;
	while (getline(FileSt, s)){
		if(s.empty()) continue;

		vector<string> Words = Trim(s);

		if(Words[0] == "var"){
			NewVar(Words);
		}
		else if(Words[0] == "function"){
			NewFunction(Words);
		}
		else if (Words[1] == "="){
			Equ(Words);
		}else{
			cout << "nothing";
		}
		cout << endl;
	}
}






