#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <conio.h>
#include <map>
#define ANY_ARROW 224
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
//start at pre
using namespace std;
vector<string> funcs =
{
	"console.addText"
};
map<string, string> strv;
map<string, int> intv;
map<string, bool> boolv;
void ok(string input,string pre) {
	COORD c;
	c.X = 0;
	c.Y = 0;

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
	std::string type_t(string pre) {
		cout << pre;
		string input;
		int rowPos = 0;
		int linePos = 0;
		//13 enter?
		for (int i = 0;;) {
			i = _getch();
			if (i == 8) {
				if (rowPos > 0) {
					cout << endl << rowPos << endl;
					if (rowPos == input.size()) {
						//cout << "\b \b";
						input.pop_back();
						cout << "\033[H \033[2J \r";
						cout << pre << input;
						if (input.size() > 0 && input.substr(input.size() - 1) == "\n") {
							linePos = linePos - 1;
						}
						rowPos = rowPos - 1;
					}
					else {
						input = input.substr(0, rowPos - 1) + input.substr(rowPos);
						cout << "\033[H \033[2J \r";
						cout << pre << input;
						for (int i = 0; i < input.size() - rowPos + 1; i++) {
							cout << "\b";
						}
						if (input.size() > 0 && input.substr(input.size() - 1) == "\n") {
							linePos = linePos - 1;
						}
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
							linePos = linePos + 1;
							rowPos = rowPos + 1;
						}
						else {
							input = input.substr(0, rowPos - 1) + "\n" + input.substr(rowPos - 1);
							cout << "\033[H \033[2J \r";
							cout << pre << input;
							for (int i = 0; i < input.size() - rowPos + 1; i++) {
								cout << "\b";
							}
							linePos = linePos + 1;
							rowPos = rowPos + 1;
						}
					}
				}
				else {
					if (rowPos == input.size()) {
						input = input + (char)i;
						ok(input, pre);
						rowPos = rowPos + 1;
					}
					else {
						input = input.substr(0, rowPos - 1) + (char)i + input.substr(rowPos - 1);
						ok(input, pre);
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
						ok(input,pre);
						rowPos = rowPos + 1;
					}
					else {
						input = input.substr(0, rowPos - 1) + (char)i + input.substr(rowPos - 1);
						ok(input,pre);
						for (int i = 0; i < input.size() - rowPos; i++) {
							cout << "\b";
						}
						rowPos = rowPos + 1;
					}
				}
			}
		}
		return input;
	}
}
void compile(string data) {
	//VARIABLES
	if (stringX::numOfStr(data, "=") > 0) {
		if (stringX::numOfStr(data, "=") == 1) {
			vector<string> var;
			stringX::splitString(data, var, "=");
			stringX::replace(var[1], " ", "", NULL);
			stringX::replace(var[0], " ", "", stringX::numOfStr(var[0], " ") - 1);
			if (stringX::numOfStr(var[1], "\"") == 2 && stringX::numOfStr(var[1], "'") < 2) {
				stringX::replace(var[1], "\"", "", NULL);
				stringX::replace(var[1], "\"", "", NULL);
				strv[var[0]] = var[1];
			}
			else if (stringX::numOfStr(data, "\"") == 1 && stringX::numOfStr(var[1], "'") < 2) {
				cout << "illegal syntax! pos: " << data.find("\"");
			}
			else if (stringX::numOfStr(data, "\"") > 2 && stringX::numOfStr(var[1], "'") < 2) {
				for (int i = 0; i < stringX::numOfStr(var[1], "\""); i++) {
					if (var[1].substr(var[1].find("\"") - 1, var[1].find("\"")) == "\\") {
						continue;
					}
					else {
						cout << "forgot \"\\\" before \"!";
					}
				}
			}
			else if (stringX::numOfStr(var[1], "'") == 2) {
				stringX::replace(var[1], "'", "", NULL);
				stringX::replace(var[1], "'", "", NULL);
				strv[var[0]] = var[1];
			}
			else if (stringX::numOfStr(data, "'") == 1) {
				cout << "illegal syntax! pos: " << data.find("'");
			}
			else if (stringX::numOfStr(data, "'") > 2) {
				for (int i = 0; i < stringX::numOfStr(data, "'"); i++) {
					if (data.substr(data.find("'") - 1, data.find("'")) == "\\") {
						continue;
					}
					else {
						cout << "forgot \"\\\" before '!";
						break;
					}
				}
			}
			else {
				if (stringX::isnum(var[1])) {
					long long conv = strtoll(var[1].c_str(), nullptr, 10);
					intv[var[0]] = conv;
				}
				else {
					cout << "illegal number!";
				}
				if (var[1] == "true") {
					boolv[var[0]] = true;
				}
				else if (var[1] == "false") {
					boolv[var[0]] = false;
				}
			}
		}
	}
}
int main()
{
	string input = stringX::type_t(">> ");
	cout << endl;
	vector<string> ans;
	stringX::splitString(input, ans, "\n");
	for (auto data : ans) {
		compile(data);
	}
	/*
	if (stringX::numOfStr(input, "\n") > 0) {
	}*/
}
