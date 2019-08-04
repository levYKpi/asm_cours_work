#ifndef __RESERVED_
#define __RESERVED_
#include <set>
#include <string>
#include <deque>
	
typedef struct T_LexTableExample{
	std::string Lexem;
	std::string Type;
	int numb;
}LexTableExample;

typedef struct T_LexTable{
	std::deque<LexTableExample> example;
}LexTable;

struct SegmentTable{
	std::string name;
	short _category;
	int b , e;
};

typedef struct T_seg_reg{
	std::string cs_r,ds_r,ss_r,es_r,gs_r,fs_r;
	T_seg_reg()
		{cs_r=ds_r=ss_r=es_r=gs_r=fs_r="nothing";}
}SegmentRegisters;

typedef struct Str_command_t{
	short one_b_pref, cod_op, RMsib, p_com, dat;
	bool c_exist;
}Str_command;

struct Pole{
	int position;
	int numb;
	Pole();
};

typedef struct T_str_struct{
	int name;
	Pole Mnem;
	Pole First;
	Pole Second;
	T_str_struct();
}Str_struct;

typedef struct T_Command_bites{
	int	pref_com, pref_com_n;
	int	pref_s_adr, pref_s_adr_n;
	int	pref_s_op, pref_s_op_n;
	int	pref_c_seg, pref_c_seg_n;
	int	code_op, code_op_n;
	int	modrm, modrm_n;
	int	sib, sib_n;
	int	shift, shift_n;
	long long	znach, znach_n;
}Command_bites;

	extern int ad_int[8][8];
	
	extern const char delimers[4];
	extern const std::string reg32[8];
	extern const std::string reg8[8];
	extern const std::string segment_reg[6];
	extern const std::string commands[9];
	extern const char one_simbol[4];
	extern const char math_simbol[6];
	extern const std::string directives[3];
	extern const std::string types[3];
	extern const std::string language;
	
	extern std::set<char> set_delimers;
	extern std::set<std::string> set_reg32;
	extern std::set<std::string> set_reg8;
	extern std::set<std::string> set_segment_reg;
	extern std::set<std::string> set_commands;
	extern std::set<char> set_one_simbol;
	extern std::set<char> set_math_simbol;
	extern std::set<std::string> set_directives;
	extern std::set<std::string> set_types;


#endif