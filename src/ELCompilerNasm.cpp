/*
 * ELCompilerNasm.cpp
 *
 *  Created on: Feb 2, 2025
 *      Author: nobady
 */

#include "ELCompilerNasm.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>

using namespace std;


//把原始馬分段
vector<string> Trim(string Line){
	vector<string> List;
	size_t StartPos = 0;
	size_t EndPos = Line.length();
	string Text;

	cout << "Source code: ";

	int Count = 0;
	for (StartPos = 0; StartPos < EndPos; StartPos++){
		if( Line[StartPos] != ' ' and Line[StartPos] != '\t'){
			switch (Line[StartPos]){
			case '(':
				goto SpecialCharacter;
				break;
			case ')':
				goto SpecialCharacter;
				break;
			case '{':
				goto SpecialCharacter;
				break;
			case '}':
				goto SpecialCharacter;
				break;
			default:
				Text = Text + Line[StartPos];
				break;
			}


			if (StartPos != EndPos - 1){
				continue;
			};

		}

		if (!Text.empty()){
			cout << Text << " ";
			List.insert(List.begin() + Count, Text);
			Text.clear();
			Count ++;
		}
		continue;

		SpecialCharacter:

		if (!Text.empty()){
			cout << Text << " ";
			List.insert(List.begin() + Count, Text);
			Text.clear();
			Count ++;
		}
		Text = Line[StartPos];
		List.insert(List.begin() + Count, Text);
		cout << Text << " ";
		Text.clear();
		Count ++;
		continue;

	}
	return List;
}

// var a = ...
void NewVar(vector<string> SourceCode){
	string name;
	string Oper;
	string value;
	if(SourceCode[2] == "="){
		cout <<"    //compiled to NASM >> "<<endl;
		cout << SourceCode[1] << " db " << SourceCode[3]<<endl;
	}else{
		cout << "error, '=' expected\n" << endl;
	}
}

// function a()
void NewFunction(vector<string> SourceCode){
	string name;
	cout <<  "    //compiled to NASM >> "<<endl;
	cout << SourceCode[1] << ":" <<endl;
}

// a = b
void Equ(vector<string> SourceCode){
	cout << "    //compiled to NASM >> "<<endl;
	cout << "mov eax, [" << SourceCode[2] <<"]"<< endl;
	cout <<"mov [" << SourceCode[0] << "], eax"<<endl;
}
