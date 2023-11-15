#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <conio.h>
#include <map>
#include <fstream>
#define ANY_ARROW 224
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
#define SINGLE 1
#define DOUBLE 2
template<typename T> using function = void (*)(T);
int line = 0;
//start at pre
using namespace std;
vector<string> funcs =
{
	"console.text",
	"save",
	"load"
};
map<string, string> strv;
map<string, int> intv;
map<string, bool> boolv;
string it;
string dt;
string cst;
void ok(string input, string pre, int line) {
	COORD c;
	c.X = 0;
	c.Y = line;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	cout << pre << input;
}
int woc() {
	CONSOLE_SCREEN_BUFFER_INFO c;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
	return c.dwSize.X;
}
int pos(string input, string data, string tolookfor) {
	return input.find(tolookfor, input.find(data));
}
void error(string error, string input, string data, string lookfor, bool next) {
	if (next == false) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); // \033[4m
		cerr << error << " pos: " << pos(input, data, lookfor) << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		line++;
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cerr << error << " pos: " << input.find(lookfor, pos(input, data, lookfor) + 1) << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		line++;
	}
}
int preformat(string & var, string quote) {
	/*cout << var << endl;
	cout << var.find(quote) << endl << var.find_last_of(quote) << endl << var.size() << endl << var[0];*/
	if (var.find(quote) != string::npos) {
		string test = var.substr(var.find(quote), var.find_last_of(quote) - var.find(quote) + 1);
		string test1 = var.substr(var.find(test) + test.size());
		string test2 = var.substr(0, var.find(test));
		if (test1.find_first_not_of(" ") != string::npos) {
			error("illegal string!", it, var, "", false);
			return 1;
		}
		else if (test2.find_first_not_of(" ") != string::npos) {
			error("illegal string!", it, var, "", false);
			return 1;
		}
		else {
			var = test;
			return 0;
		}
	}
	return -1;
}
int format(vector<string>& var, int quotes) { // second param is SINGLE or DOUBLE
	var[0] = var[0].substr(var[0].find_first_not_of("	"), var[0].find_last_not_of("	") + 1);
	var[1] = var[1].substr(var[1].find_first_not_of("	"), var[1].find_last_not_of("	") + 1);
	var[0] = var[0].substr(var[0].find_first_not_of(" "), var[0].find_last_not_of(" ") + 1);
	var[1] = var[1].substr(var[1].find_first_not_of(" "), var[1].find_last_not_of(" ") + 1);
	if (quotes == SINGLE) {
		int h = preformat(var[1], "'");
		if (h == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (quotes == DOUBLE) {
		int h = preformat(var[1], "\"");
		if (h == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}
}
void rc(string& var) {
	if (var[0] == '\'' && var[var.size() - 1] == '\'') {
		var = var.substr(1, var.size() - 2);
	}
	else if (var[0] == '"' && var[var.size() - 1] == '"') {
		var = var.substr(1, var.size() - 2);
	}
}
namespace stringX {
	void replace_all(string& mainString, string stringToReplace, string stringToReplaceWith) {
		for (int i = mainString.find(stringToReplace); i != string::npos; i = mainString.find(stringToReplace)) {
			mainString.replace(i, stringToReplace.size(), stringToReplaceWith);
		}
	}
	void replace(string& mainString, string stringToReplace, string stringToReplaceWith, int findNthOccurrence) { //if 4th param is null (0), will replace first instance of string
		int inc = 0;
		int nth = mainString.find(stringToReplace);
		while (nth != string::npos) {
			if (inc == findNthOccurrence) {
				break;
			}
			else {
				nth = mainString.find(stringToReplace, nth + stringToReplace.size());
				inc++;
			}
		}
		mainString.replace(nth, stringToReplace.size(), stringToReplaceWith);
	}
	void splitString(string mainString, vector<string>& vec, string stringToSplitWith) {
		int fPos = 0;
		int lPos = mainString.find(stringToSplitWith);
		while (lPos != string::npos) {
			vec.push_back(mainString.substr(fPos, lPos - fPos));
			fPos = lPos + stringToSplitWith.size();
			lPos = mainString.find(stringToSplitWith, fPos);
		}
		vec.push_back(mainString.substr(fPos));
	}
	int numOfStr(string mainString, string strToCount) {
		int amount = 0;
		string tempMain = mainString;
		while (tempMain.find(strToCount) != string::npos) {
			amount++;
			replace(tempMain, strToCount, "", NULL);
		}
		return amount;
	}
	bool isnum(string mainString) {
		return mainString.find_first_not_of("0123456789") == string::npos;
	}
	void removeNonInt(string& mainString) {
		int pos = 0;
		string tempMain = "";
		while (pos != mainString.size()) {
			if (isnum(string(1, mainString[pos]))) {
				tempMain.append(string(1, mainString[pos]));
			}
			pos++;
		}
		mainString = tempMain;
	}
	void reverse(std::string& string) {
		std::string ts;
		for (int i = 0; i < string.size(); i++) {
			ts += string[string.size() - 1 - i]; // size - 1 cuz size returns starting from 1 but index starts from 0 so ye
		}
		string = ts;
	}
	void encrypt(std::string& data) {
		std::string td;
		for (int i = 0; i < data.size(); i++) {
			char fc = data[i];
			int sc = (int)fc;
			std::string ti = std::to_string(sc - 1);
			int tioti = std::stoi(ti);
			td += (char)tioti;
		}
		reverse(td);
		data = td;
	}
	void decrypt(std::string& data) {
		std::string td;
		for (int i = 0; i < data.size(); i++) {
			int fc = (int)data[i];
			std::string ti = std::to_string(fc + 1);
			int tioti = std::stoi(ti);
			td += (char)tioti;
		}
		reverse(td);
		data = td;
	}
	string type_t(string pre) {
		cout << pre;
		string input;
		/*
		for (int i = 0; i < line; i++) {
			input.append("\n");
			cout << "\n";
		}*/
		int rowPos = 0;
		//int linePos = 0;
		//13 enter?
		for (int i = 0;;) {
			i = _getch();
			if (i == 8) {
				if (input.size() > 0 && rowPos > 0) {
					if (rowPos == input.size()) {
						//cout << "\b \b";
						input.pop_back();
						ok(input, pre, line);
						cout << " \b";
						/*
						if (input.substr(input.size() - 1) == "\n") {
							linePos = linePos - 1;
						}
						*/
						rowPos = rowPos - 1;
					}
					else {
						input = input.substr(0, rowPos - 1) + input.substr(rowPos);
						ok(input, pre, line);
						cout << " \b\b";
						for (int i = 0; i < input.size() - rowPos; i++) {
							cout << "\b";
						}
						/*
						cout << " \b";*/
						/*
						if (input.substr(input.size() - 1) == "\n") {
							linePos = linePos - 1;
						}
						*/
						rowPos = rowPos - 1;
					}
				}
			}
			else if (GetKeyState(VK_SHIFT) & 0x8000) {
				if (i == 13) {
					if (rowPos >= 0) {
						if (rowPos == input.size()) {
							cout << endl;
							input = input + "\n";
							//linePos = linePos + 1;
							rowPos = rowPos + 1;
						}
						else {
							input = input.substr(0, rowPos - 1) + "\n" + input.substr(rowPos);
							for (int i = 0; i < input.substr(rowPos - 1).size(); i++) {
								cout << " ";
							}
							cout << "\b\b";
							ok(input, pre, line);
							for (int i = 0; i < input.substr(rowPos - 1).size(); i++) {
								cout << "\b";
							}
							//linePos = linePos + 1;
							rowPos = rowPos + 1;
						}
					}
				}
				else {
					if (rowPos == input.size()) {
						input = input + (char)i;
						ok(input, pre, line);
						rowPos = rowPos + 1;
					}
					else {
						input = input.substr(0, rowPos) + (char)i + input.substr(rowPos);
						ok(input, pre, line);
						for (int i = 0; i < input.size() - rowPos; i++) {
							cout << "\b";
						}
						rowPos = rowPos + 1;
					}
				}
			}
			else if (i == 13) {
				if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
					break;
				}
			}
			else {
				switch (i) {
				case ANY_ARROW:
					i = _getch();
					switch (i) {
					case LEFT_ARROW:
						if (rowPos > 0) {
							cout << "\b";
							rowPos = rowPos - 1;
						}
						break;
					case RIGHT_ARROW:
						if (rowPos < input.length()) {
							CONSOLE_SCREEN_BUFFER_INFO csbi;
							COORD c;
							GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
							c.Y = csbi.dwCursorPosition.Y;
							c.X = csbi.dwCursorPosition.X + 1;
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
							rowPos = rowPos + 1;
						}
						break;
					case UP_ARROW:
						break;
					case DOWN_ARROW:
						break;
					}
					break;
				default:
					if (rowPos == input.size()) {
						input = input + (char)i;
						ok(input, pre, line);
						rowPos = rowPos + 1;
					}
					else {
						input = input.substr(0, rowPos) + (char)i + input.substr(rowPos);
						ok(input, pre, line);
						for (int i = 0; i < input.size() - rowPos; i++) {
							cout << "\b";
						}
						rowPos = rowPos + 1;
					}
				}
			}
		}
		line++;
		for (int i = 0; i < numOfStr(input, "\n"); i++) {
			line++;
		}
		return input;
	}
}
int str(vector<string> & var, string input, string data) {
	if (stringX::numOfStr(var[1], "\"") == 2) {
		stringX::replace_all(var[1], "\\n", "\n");
		return DOUBLE;
	}
	else if (stringX::numOfStr(var[1], "\"") == 1) {
		error("forgot \" !", input, data, "\"", false);
		return -1;
	}
	else if (stringX::numOfStr(var[1], "\"") > 2) {
		int find = var[1].find("\"");
		for (int i = 0; i < stringX::numOfStr(var[1], "\""); i++) {
			if (find > 0) {
				if (var[1].substr(find - 1, find - 1) == "\\") {
					find = var[1].find("\"", find + 1);
				}
				else {
					error("forgot \"\\\" before \"!", input, data, "\"", true);
					return -1;
				}
			}
			else {
				find = var[1].find("\"", find + 1);
			}
		}
		return DOUBLE;
	}
	else if (stringX::numOfStr(var[1], "'") == 2) {
		stringX::replace_all(var[1], "\\n", "\n");
		return SINGLE;
	}
	else if (stringX::numOfStr(var[1], "'") == 1) {
		error("forgot \"'\" !", input, data, "'", false);
		return -1;
	}
	else if (stringX::numOfStr(var[1], "'") > 2) {
		for (int i = 0; i < stringX::numOfStr(var[1], "'"); i++) {
			if (var[1].find("'") > 0 && var[1].substr(var[1].find("'") - 1, var[1].find("'")) == "\\") {
				continue;
			}
			else {
				error("forgot \"\\\" before '!", input, data, "\"", true);
				return -1;
			}
		}
		return SINGLE;
	}
	else {
		return NULL;
	}
}
void precompile(vector<string> var, string input, string data, function<vector<string>> ifstr, function<vector<string>> els) {
	it = input;
	dt = data;
	int strr = str(var, input, data);
	rc(var[1]);
	if (strr != NULL && strr != -1) {
		ifstr(var);
	}
	else if (strr == -1) {
		return;
	}
	else {
		els(var);
	}
}
void ifstrvar(vector<string> var) {
	intv.erase(var[0]);
	boolv.erase(var[0]);
	rc(var[1]);
	strv[var[0]] = var[1];
}
void elsvar(vector<string> var) {
	int inc = 0;
	format(var, NULL);
	if (var[1] == "true") {
		strv.erase(var[0]);
		intv.erase(var[0]);
		boolv[var[0]] = true;
		return;
	}
	else if (var[1] == "false") {
		strv.erase(var[0]);
		intv.erase(var[0]);
		boolv[var[0]] = false;
		return;
	}
	else if (stringX::isnum(var[1])) {
		long long conv = strtoll(var[1].c_str(), nullptr, 10);
		if (conv < INT64_MAX) {
			strv.erase(var[0]);
			boolv.erase(var[0]);
			intv[var[0]] = conv;
		}
		else {
			error("number is too big! variable could not be initialized.", it, dt, var[1], false);
			line++;
		}
		return;
	}
	else {
		error("illegal number!", it, dt, var[1], false);
		return;
	}
}
void ifstrtext(vector<string> var) {
	int a = stringX::numOfStr(var[1], "\n");
	for (int i = 0; i < a; i++) {
		line++;
	}
	cout << var[1];
}
void elstext(vector<string> var) {
	map<string, string>::iterator str = strv.begin();
	map<string, int>::iterator i = intv.begin();
	map<string, bool>::iterator bo = boolv.begin();
	int inc = 0;
	if (var[1] == "true") {
		cout << true;
		return;
	}
	else if (var[1] == "false") {
		cout << false;
		return;
	}
	else if (stringX::isnum(var[1])) {
		long long conv = strtoll(var[1].c_str(), nullptr, 10);
		if (conv < INT64_MAX) {
			cout << conv;
		}
		else {
			error("number is too big! cannot be converted to a number.", it, dt, var[1], false);
			cout << endl << var[1];
			line++;
		}
		return;
	}
	while (str != strv.end()) {
		if (var[1] == str->first) {
			cout << str->second;
			int a = stringX::numOfStr(var[1], "\n");
			for (int i = 0; i < a; i++) {
				line++;
			}
			return;
		}
	}
	while (i != intv.end()) {
		if (var[1] == i->first) {
			cout << i->second;
			return;
		}
	}
	while (bo != boolv.end()) {
		if (var[1] == bo->first) {
			cout << bo->second;
			return;
		}
	}
	error("malformed number or bool!", it, dt, var[1], false);
	return;
}
void ifstrsave(vector<string> var) {
	var[2] = var[2].substr(0, var[2].find("save"));
	ofstream save(var[1].c_str());
	save << var[2];
}
void elssave(vector<string> var) {
	map<string, string>::iterator str = strv.begin();
	map<string, int>::iterator i = intv.begin();
	map<string, bool>::iterator bo = boolv.begin();
	int inc = 0;
	while (str != strv.end()) {
		if (var[1] == str->first) {
			var[2] = var[2].substr(0, var[3].find("save"));
			ofstream save(str->second.c_str());
			save << var[2];
			return;
		}
	}
	while (i != intv.end()) {
		if (var[1] == i->first) {
			error("path is int!", it, dt, var[0], false);
			return;
		}
	}
	while (bo != boolv.end()) {
		if (var[1] == bo->first) {
			error("path is bool!", it, dt, var[0], false);
			return;
		}
	}
	error("malformed number or bool!", it, dt, var[1], false);
	return;
}
void ifstrload(vector<string> var) {
	string input1;
	string input2;
	input1 = var[2].substr(0, var[2].find("load"));
	input2 = var[2].substr(var[2].find(")", var[2].find("load")) + 1);
	var[2] = ""; // input1
	ifstream save;
	save.open(var[1].c_str());
	string data;
	while (getline(save, data)) {
		var[2] = var[2] + "\n" + data;
	}
	var[2] = var[2] + "\n" + input2;
	cst = var[2];
}
void elsload(vector<string> var) {
	map<string, string>::iterator str = strv.begin();
	map<string, int>::iterator i = intv.begin();
	map<string, bool>::iterator bo = boolv.begin();
	int inc = 0;
	while (str != strv.end()) {
		if (var[1] == str->first) {
			string input1;
			string input2;
			input1 = var[2].substr(0, var[2].find("load"));
			input2 = var[2].substr(var[2].find(")", var[2].find("load")) + 1);
			var[2] = ""; // input1
			ifstream save;
			save.open(str->second.c_str());
			string data;
			while (getline(save, data)) {
				var[2] = var[2] + "\n" + data;
			}
			var[2] = var[2] + "\n" + input2;
			cst = var[2];
			return;
		}
	}
	while (i != intv.end()) {
		if (var[1] == i->first) {
			error("path is int!", it, dt, var[1], false);
			return;
		}
	}
	while (bo != boolv.end()) {
		if (var[1] == bo->first) {
			error("path is bool!", it, dt, var[1], false);
			return;
		}
	}
	error("malformed number or bool!", it, dt, var[1], false);
	return;
}
void compile(string input, string data) {
	//FUNCTIONS
	if (data.substr(0, funcs[0].size()) == funcs[0]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			if (str(b, input, data) == DOUBLE) {
				int form = format(b, DOUBLE);
				if (form == 1 || form == -1) {
					return;
				}
			} 
			else if (str(b, input, data) == SINGLE) {
				int form = format(b, SINGLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(b, input, data) == -1) {
				return;
			}
			precompile(b, input, data, ifstrtext, elstext);
		}
		else {
			error("function has been called inavlidly", input, data, "(", false);
			return;
		}
	}
	else if (data.substr(0, funcs[1].size()) == funcs[1]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			if (str(b, input, data) == DOUBLE) {
				int form = format(b, DOUBLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(b, input, data) == SINGLE) {
				int form = format(b, SINGLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(b, input, data) == -1) {
				return;
			}
			b.push_back(input);
			precompile(b, input, data, ifstrsave, elssave);
			return;
		}
		else {
			error("function has been called inavlidly", input, data, "(", false);
			return;
		}
	}
	else if (data.substr(0, funcs[2].size()) == funcs[2]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			b.push_back(input);
			if (str(b, input, data) == DOUBLE) {
				int form = format(b, DOUBLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(b, input, data) == SINGLE) {
				int form = format(b, SINGLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(b, input, data) == -1) {
				return;
			}
			precompile(b, input, data, ifstrload, elsload);
			vector<string> ans;
			stringX::splitString(cst, ans, "\n");
			for (auto data : ans) {
				compile(cst, data);
			}
			return;
		}
		else {
			error("function has been called inavlidly", input, data, "(", false);
			return;
		}
	}
	//VARIABLES
	else if (stringX::numOfStr(data, "=") > 0) {
		if (stringX::numOfStr(data, "=") == 1) {
			vector<string> var;
			stringX::splitString(data, var, "=");
			/*
			cout << var.size();
			if (var.size() < 2) {
				if (data.find(var[0]) > data.find("=")) {
					cerr << "variable name is empty!";
					return;
				}
				else if (data.find(var[0]) < data.find("=")) {
					cerr << "variable value is empty!";
					return;
				}
				else {
					cerr << "illegal variable! possibly missing both value and name!";
					return;
				}
				return;
			}*/
			if ((var[0].find_first_not_of("") == string::npos || var[0].find_first_not_of(" ") == string::npos) && (var[1].find_first_not_of("") == string::npos || var[1].find_first_not_of(" ") == string::npos)) {
				error("missing variable name and value!", input, data, "", false);
				return;
			}
			if (var[0].find_first_not_of("") == string::npos || var[0].find_first_not_of(" ") == string::npos) {
				error("variable name is empty!", input, data, var[0], false);
				return;
			}
			if (var[1].find_first_not_of("") == string::npos || var[1].find_first_not_of(" ") == string::npos) {
				error("variable value is empty!", input, data, var[1], false);
				return;
			}
			if (str(var, input, data) == DOUBLE) {
				int form = format(var, DOUBLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(var, input, data) == SINGLE) {
				int form = format(var, SINGLE);
				if (form == 1 || form == -1) {
					return;
				}
			}
			else if (str(var, input, data) == -1) {
				return;
			}
			precompile(var, input, data, ifstrvar, elsvar);
		}
	}
	else {
		if (data.find_first_not_of(" ") != string::npos) {
			error("unknown syntax!", input, data, data, false);
		}
		return;
	}
}
int main()
{
	string input = "";
	while (true) {
		input = stringX::type_t(">> ");
		string tinput = input;
		cout << endl;
		vector<string> ans;
		stringX::splitString(input, ans, "\n");
		for (auto data : ans) {
			compile(tinput, data);
		}
		cout << endl;
		line++;
	}
	/*
	if (stringX::numOfStr(input, "\n") > 0) {
	}*/
}
