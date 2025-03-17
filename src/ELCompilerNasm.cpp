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
#include <future>
#include "ELFileReader.h"

using namespace std;

bool FirstTime = true;
//把原始馬分段


// var a = ...
void NewVar(vector<string> SourceCode){
	string name;
	string Oper;
	string value;
	cout <<"state"<< GetLayerState();
	if(SourceCode[2] == "="){
		cout <<"    //compiled to NASM >> "<<endl;
		cout << SourceCode[1] << " db " << SourceCode[3]<<endl;
	}else{
		cout << "error, '=' expected\n" << endl;
	}
}

// function a()
void NewFunction(vector<string> SourceCode){
	cout << "state" << GetLayerState();
	cout <<  "    //compiled to NASM >> "<<endl;
	cout << SourceCode[1] << ":" <<endl;
}

// a = b
void Equ(vector<string> SourceCode){
	cout << "state" <<GetLayer();
	cout << "    //compiled to NASM >> "<<endl;
	cout << "mov eax, [" << SourceCode[2] <<"]"<< endl;
	cout <<"mov [" << SourceCode[0] << "], eax"<<endl;
}
