// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WordCounter.h"
#include <tchar.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Please specify a path. WordCounter will analyze all text files in there. (txt, pdf, doc, docx, log, note)" << endl;
	cout << "Note: input 'c' or 'current' to analyze files in current directory" << endl;
	string filePath = "";
	cin >> filePath;

	std::wstring stemp;
	if ("current" == filePath || "c" == filePath)
	{
		TCHAR pwd[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,pwd);
		stemp = pwd;
	}
	else
	{
		stemp = std::wstring(filePath.begin(), filePath.end());
	}
	LPCWSTR lpcwPath = stemp.c_str();

	WordCounter *wc = new WordCounter();
	
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
   
	// Check that the input path plus 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
	StringCchLength(lpcwPath, MAX_PATH, &length_of_arg);
	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return -1;
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), lpcwPath);

	// Prepare string for use with FindFile functions.  First, copy the string to a buffer, then append '\*' to the directory name.
	StringCchCopy(szDir, MAX_PATH, lpcwPath);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) 
	{
		return dwError;
	} 
   
	int rc = -1;
	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			std::wstring nametemp = std::wstring(wc->m_ReportName.begin(), wc->m_ReportName.end());
			if (wcscmp(nametemp.c_str(), ffd.cFileName) == 0)
			{
				_tprintf(TEXT("Skipping file: %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				continue;
			}
			else
			{
				if (!wc->CheckFileExt(ffd.cFileName))
				{
					_tprintf(TEXT("Skipping file: %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
					continue;
				}
			}
			
			_tprintf(TEXT("Processing file: %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);

			std::wstring fileFullPath(lpcwPath);
			string pathsap = "\\";
			std::wstring wtemp = std::wstring(pathsap.begin(), pathsap.end());
			LPCWSTR wpathsap = wtemp.c_str();
			fileFullPath += wstring(wpathsap);
			fileFullPath += std::wstring(ffd.cFileName);
			rc = wc->AnalyzeTexts(fileFullPath);
			if (-1 == rc)
				cout << "Cannot open file:" << ffd.cFileName << endl;
			else
				rc = 0;
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
 
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		printf("error: ERROR_NO_MORE_FILES \n");
	}

	FindClose(hFind);
/*	if (0 != dwError)
		cout << "Last error is: " << dwError << endl;
*/
	// Write analyze result into a csv report
	if (0 == rc)
		wc->WriteReport(lpcwPath);
	
	system("PAUSE");
   	return 0;
}