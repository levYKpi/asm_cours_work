#include "LexAnalisis.h"
#include "SyntaxAnalisis.h"
#include <iostream>
#include <string.h>
#include <gramar.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	std::string tmp_f;
	std::cout << " input filename:\n ../other/<filename>[.asm] : ";
	std::cin >> tmp_f;
	tmp_f.insert(0,"../other/");
	tmp_f += ".asm";
	//std::string tmp_f = "../other/MY_TEST.asm";
	//if(argc == 2){ tmp_f += argv[1];
	std::cout << "ASM_COMPILER_CPP\n\n";
	LexAnalisis L;
	L.ReadFile((const char*)tmp_f.c_str());
	std::cout<<std::endl;
	L.CreateLexTable();
	L.PrintLexTable();
	SintaxAnalisis S;
	S.CreateStrTable(L.GetLexTable(),L.GetNumOfLines(),L.GetErrorTable(),L.GetFileDeque());
	S.PrintStrTable();
	gramar G;
	G.set_data(L.GetLexTable(),L.GetNumOfLines(),S.GetErrors(),S.getStrOfStrs(),L.GetFileDeque());
	G.IdentifyTable();
	G.CreateSegmentTable();
	G.data_seg();
	G.code_seg();
	G.assume();
	G.CreateCommandSizeTable();
	exit(0);
	return 0;
}
