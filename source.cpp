//still adding if statement
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
template<typename T, typename...a> using function = T(*)(a...);
int line = 0;
using namespace std;
vector<string> funcs =
{
	"console.text",
	"save",
	"load",
	"if"
};
map<string, string> strv;
map<string, int> intv;
map<string, bool> boolv;
string it;
string dt;
string cst;
bool strfind = false;
void setxy(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void ok(string input, string pre, int line) {
	setxy(0, line);
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
void gsbas(string var, string midStr, string & after, string & before) { //get strings before and after main string
	after = var.substr(var.find(midStr) + midStr.size());
	before = var.substr(0, var.find(midStr));
}
void gsbas(string var, int substr1, int substr2, string& after, string& before) { //get strings before and after main string
	after = var.substr(substr2);
	before = var.substr(0, substr1);
}
int preformat(string & var, string quote) {
	if (var.find(quote) != string::npos) {
		string test = var.substr(var.find(quote), var.find_last_of(quote) - var.find(quote) + 1);
		string test1;
		string test2;
		gsbas(var, test, test1, test2);
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
void rs(vector<string>& var) {
	if (var[0] != "" && var[1] != "") {
		var[0] = var[0].substr(var[0].find_first_not_of("	"), var[0].find_last_not_of("	") + 1);
		var[1] = var[1].substr(var[1].find_first_not_of("	"), var[1].find_last_not_of("	") + 1);
		var[0] = var[0].substr(var[0].find_first_not_of(" "), var[0].find_last_not_of(" ") + 1);
		var[1] = var[1].substr(var[1].find_first_not_of(" "), var[1].find_last_not_of(" ") + 1);
	}
}
void rs(string & var) {
	if (var != "") {
		var = var.substr(var.find_first_not_of("	"), var.find_last_not_of("	") + 1);
		var = var.substr(var.find_first_not_of(" "), var.find_last_not_of(" ") + 1);
	}
}
int format(vector<string>& var, int quotes) { // second param is SINGLE or DOUBLE
	rs(var);
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
int format(string var, int quotes) { // second param is SINGLE or DOUBLE
	rs(var);
	if (quotes == SINGLE) {
		int h = preformat(var, "'");
		if (h == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (quotes == DOUBLE) {
		int h = preformat(var, "\"");
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
// BOOLEAN COMPARISON FUNCS
// check if quote has \ and check if there is more after == cut
// for loop each character if quote then continue, if \, check if next char is quote or not, 
// if quote, skip two/one chars and continue until found quote without \ before quote
vector<int> stoq(string var) { // str to qpos (QUOTE POS)
	vector<int> qpos;
	bool dcw = false; //double QUOTE wait
	bool scw = false; //single QUOTE wait
	for (int i = 0; i < var.size() - 1; i++) {
		if (var[i] == '\\') {
			i++;
			continue;
		}
		else {
			if (var[i] == '\'') {
				if (dcw == false) {
					qpos.push_back(i);
					if (scw == true) {
						scw = false;
					}
					else if (scw == false) {
						scw = true;
					}
				}
			}
			if (var[i] == '\"') {
				if (scw == false) {
					qpos.push_back(i);
					if (dcw == true) {
						dcw = false;
					}
					else if (dcw == false) {
						dcw = true;
					}
				}
			}
		}
	}
	return qpos;
}
vector<string> qtos(string str, vector<int> qpos) { //qpos to str
	vector<string> cvqpstr; //converted qpos to str
	for (int i = 0; i < qpos.size(); i++) {
		if (i == qpos.size() - 1) {
			cvqpstr.push_back(str.substr(qpos[i]));
		}
		else {
			cvqpstr.push_back(str.substr(qpos[i], qpos[i + 1] - qpos[i] + 1));
			i++;
		}
	}
	return cvqpstr;
}
string rsfbc(string var) { // remove bool comparisons with quotes from boolean comparison
	vector<int> qpos = stoq(var);
	vector<string> s = qtos(var, qpos);
	string tvar = var;
	for (int i = 0; i < s.size(); i++) {
		if (tvar.find(s[i]) != string::npos) {
			tvar = tvar.substr(0, tvar.find(s[i])) + tvar.substr(tvar.find(s[i]) + s[i].size());
		}
	}
	return tvar;
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
		int rowPos = 0;
		for (int i = 0;;) {
			i = _getch();
			if (i == 8) {
				if (input.size() > 0 && rowPos > 0) {
					if (rowPos == input.size()) {
						input.pop_back();
						ok(input, pre, line);
						cout << " \b";
						rowPos = rowPos - 1;
					}
					else {
						input = input.substr(0, rowPos - 1) + input.substr(rowPos);
						ok(input, pre, line);
						cout << " \b\b";
						for (int i = 0; i < input.size() - rowPos; i++) {
							cout << "\b";
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
		CONSOLE_SCREEN_BUFFER_INFO csbi; 
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		for (int i = 0; i <= numOfStr(input, "\n"); i++) {
			line++;
		}
		line = csbi.dwCursorPosition.Y + 1;
		return input;
	}
}
int str(vector<string> & var, string input, string data) {
	if (stringX::numOfStr(var[1], "\"") == 2) {
		rs(var);
		if (var[1][0] == '\'' && var[1][var[1].size() - 1] == '\'') {
			stringX::replace_all(var[1], "\\n", "\n");
			return SINGLE;
		}
		stringX::replace_all(var[1], "\\n", "\n");
		return DOUBLE;
	}
	else if (stringX::numOfStr(var[1], "\"") == 1) {
		error("forgot \" !", input, data, "\"", false);
		return -1;
	}
	else if (stringX::numOfStr(var[1], "\"") > 2) {
		int find = var[1].find("\"");
		find = var[1].find("\"", find + 1);
		for (int i = 0; i < stringX::numOfStr(var[1], "\"") - 2; i++) {
			if (var[1][find - 1] == '\\') {
				if (strfind == true) {
					var[1] = var[1].substr(0, find - 1) + var[1].substr(find);
				}
				find = var[1].find("\"", find + 1);
				continue;
			}
			else {
				error("forgot \"\\\" before \"!", input, data, "\"", true);
				return -1;
			}
		}
		return DOUBLE;
	}
	else if (stringX::numOfStr(var[1], "'") == 2) {
		rs(var);
		if (var[1][0] == '"' && var[1][var[1].size() - 1] == '"') {
			stringX::replace_all(var[1], "\\n", "\n");
			return DOUBLE;
		}
		stringX::replace_all(var[1], "\\n", "\n");
		return SINGLE;
	}
	else if (stringX::numOfStr(var[1], "'") == 1) {
		error("forgot \"'\" !", input, data, "'", false);
		return -1;
	}
	else if (stringX::numOfStr(var[1], "'") > 2) {
		int find = var[1].find("'");
		find = var[1].find("'", find + 1);
		for (int i = 0; i < stringX::numOfStr(var[1], "'") - 2; i++) {
			if (var[1][find - 1] == '\\') {
				if (strfind == true) {
					var[1] = var[1].substr(0, find - 1) + var[1].substr(find);
				}
				find = var[1].find("'", find + 1);
				continue;
			}
			else {
				error("forgot \"\\\" before '!", input, data, "'", true);
				return -1;
			}
		}
		return SINGLE;
	}
	else {
		return NULL;
	}
}
int str(string var, string input, string data) {
	if (stringX::numOfStr(var, "\"") == 2) {
		rs(var);
		if (var[0] == '\'' && var[var.size() - 1] == '\'') {
			stringX::replace_all(var, "\\n", "\n");
			return SINGLE;
		}
		stringX::replace_all(var, "\\n", "\n");
		return DOUBLE;
	}
	else if (stringX::numOfStr(var, "\"") == 1) {
		error("forgot \" !", input, data, "\"", false);
		return -1;
	}
	else if (stringX::numOfStr(var, "\"") > 2) {
		int find = var.find("\"");
		find = var.find("\"", find + 1);
		for (int i = 0; i < stringX::numOfStr(var, "\"") - 2; i++) {
			if (var[find - 1] == '\\') {
				if (strfind == true) {
					var = var.substr(0, find - 1) + var.substr(find);
				}
				find = var.find("\"", find + 1);
				continue;
			}
			else {
				error("forgot \"\\\" before \"!", input, data, "\"", true);
				return -1;
			}
		}
		return DOUBLE;
	}
	else if (stringX::numOfStr(var, "'") == 2) {
		rs(var);
		if (var[0] == '"' && var[var.size() - 1] == '"') {
			stringX::replace_all(var, "\\n", "\n");
			return DOUBLE;
		}
		stringX::replace_all(var, "\\n", "\n");
		return SINGLE;
	}
	else if (stringX::numOfStr(var, "'") == 1) {
		error("forgot \"'\" !", input, data, "'", false);
		return -1;
	}
	else if (stringX::numOfStr(var, "'") > 2) {
		int find = var.find("'");
		find = var.find("'", find + 1);
		for (int i = 0; i < stringX::numOfStr(var, "'") - 2; i++) {
			if (var[find - 1] == '\\') {
				if (strfind == true) {
					var = var.substr(0, find - 1) + var.substr(find);
				}
				find = var.find("'", find + 1);
				continue;
			}
			else {
				error("forgot \"\\\" before '!", input, data, "'", true);
				return -1;
			}
		}
		return SINGLE;
	}
	else {
		return NULL;
	}
}
void exfunc1(string & eq) {
	map<string, string>::iterator strr = strv.begin();
	map<string, int>::iterator ii = intv.begin();
	map<string, bool>::iterator boo = boolv.begin();
	while (strr != strv.end()) {
		if (eq == strr->first) {
			eq = '"' + strr->second + '"';
			return;
		}
		++strr;
	}
	while (ii != intv.end()) {
		if (eq == ii->first) {
			eq = to_string(ii->second);
			return;
		}
		++ii;
	}
	while (boo != boolv.end()) {
		if (eq == boo->first) {
			if (boo->second == true) {
				eq = to_string(boo->second);
				return;
			}
			else if (boo->second == false) {
				eq = to_string(boo->second);
				return;
			}
		}
		++boo;
	}
}
bool boolcomp(string input, string data, vector<string>& var) {
	string b4eq;
	string afeq;
	strfind = false;
	bool int1 = false; // something before bcmp
	bool int2 = false; // something after bcmp
	vector<int> qpos = stoq(var[1]);
	string bcmp = rsfbc(var[1]);
	vector<string> strcomp = qtos(var[1], qpos);
	if (stringX::numOfStr(bcmp, "==") == 1) {
		gsbas(bcmp, "==", afeq, b4eq);
		if (b4eq.find_first_not_of(" ") != string::npos) {
			if (int1 == false) {
				int1 = true;
			}
		}
		if (afeq.find_first_not_of(" ") != string::npos) {
			if (int2 == false) {
				int2 = true;
			}
		}
		if (int1 == true && int2 == true) {
			//what to do if both comparisons are bool/int
			vector<string> both = { afeq,b4eq };
			rs(both);
			afeq = both[0];
			b4eq = both[1];
			exfunc1(afeq);
			exfunc1(b4eq);
			string combine = afeq + " == " + b4eq;
			bcmp = rsfbc(combine);
			gsbas(bcmp, "==", afeq, b4eq);
			both = { afeq,b4eq };
			rs(both);
			afeq = both[0];
			b4eq = both[1];
			int strcheck = str(afeq, input, data);
			if (strcheck == 0) {
				if (stringX::isnum(afeq)) {
					long long conv = strtoll(var[1].c_str(), nullptr, 10);
					if (conv > INT64_MAX) {
						error("number is too big! boolean comparison could not be run.", it, dt, var[1], false);
						line++;
					}
				}
				else {
					error("malformed number or bool!", it, dt, var[1], false);
					return false;
				}
			}
			strcheck = str(b4eq, input, data);
			if (strcheck == 0) {
				if (stringX::isnum(b4eq)) {
					long long conv = strtoll(var[1].c_str(), nullptr, 10);
					if (conv > INT64_MAX) {
						error("number is too big! boolean comparison could not be run.", it, dt, var[1], false);
						line++;
					}
				}
				else {
					error("malformed number or bool!", it, dt, var[1], false);
					return false;
				}
			}
			if (afeq == "true") {
				afeq = "1";
			}
			else if (afeq == "false") {
				afeq = "0";
			}
			if (b4eq == "true") {
				b4eq = "1";
			}
			else if (b4eq == "false") {
				b4eq = "0";
			}

			if (afeq == b4eq) {
				var[1] = "true";
			}
			else {
				var[1] = "false";
			}
		}
		else if (int1 == false && int2 == false) {
			gsbas(var[1], var[1].substr(qpos[1] + 1).find("==") + qpos[1] + 1, var[1].substr(qpos[1] + 1).find("==") + qpos[1] + 4, afeq, b4eq);
			int prestr = str(afeq, input, data);
			int prestr1 = str(b4eq, input, data);
			if (prestr == -1 || prestr == 0 || prestr1 == -1 || prestr1 == 0) {
				return false;
			}
			int strcheck = format(afeq, prestr);
			int strcheck1 = format(b4eq, prestr1);
			if (strcheck == 0 && strcheck1 == 0) {
				if (strcomp.size() == 2) {
					rc(strcomp[0]);
					rc(strcomp[1]);
					if (strcomp[0] == strcomp[1]) {
						var[1] = "true";
					}
					else {
						var[1] = "false";
					}
				}
				else if (strcomp.size() > 2) {
					error("critical error has occured while trying to process boolean comparison!", input, data, var[1], false);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			if (qpos.size() > 0) {
				error("cannot compare string by bool/int!", input, data, afeq, false);
				return false;
			}
			else {
				error("malformed number or bool!", input, data, "(", false);
				return false;
			}
		}
	}
	else if (stringX::numOfStr(bcmp, "==") > 1) {
		error("boolean comparison can only have one pair of equal symbols!", input, bcmp, "==", false);
		return false;
	}
	return true;
}
void precompile(vector<string> var, string input, string data, function<void, vector<string>> ifstr, function<void, vector<string>> els) {
	it = input;
	dt = data;
	strfind = true;
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
	//rc(var[1]);
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
	error("malformed number or bool!", it, dt, var[1], false);
	return;
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
		cout << var[1];
		return;
	}
	else if (var[1] == "false") {
		cout << var[1];
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
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			line = csbi.dwCursorPosition.Y;
			for (int i = 0; i < stringX::numOfStr(var[1], "\n"); i++) {
				line++;
			}
			return;
		}
		++str;
	}

	while (i != intv.end()) {
		if (var[1] == i->first) {
			cout << i->second;
			return;
		}
		++i;
	}

	while (bo != boolv.end()) {
		if (var[1] == bo->first) {
			if (bo->second == true) {
				cout << "true";
			}
			else if (bo->second == false) {
				cout << "false";
			}
			return;
		}
		++bo;
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
void compile(string input, string data, int & i) {
	//FUNCTIONS
	if (data.substr(0, funcs[0].size()) == funcs[0]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			if (b[1].find_first_not_of("") == string::npos || b[1].find_first_not_of(" ") == string::npos || b[1].find_first_not_of("	") == string::npos) {
				cout << "";
				return;
			}
			bool bc = boolcomp(input, data, b);
			if (bc == false) {
				return;
			}
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
			if (b[1].find_first_not_of("") == string::npos || b[1].find_first_not_of(" ") == string::npos || b[1].find_first_not_of("	") == string::npos) {
				error("function parameter 1 is empty!", input, data, "=", false);
				return;
			}
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
			if (b[1].find_first_not_of("") == string::npos || b[1].find_first_not_of(" ") == string::npos || b[1].find_first_not_of("	") == string::npos) {
				error("function parameter 1 is empty!", input, data, "=", false);
				return;
			}
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
				compile(cst, data, i);
			}
			return;
		}
		else {
			error("function has been called inavlidly", input, data, "(", false);
			return;
		}
	}
	else if (data.substr(0, funcs[3].size()) == funcs[3]) {
		if (stringX::numOfStr(data, "(") == 1 && stringX::numOfStr(data, ")") == 1) {
			vector<string> b;
			stringX::splitString(data, b, "(");
			stringX::replace(b[1], ")", "", NULL);
			if (b[1].find_first_not_of("") == string::npos || b[1].find_first_not_of(" ") == string::npos || b[1].find_first_not_of("	") == string::npos) {
				error("if statement comparison is empty!", input, data, "=", false);
				return;
			}
			bool bc = boolcomp(input, data, b);
			if (bc == false) {
				return;
			}
			if (bc == true) {
				if (b[1] == "true") {
					int prei = i;
					bool ifcomplete = false;
					vector<string> vs;
					stringX::splitString(input, vs, "\n");
					if (vs.size() > 1) {
						while (prei < vs.size()) {
							++prei;
							rs(vs[prei]);
							if (vs[prei] == "end") {
								ifcomplete = true;
								break;
							}
						}
						if (ifcomplete == false) {
							error("if statement was not closed!", input, data, data, false);
						}
						while (i < vs.size()) {
							++i;
							if (vs[i] == "end") {
								break;
							}
							compile(input, vs[i], i);
						}
					}
					else {
						error("if statement was not closed!", input, data, data, false);
					}
				}
				else if (b[1] == "false") {
					int prei = i;
					bool ifcomplete = false;
					vector<string> vs;
					stringX::splitString(input, vs, "\n");
					if (vs.size() > 1) {
						while (prei < vs.size()) {
							++prei;
							rs(vs[prei]);
							if (vs[prei] == "end") {
								ifcomplete = true;
								break;
							}
						}
						if (ifcomplete == false) {
							error("if statement was not closed!", input, data, data, false);
						}
						while (i < vs.size()) {
							++i;
							if (vs[i] == "end") {
								break;
							}
						}
					}
					else {
						error("if statement was not closed!", input, data, data, false);
					}
				}
			}
		}
		else {
			error("function has been called inavlidly", input, data, "(", false);
			return;
		}
	}
	//VARIABLES
	else if (stringX::numOfStr(data, "=") > 0) {
		vector<string> var;
		var.push_back(data.substr(0, data.find("=")));
		var.push_back(data.substr(data.find("=") + 1));
		//-----------
		if ((var[0].find_first_not_of("") == string::npos || var[0].find_first_not_of(" ") == string::npos) && (var[1].find_first_not_of("") == string::npos || var[1].find_first_not_of(" ") == string::npos)) {
			error("missing variable name and value!", input, data, "", false);
			return;
		}
		if (var[0].find_first_not_of("") == string::npos || var[0].find_first_not_of(" ") == string::npos) {
			error("variable name is empty!", input, data, var[0], false);
			return;
		}
		if (var[1].find_first_not_of("") == string::npos || var[1].find_first_not_of(" ") == string::npos) {
			error("variable value is empty!", input, data, "=", false);
			return;
		}
		//-----------
		rs(var);
		bool bc = boolcomp(input, data, var);
		if (bc == false) {
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
		for (int i = 0; i < ans.size(); i++) {
			string data = ans[i];
			rs(data);
			compile(tinput, data, i);
		}
		cout << endl;
		line++;
	}
}
