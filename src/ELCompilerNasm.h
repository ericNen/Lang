/*
 * ELCompilerNasm.h
 *
 *  Created on: Feb 2, 2025
 *      Author: nobody
 */

#ifndef ELCOMPILERNASM_H_
#define ELCOMPILERNASM_H_
#pragma once






#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>

using namespace std;

void NewVar(vector<string> SourceCode);
void NewFunction(vector<string> SourceCode);
void Equ(vector<string> SourceCode);
vector<string> Trim(string Line);


#endif /* ELCOMPILERNASM_H_ */
