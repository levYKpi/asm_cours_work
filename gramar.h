#ifndef __GRAMAR_
#define __GRAMAR_

#include "Reserved.h"
#include <map>
#include <fstream>

class gramar
{
public:
	gramar();
	~gramar();
	void set_data(LexTable *, unsigned int, std::deque<int>*,\
	std::deque<Str_struct>*, std::deque<std::string>*);
	void IdentifyTable();
	void CreateSegmentTable();
	void CreateCommandSizeTable();
	bool data_seg();
	bool code_seg();
	void assume();
private:
	int sti_func(int );
	int jnz_func(int );
	int dec_func(int );
	int inc_func(int );
	int	add_func(int );
	int  or_func(int );
	int and_func(int );
	int cmp_func(int );
	int mov_func(int );
	int mem_op_func(int , int, int);
	
	long long* calculator(int , int);
	long long func_operation(long long, long long, char);
	
	long long find_mem(std::string*);
	long long find_label(std::string*);
	bool err_identify(std::string *, int);
	void PushIdentifySh(int);
	void print_c(std::ofstream* , int);
	
	LexTable *Lt_from_LA;
	std::deque<std::string>* file_asm;		//з лексичного аналізу, ввід файлу
	std::deque<Str_struct>* tmp_str_strs;	//з синтаксичного аналізу
	std::deque<std::string> identify_table;	//табличка ідентифікаторів !!!
	std::deque<int> identify_t_sh;			// розмір ідентифікаторів
	std::deque<int> command_t_sh;			//розмір команди
	std::deque<int> shift_table;
	Command_bites *table_comands_b;
	SegmentRegisters s_r_table;				//dc cs ss fs gs ...
	std::deque<SegmentTable> s_table;		//name segment type...
	std::deque<int>* err_table;				//error from syntax
	std::deque<Str_command> s_comands;		//forming code of command
	unsigned int lines;

	std::string _data, _code;
	int _data_n, _code_n;
	int _data_pos, _code_pos;
	bool segmants_error;
	
	std::map<std::string, int> ad_int_E;
	std::map<std::string, int> ad_int_nE;
};

#endif