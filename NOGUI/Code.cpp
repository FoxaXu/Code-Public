#include<Windows.h>
#include<stdio.h>
#include<direct.h>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

LPCWSTR stringToLPCWSTR(string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[sizeof(wchar_t) * (orig.length() - 1)];
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

bool isFileExists_ifstream(string& name) {
	ifstream f(name.c_str());
	return f.good();
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		MessageBox(0, "请在文件后添加参数以运行", "NoGUI", MB_OK);
		return 0;
	}
	if (argc > 3) {
		MessageBox(0, "附加参数超出支持范围，目前支持范围仅为2", "NoGUI", MB_OK);
		return 0;
	}
	int ia = 0;
	ia < argc;
	ia++;
	string RunMain = argv[ia];

	if (argc == 2) {
		bool retRM = isFileExists_ifstream(RunMain);
		if (retRM) {}
		else
		{
			MessageBox(0, "程序不存在，无法启动", "NOGUI", MB_OK);
			return 0;
		}

		LPCSTR RunMainLST = RunMain.c_str();

		ShellExecute(0, "open", RunMainLST, 0, 0, SW_HIDE);
		return 0;
	}

	ia++;
	string RMAdd = argv[ia];
	bool retRM = isFileExists_ifstream(RunMain);
	if (retRM) {}
	else
	{
		MessageBox(0, "程序不存在，无法启动", "NOGUI", MB_OK);
		return 0;
	}

	LPCSTR RunMainLST = RunMain.c_str();
	LPCSTR RMAddLST = RMAdd.c_str();

	ShellExecute(0, "open", RunMainLST, RMAddLST, 0, SW_HIDE);
	return 0;
}