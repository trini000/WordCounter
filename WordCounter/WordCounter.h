#ifndef WORDCOUNTER_H_ 
#define WORDCOUNTER_H_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <windows.h>



class WordCounter
{
public:
	WordCounter();
	int AnalyzeTexts(std::wstring strPath);
	void WriteReport(std::wstring directory);
	bool CheckFileExt(WCHAR *filename);
	
	std::string m_ReportName;

private:
	std::map<std::string, int> m_theMap;
};


#endif	//WORDCOUNTER_H_