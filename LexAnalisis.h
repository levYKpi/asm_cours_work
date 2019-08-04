#ifndef __LEXANALISIS_
#define __LEXANALISIS_
#include "Reserved.h"

class LexAnalisis{
public:
	LexAnalisis();
	LexAnalisis(const char*);
	~LexAnalisis();
	void ReadFile(const char*);
	void PrintFile();
	void PrintFileUpper();
	void PrintLexTable();
	std::deque<std::string>* GetFileDeque();
	std::deque<std::string>* GetFileDequeUpper();
	LexTable* GetLexTable();
	std::deque<int>* GetErrorTable();
	unsigned int GetNumOfLines();
	void FileToUpper();
	void CreateLexTable();
private:
	bool LexTableString(int);
	void LexTablePushLex(int,LexTableExample *);
	void LexTableUnKnown(int,LexTableExample *);
	void PrintStringLex(int,std::ofstream *);
	bool FindDec(char);
	bool FindBin(char);
	bool FindHex(char);
	std::deque<std::string> file_deque;
	std::deque<std::string> file_deque_upper;
	LexTable *lex_table;
	std::deque<int> Error_table;
	unsigned int lines_numb;
};

#endif