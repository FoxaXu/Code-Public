// Copyright FoxaXu 2023 (C)
// Download File Tool

#include<Windows.h>
#include<stdio.h>
#include<direct.h>
#include<string>
#include<iostream>
#include<fstream>
#include<unistd.h>
#include<urlmon.h>
#pragma comment(lib,"URlmon.lib")

using namespace std;

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


int main(int argc,char *argv[])
{
	if (argc == 1) {
		MessageBox(0, L"参数无效，无法执行任务，格式 FXDS.exe %下载链接% %保存目录%", L"FX Download Service",MB_OK);
		return 0;
	}
	if (argc == 2) {
		MessageBox(0, L"缺少保存目录参数，格式 FXDS.exe %下载链接% %保存目录%", L"FX Download Service", MB_OK);
		return 0;
	}
	int ia = 0;
	ia < argc;
	ia++;

	string durl = argv[ia];
	ia++;
	string dsp = argv[ia];

	StartDownload:
	LPCWSTR LcDsp = stringToLPCWSTR(dsp);
	LPCWSTR LcDURL = stringToLPCWSTR(durl);

	cout << "下载开始  " << "  - 下载链接:" << durl << "   保存地址 : " << dsp << endl;

	HRESULT applydownload = URLDownloadToFileW(
		nullptr,
		LcDURL,
		LcDsp,
		0,
		nullptr
	);

	bool CDSP = isFileExists_ifstream(dsp);
	if (CDSP) {
		cout << "下载成功，目录  " << dsp << endl;

		system("echo=TRUE >>%temp%\\DownloadReport.dat");

		return 0;
	}
	else
	{
		cout << "下载失败, URL错误: " << durl << endl;

		system("echo=FALSE >>%temp%\\DownloadReport.dat");

		return 0;
	}
}