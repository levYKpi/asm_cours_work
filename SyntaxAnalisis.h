#ifndef __SYNTAXANALISIS_
#define __SYNTAXANALISIS_
#include "Reserved.h"

class SintaxAnalisis{
public:
	SintaxAnalisis();
	SintaxAnalisis(LexTable *, unsigned int,\
	std::deque<int>*, std::deque<std::string>*);/////////////////
	~SintaxAnalisis();
	void CreateStrTable(LexTable *, unsigned int,\
	std::deque<int>*, std::deque<std::string>*);
	void PrintStrTable();
	std::deque<Str_struct>* getStrOfStrs();
	std::deque<int>* GetErrors();
private:
	void name(int, Str_struct *, int);
	void mnem(int, Str_struct *, int);
	void first(int, Str_struct *, int);
	void second(int, Str_struct *, int);
	void PrintStrLine(int,std::ofstream *);
	unsigned int lines;
	std::deque<Str_struct> structs_of_strs;
	LexTable *ptr_lex_table;
	std::deque<int>* Error_lines;
	std::deque<std::string>* file_deque;
};

#endif