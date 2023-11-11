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
			//cout << mainString;
			if (string(1, tempMain[0]) == strToCount) {
				tempMain = tempMain.substr(1);
				amount++;
			}
			else {
				tempMain = tempMain.substr(1);
			}
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
							cout << "\033[C"; //idk how tf this works whoever's reading this pls explain!11
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

void precompile(vector<string> var, string data, function<vector<string>> ifstr, function<vector<string>> els) {
	if (stringX::numOfStr(var[1], "\"") == 2) {
		stringX::replace(var[1], "\"", "", NULL);
		stringX::replace(var[1], "\"", "", NULL);
		ifstr(var);
		return;
	}
	else if (stringX::numOfStr(var[1], "\"") == 1) {
		cerr << "illegal syntax! pos: " << data.find("\"");
		return;
	}
	else if (stringX::numOfStr(var[1], "\"") > 2) {
		for (int i = 0; i < stringX::numOfStr(var[1], "\""); i++) {
			if (var[1].substr(var[1].find("\"") - 1, var[1].find("\"")) == "\\") {
				continue;
			}
			else {
				cerr << "forgot \"\\\" before \"!";
				return;
			}
		}
	}
	else if (stringX::numOfStr(var[1], "'") == 2) {
		stringX::replace(var[1], "'", "", NULL);
		stringX::replace(var[1], "'", "", NULL);
		ifstr(var);
		return;
	}
	else if (stringX::numOfStr(var[1], "'") == 1) {
		cerr << "illegal syntax! pos: " << data.find("'");
		return;
	}
	else if (stringX::numOfStr(var[1], "'") > 2) {
		for (int i = 0; i < stringX::numOfStr(var[1], "'"); i++) {
			if (var[1].substr(var[1].find("'") - 1, var[1].find("'")) == "\\") {
				continue;
			}
			else {
				cerr << "forgot \"\\\" before '!";
				return;
			}
		}
	}
	else {
		els(var);
	}
}
void ifstrvar(vector<string> var) {
	strv[var[0]] = var[1];
}
void elsvar(vector<string> var) {
	if (var[1] == "true") {
		boolv[var[0]] = true;
		return;
	}
	else if (var[1] == "false") {
		boolv[var[0]] = false;
		return;
	}
	else if (stringX::isnum(var[1])) {
		long long conv = strtoll(var[1].c_str(), nullptr, 10);
		intv[var[0]] = conv;
		return;
	}
	else {
		cerr << "illegal number!";
		return;
	}
}
void ifstrtext(vector<string> var) {
	cout << var[1];
}
void elstext(vector<string> var) {
	map<string, string>::iterator str = strv.begin();
	map<string, int>::iterator i = intv.begin();
	map<string, bool>::iterator bo = boolv.begin();
	int inc = 0;
	while (str != strv.end()) {
		if (var[1] == str->first) {
			cout << str->second;
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
	cerr << "malformed number or bool!";
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
			cerr << "path is int!";
			return;
		}
	}
	while (bo != boolv.end()) {
		if (var[1] == bo->first) {
			cerr << "path is bool!";
			return;
		}
	}
	cerr << "malformed number or bool!";
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
			cerr << "path is int!";
			return;
		}
	}
	while (bo != boolv.end()) {
		if (var[1] == bo->first) {
			cerr << "path is bool!";
			return;
		}
	}
	cerr << "malformed number or bool!";
	return;
}
void compile(string input, string data) {
	//VARIABLES
	if (stringX::numOfStr(data, "=") > 0) {
		if (stringX::numOfStr(data, "=") == 1) {
			vector<string> var;
			stringX::splitString(data, var, "=");
			stringX::replace(var[1], " ", "", NULL);
			stringX::replace(var[0], " ", "", stringX::numOfStr(var[0], " ") - 1);
			precompile(var, data, ifstrvar, elsvar);
		}
	} //FUNCTIONS
	else if (data.substr(0, funcs[0].size()) == funcs[0]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			precompile(b, data, ifstrtext, elstext);
		}
		else {
			cerr << "function has been called invalidly";
			return;
		}
	}
	else if (data.substr(0, funcs[1].size()) == funcs[1]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			b.push_back(input);
			precompile(b, data, ifstrsave, elssave);
			return;
		}
		else {
			cerr << "function has been called invalidly";
			return;
		}
	}
	else if (data.substr(0, funcs[2].size()) == funcs[2]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			b.push_back(input);
			precompile(b, data, ifstrload, elsload);
			vector<string> ans;
			stringX::splitString(cst, ans, "\n");
			for (auto data : ans) {
				compile(cst, data);
			}
			return;
		}
		else {
			cerr << "function has been called invalidly";
			return;
		}
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
			compile(tinput,data);
		}
		cout << endl;
		line++;
	}
	/*
	if (stringX::numOfStr(input, "\n") > 0) {
	}*/
}
