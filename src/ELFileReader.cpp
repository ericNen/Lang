/*
 * ELFileReader.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: er
 */
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "ELFileReader.h"

using namespace std;



vector<string> Object::MainTypes;
vector<string> Object::Unused;
vector<string> Object::ChildTypes;
vector<string> Object::CommonPrefix;
vector<string> Object::VarPrefix;
vector<string> Object::FuncPrefix;

vector<char> ValidObjNameChar = {
		'a','b','d'
};

void Object::Initialize(){
	ChildTypes.insert(ChildTypes.begin(), "any");
	ChildTypes.insert(ChildTypes.begin(), "string");
	ChildTypes.insert(ChildTypes.begin(), "char");
	ChildTypes.insert(ChildTypes.begin(), "int");
	ChildTypes.insert(ChildTypes.begin(), "list");


	MainTypes.insert(MainTypes.begin(), "var");
	MainTypes.insert(MainTypes.begin(), "function");

	FuncPrefix.insert(FuncPrefix.begin(), "parallel");
};

bool Object::RegisterNewType(string name, vector<string> childs){
	return 0;
};

int Layer;

bool LayerControl(int CharacterLabel){
	switch (CharacterLabel){
	case 5:
		break;
	case 6:
		RemoveLayer();
		break;
	default:
		return false;
		break;
	}
	return true;
}
int GetLayer(){ return Layer; };
void SetLayer(int number){ Layer = number;};
void IncreaseLayer(){ Layer ++; };
void DecreaseLayer(){ Layer --; };

string CurrentLayerState;
vector<string> LayerState;
string GetLayerState(){
	if (LayerState.size() == 0){ return "NoLayer";}
	return LayerState[LayerState.size()-1];
};
void AddLayer(string State){ LayerState.insert(LayerState.begin() + LayerState.size(), State); };
void RemoveLayer(){ LayerState.erase(LayerState.end() + LayerState.size()); };



bool IsCompiling = true;
enum class TrimingState:short{
	None ,String ,Number ,Byte
};
TrimingState TrimState = TrimingState::None;

//False if it is empty after triming
bool Trim(string Line){
	size_t StartPos = 0;
	size_t EndPos = Line.length();
	string Text;
	bool HasValidCode = false;

	CheckTag(Line);
	// This will check for #Start #Pause #End tag and it will effect bool {IsCompiling} Global variable
	if (IsCompiling == false){
		// #Pause used than skip triming this line
		return HasValidCode;
	}

	int Count = 0;
	for (StartPos = 0; StartPos < EndPos; StartPos++){
		if( Line[StartPos] != ' ' and Line[StartPos] != '\t'){

			if (Text.size() == 1 && Text[0] == '/'  && Line[StartPos] == '/'){
				// ensure there is at least 2 character
				// If true than it is a command
				return HasValidCode;
			}


			// "string or char"
			if (TrimState == TrimingState::String){
				if (Line[StartPos] == '\"'){
					//Last " of the string
					TrimState = TrimingState::None ;
					Text += Line[StartPos];
				}else{
					Text += Line[StartPos];
				}

			}
			else if(Line[StartPos] == '\"'){
				//First " of the string
				TrimState = TrimingState::String;
				Text += Line[StartPos];
			}
			else{
				if (CheckSpecialCharacter(Line[StartPos]) != 0){
					// Special character
					Interpret(Text);
					// Previous Text ex: "var<" in "var<int>" the "var" will be the text before "<"
					Text.clear();
					// Text become empty
					Text += Line[StartPos];
					// Add "<" to the Text
					Interpret(Text);
					Text.clear();
				}else{
					// Not Special character. Normal condition
					Text += Line[StartPos];
				}
			}


			if (StartPos != EndPos - 1){
				// If "StartPos" equals to "EndPos-1" than it is the end of the line
				continue;
			}
		}
		Interpret(Text);
		HasValidCode = true;
		Text.clear();
	}

	return HasValidCode;
}

void CheckTag(string Text){
	if(Text == "#Start"){
		IsCompiling = true;
		return;
	}else if(Text == "#Pause"){
		IsCompiling = false;
	}else if(Text == "#End"){
		cout << endl<<endl<<">>>Program Terminated ";
		abort();
	}
}

enum class InterpState:short{
	None,VarDec,FunDec,VarTypeDec,FunTypeDec,ClassDec,AssignVal,FunParams,ParamTypeDec
};
bool IsInsideFunction;
InterpState IntpState;
string PreviousUsedObjName;    // "a" in "var a = b"   or    "a" in "a = b"

bool InterpretAgain = false;
void Interpret(string Text){
	if (Text.empty())return;
	switch(IntpState){
	case InterpState::VarDec:
		DeclarVar(Text);
		break;
	case InterpState::VarTypeDec:
		DeclarVar(Text);
		break;
	case InterpState::FunDec:
		DeclarFun(Text);
		break;
	default:
		// not in any declaration
		goto Check;
		break;
	}
	goto End; // If the process is already known

	Check:
	if(Text == "var"){
		cout << endl;
		IntpState = InterpState::VarDec;
	}else if(Contains(Object::VarPrefix, Text)){
		cout << endl;
		IntpState = InterpState::VarDec;
	}else if(Text == "function"){
		cout << endl;
		IntpState = InterpState::FunDec;
	}else{
		cout << endl;
	}


	End:

	if(InterpretAgain){
		// when the instruction type change
		InterpretAgain = false;
		Interpret(Text);
		return;
	}

	cout <<"{" << Text <<"} ";
}


string VarName;
string VarType = "any";
bool IsWaitingForDeclare = false;
void DeclarVar(string Text){
	if(IntpState == InterpState::VarTypeDec){
		// declaring tyle <Type>
		if(Text != ">"){
			// "Type" in "<Type>"
			VarType = Text;
			cout << " Type:";
			return;
		}
	}

	// special characters
	if(CheckSpecialCharacter(Text[0]) != 0){
		// It is a Special Character
		if(Text == "<"){
			// First < of declaring type
			IntpState = InterpState::VarTypeDec;
		}else if (Text == ">"){
			// Last > of declaring type
			IntpState = InterpState::VarDec;
		}else if (Text == "="){
			IsWaitingForDeclare = true;
			cout << "Assign:";
		}else if (Text == ";"){
			IsWaitingForDeclare = false;
			IntpState = InterpState::None;
		}

		return;
	}


	// words ( not a special character)
	if(Text == "var"){
		if (!VarName.empty()){
			IsWaitingForDeclare = false;
			cout << " New ";
			VarName.clear();
		}
	}else if(Contains(Object::VarPrefix, Text)){
		if (!VarName.empty()){
			IsWaitingForDeclare = false;
			cout << " New ";
			VarName.clear();
		}
	}else{
		// not reserved symbol
		if(VarName.empty()){
			cout << "Name:";
			VarName = Text;
		}else if(IsWaitingForDeclare){
			// var a = b c
			// 				^ already assigned so the next symbol "c" must be in next line
			cout << "Value:";
			IsWaitingForDeclare = false;
			VarName.clear();
			IntpState = InterpState::None;
		}else{
			// var a  b = c
			//			  ^Here "b = c" is a different line
			InterpretAgain = true;
			IsWaitingForDeclare = false;
			VarName.clear();
			IntpState = InterpState::None;
		}
	}
}

void DeclarFun(string Text){
	cout << "Func  ";
}

vector<string> PrefixKeyWords = {
	"parallel"
};

vector<string> Tags = {
	"#NoCompile"
};



vector<VariableType> DefinedVariable = {};
vector<FunctionType> DefinedFunction = {};

vector<string> ObjToLookup = {};

bool Contains(vector<string> &list, string item){
	if( find(list.begin(), list.end(), item) != list.end() ){
		return true;
	}else{return false;};
};




void AssignValue(){

}

void UseTags(vector<string> &Snippet){
	if (Snippet[0] == "#NoCompile"){
		CurrentLayerState = "NoCompile";
	}
};


int CheckSpecialCharacter(char word){
	switch (word) {
	case '(':
		return Bracket1Start;
		break;
	case ')':
		return Bracket1End;
		break;
	case '{':
		return Bracket3Start;
		break;
	case '}':
		return Bracket3End;
		break;
	case '=':
		return EqualMark;
		break;
	case ',':
		return Comma;
		break;
	case '<':
		return AngleBracketStart;
		break;
	case '>':
		return AngleBracketEnd;
		break;
	default:
		break;
	}
	return Others;
};



