// WordCounter.cpp
//

#include "stdafx.h"
#include "WordCounter.h"
#include <tchar.h>
//#pragma comment(lib, "User32.lib")

using namespace std;

WordCounter::WordCounter()
{
	 m_ReportName = "report.csv";
}

int WordCounter::AnalyzeTexts(wstring filePath)
{
	fstream textFile(filePath.c_str(), ios::in);
	if (!textFile)
		return -1;

	// Read text file and save word appearance in a map
	char word[100];
	memset(word, 0, 100);
	int i = 0;
	while (!textFile.eof())
	{
		word[i++] = textFile.get();
		if ((word[i-1] <'A' || word[i-1] > 'Z') && (word[i-1] < 'a' ||word[i-1] > 'z'))
		{
			if (i > 2)	// skip empty stuff and letters
			{
				word[i-1] = '\0';
				string str = word;
				m_theMap[str]++;
			}
			i = 0;
		}
		else if (word[i-1] >= 'A' && word[i-1] <= 'Z')
		{
			word[i-1] = word[i-1] - ('A' - 'a');
		}
	}

	return 0;
}

bool WordCounter::CheckFileExt(WCHAR *filename)
{
	wchar_t *txt = L".txt";
	wchar_t *pdf = L".pdf";
	wchar_t *doc = L".doc";
	wchar_t *docx = L".docx";
	wchar_t *log = L".log";
	wchar_t *note = L".note";
	if(wcsstr(filename, txt) || wcsstr(filename, pdf) || wcsstr(filename, doc) || wcsstr(filename, docx) || wcsstr(filename, log) || wcsstr(filename, note))
		return true;
	
	return false;
}

void WordCounter::WriteReport(wstring directory)
{
	// Write Output
	std::wstring fileFullPath(directory);
	string pathsap = "\\";
	std::wstring wtemp = std::wstring(pathsap.begin(), pathsap.end());
	LPCWSTR wpathsap = wtemp.c_str();
	fileFullPath += wstring(wpathsap);
	fileFullPath += wstring(this->m_ReportName.begin(), this->m_ReportName.end());
	
	fstream report(fileFullPath.c_str(), ios::out|ios::trunc|ios::binary);

	for(map<string, int>::iterator itr = m_theMap.begin(); itr != m_theMap.end(); itr++)
	{
		report.write(itr->first.c_str(), itr->first.length());
		report.write(",", 1);
		char strCnt[10];
		memset(strCnt, 0, 0);
		itoa(itr->second, strCnt, 10);
		report.write(strCnt, strlen(strCnt));
		report.write("\n", 1);
	}

	report.close();
	cout << "Report is generated and saved: " << this->m_ReportName << endl;
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
//std::wstring stemp = s2ws(myString);
//LPCWSTR result = stemp.c_str();
}



