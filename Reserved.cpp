#include "Reserved.h"


	int ad_int[8][8] = {0};
	
	const char delimers[4] = {' ','\t','\n',';'};
	const std::string reg32[8] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
	const std::string reg8[8] = {"AL","CL","DL","BL","AH","CH","DH","BH"};
	const std::string segment_reg[6] = {"CS", "SS", "DS", "ES", "FS", "GS"};
	const std::string commands[9] = {"STI","JNZ","INC","ADD","OR","AND","CMP","MOV","DEC"};
	const char one_simbol[4] = {'[',']'/*,'.'*/,',',':'};
	const char math_simbol[6] = {'*','/','+','-','(',')'};
	const std::string directives[3] = {"END","SEGMENT","ENDS"};
	const std::string types[3] = {"DB","DW","DD"};
	const std::string language = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@_?$[],.:;*/+-() \t\n";
	
	std::set<char> set_delimers(delimers,delimers+4);
	std::set<std::string> set_reg32(reg32,reg32+8);
	std::set<std::string> set_reg8(reg8,reg8+8);
	std::set<std::string> set_segment_reg(segment_reg,segment_reg+6);
	std::set<std::string> set_commands(commands,commands+9);
	std::set<char> set_one_simbol(one_simbol,one_simbol+4);
	std::set<char> set_math_simbol(math_simbol,math_simbol+6);
	std::set<std::string> set_directives(directives,directives+3);
	std::set<std::string> set_types(types,types+3);
	
	Pole::Pole(){position = 0; numb = 0;}
	T_str_struct::T_str_struct(){name = 0;}