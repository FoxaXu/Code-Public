// Copyright FoxaXu
// Create New Sign Tool
// PowerShell -New-SelfSignedCertificate
// Format  newsign %filesign%

#include<Windows.h>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<string>
#pragma comment(lib,"URlmon.lib")

#undef UNICODE
#undef _UNICODE

using namespace std;

bool existcheck(string& name) {
	ifstream f(name.c_str());
	return f.good();
}

LPCWSTR stringToLPCWSTR(string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = new wchar_t[sizeof(wchar_t) * (orig.length() - 1)];
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

string ReadText(string filename, int line)
{
	ifstream fin;
	fin.open(filename, ios::in);
	string strVec[11];     //文本中总共有10行
	int i = 0;
	while (!fin.eof())
	{
		string inbuf;
		getline(fin, inbuf, '\n');
		strVec[i] = inbuf;
		i = i + 1;
	}
	return strVec[line - 1];
}

int main(int argc, char* argv[]) {
	ofstream WritePoint;
	ifstream ReadPoint;

	WritePoint.open("C:\\Test.bin");
	WritePoint << "Test" << endl;
	WritePoint.close();

	string charcode = "C:\\Test.bin";
	bool retTestbin = existcheck(charcode);
	if (retTestbin) {
		remove(charcode.c_str());
		goto PassCheck;
	}
	else {
		cout << "Access is denied" << endl;
		cout << "Use Administrator try again" << endl;
		getchar();
		return 0;
	}

PassCheck:

	charcode = ".\\quotation.mark";
	bool retQUA = existcheck(charcode);
	if (retQUA) {}
	else {
		string durl = "https://api.foxaxu.com/res/char/quotation.mark";
		LPCWSTR LcURL = stringToLPCWSTR(durl);
		string dsp = ".\\quotation.mark";
		LPCWSTR LcSP = stringToLPCWSTR(dsp);

		HRESULT applydownload = URLDownloadToFileW(
			nullptr,
			LcURL,
		    LcSP,
			0,
			nullptr
		);

		bool retdownload = existcheck(dsp);
		if (retdownload) {}
		else
		{
			cout << "NewSign requires some auxiliary files, " << endl;
			cout << "Cannot Connect Server. Please check your Internet" << endl;
			getchar();
			return 0;
		}
	}

	string quo;
	ReadPoint.open(".\\quotation.mark");
	ReadPoint >> quo;
	ReadPoint.close();

	if (argc > 1) {
		cout << "New Sign Tool" << endl;
		cout << "Format " << endl;
		cout << "         File Sign:" << endl;
		cout << "                     NewSign filesign" << endl;
		cout << "      Message %cn% %o% %email% %firendlyname% %Outfile% %Password% %AddDays% " << endl;
		cout << "          Use PowerShell New-SelfSignedCertificate" << endl;
		return 0;
	}
	
	cout << "Create New Sign File Sign" << endl;
	cout << "More info please visit :   https://developer.foxaxu.com/newsign" << endl;
	cout << "Use PowerShell Create" << endl;
	cout << endl;
	
	string CN;
	cout << "Set CN Info  $";
	getline(cin, CN);
	cout << endl;

	string O;
	cout << "Set O Info  $";
	getline(cin, O);
	cout << endl;

	string Email;
	cout << "Set Email Info  $";
	getline(cin, Email);
	cout << endl;

	string Fname;
	cout << "Set FriendlyName Info  $";
	getline(cin, Fname);
	cout << endl;

	string outfile;
	cout << "Set outfile path  $";
	getline(cin, outfile);
	cout << endl;

	string Password;
	cout << "Set Cert Password  $";
	getline(cin, Password);
	cout << endl;

	string AddDays;
	cout << "Set Cert Vaild Time. Unit: Day   $";
	getline(cin, AddDays);
	cout << endl;

	cout << endl;
	cout << "Create Your Cert :   CN: " << CN << "  O: " << O << "  Email: " << Email << "  FriendlyName: " << Fname << "  outfile Path : " << outfile << "  AddDays: " << AddDays << "  Password: " << Password << endl;
	cout << "Please Wait" << endl;

	WritePoint.open("C:\\$CreateScript.ps1");

	WritePoint << "New-SelfSignedCertificate -Type Custom -Subject " + quo + "CN=" + CN + ", O=" + O + ", Email=" + Email  + quo + " -KeyUsage DigitalSignature -FriendlyName " + quo + Fname + quo + " -CertStoreLocation " + quo + "Cert:\\CurrentUser\\My" + quo + " -TextExtension @(" + quo + "2.5.29.37={text}1.3.6.1.5.5.7.3.3" + quo + ", " + quo + "2.5.29.19={text}" + quo + ") -NotAfter (Get-Date).AddDays(" + AddDays + ")" << endl;

	WritePoint.close();

	WritePoint.open("C:\\$SetDefault.bat");

	WritePoint << "PowerShell Set-ExecutionPolicy Unrestricted" << endl;
	WritePoint << "PowerShell C:\\$CreateScript.ps1 >>$PS~out.txt" << endl;
	WritePoint << "del C:\\$SetDefault.bat" << endl;

	WritePoint.close();

	ShellExecute(0, L"runas", L"C:\\$SetDefault.bat", 0, 0, SW_HIDE);
	string DELSD = "C:\\$SetDefault.bat";
WaitProcess:
	bool retDELSD = existcheck(DELSD);
	if (retDELSD) {
		Sleep(500);
		goto WaitProcess;
	}

	string ThumbprintTemp;
	ThumbprintTemp = ReadText("$PS~out.txt",7);
	remove("$PS~out.txt");

	WritePoint.open("CleanCode.tmp");
	WritePoint << ThumbprintTemp;
	WritePoint.close();

	string Thumbprint;

	ReadPoint.open("CleanCode.tmp");
	ReadPoint >> Thumbprint;
	ReadPoint.close();
	remove("CleanCode.tmp");

	WritePoint.open("C:\\$OutCertFile.ps1");

	WritePoint << "$password = ConvertTo-SecureString -String " + Password + " -Force -AsPlainText" << endl;
	WritePoint << "Export-PfxCertificate -cert " + quo + "Cert:\\CurrentUser\\My\\" + Thumbprint + quo + " -FilePath " + outfile + " -Password $password" << endl;
	WritePoint.close();

	WritePoint.open("C:\\$OutCertScript.bat");
	WritePoint << "PowerShell C:\\$OutCertFile.ps1" << endl;
	WritePoint << "del C:\\$OutCertScript.bat" << endl;
	WritePoint.close();

	ShellExecute(0, L"runas", L"C:\\$OutCertScript.bat", 0, 0, SW_HIDE);
	string OCSFile = "C:\\$OutCertScript.bat";
WaitReturnOCS:	
	bool retWROCS = existcheck(OCSFile);
	if (retWROCS) {
		Sleep(500);
		goto WaitReturnOCS;
	}
	Sleep(1000);

	// 验证证书是否正常导出
	bool retCertOut = existcheck(outfile);
	remove("C:\\$OutCertFile.ps1");
	remove("C:\\$CreateScript.ps1");
	
	if (retCertOut) {
		cout << endl;
		cout << "Cert Create Successfully" << endl;
		cout << "Cert is save in:  " << outfile << endl;
		cout << "Press any key to Exit" << endl;
		getchar();
		return 0;
	}
	else
	{
		cout << endl;
		cout << "Cert Out Failed" << endl;
		cout << "Check your write or try to use quotation mark" << endl;
		cout << "Press any key to Exit" << endl;
		getchar();
		return 0;
	}
}