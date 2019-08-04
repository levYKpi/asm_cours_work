#include "SyntaxAnalisis.h"
#include <iostream>
#include <fstream>
#include <iomanip>
//КОНСТРУКТОР 1
SintaxAnalisis::SintaxAnalisis(){
	lines = 0;
	ptr_lex_table = NULL;
	if(!structs_of_strs.empty())
		structs_of_strs.clear();
}
//КОНСТРУКТОР 2
SintaxAnalisis::SintaxAnalisis(LexTable * LT,unsigned int L,\
std::deque<int>* E, std::deque<std::string>* F){
	lines = 0;
	ptr_lex_table = NULL;
	if(!structs_of_strs.empty())
		structs_of_strs.clear();
	this->CreateStrTable(LT,L,E,F);
}
//ДЕСТРУКТОР
SintaxAnalisis::~SintaxAnalisis(){
	lines = 0;
	if(!structs_of_strs.empty())
		structs_of_strs.clear();
	ptr_lex_table = NULL;
	
}
//СТВОРЕННЯ ТАБЛИЦІ СТРУКТУРИ РЯДКА
void SintaxAnalisis::CreateStrTable(LexTable * LT,unsigned int L,\
std::deque<int>* E, std::deque<std::string>* F){
	Str_struct tmp;
	int i , n;
	file_deque = F;
	Error_lines = E;
	ptr_lex_table = LT;
	lines = L;
	for(i = 0; i < (int)lines; i++){
		n = ptr_lex_table[i].example.size();
		this->name(i,&tmp,n);
		this->mnem(i,&tmp,n);
		this->first(i,&tmp,n);
		this->second(i,&tmp,n);
		structs_of_strs.push_back(tmp);
	}
}
//ЗАПОВНЕННЯ ПОЛЯ ІМЕНІ (МІТКИ)
void SintaxAnalisis::name(int i, Str_struct *tmp, int n){
	tmp->name = 0;
	if(n >= 1)
		if(ptr_lex_table[i].example[0].Type == "unknown"){
			if(n > 1){
				if(	ptr_lex_table[i].example[1].Lexem == ":" 		||\
					ptr_lex_table[i].example[1].Type == "directive"	||\
					ptr_lex_table[i].example[1].Type == "type"		){
						tmp->name = 1;
				}
			}else{
				(*Error_lines)[i] = 1;
			}
		}
}
//ЗАПОВНЕННЯ ПОЛЯ М'ЄМОНІКИ
void SintaxAnalisis::mnem(int i, Str_struct *tmp, int n){
	int j , p;
	//Error E;
	p = -1;
	tmp->Mnem.numb = tmp->Mnem.position = 0;
	for(j = tmp->name; j < n; j++){
		if(	ptr_lex_table[i].example[j].Type == "directive"	||\
			ptr_lex_table[i].example[j].Type == "type"		||\
			ptr_lex_table[i].example[j].Type == "command"	){
				if(p == -1) p = j+1;
				tmp->Mnem.numb++;
			}
	}
	if(tmp->Mnem.numb > 1) {(*Error_lines)[i] = true;}
	if(p != -1) tmp->Mnem.position = p;
}
//ЗАПОВНЕННЯ ПОЛЯ ПЕРШОГО ОПЕРАНДУ
void SintaxAnalisis::first(int i, Str_struct *tmp, int n){
	int j, p = -1,k=0;
	//Error E;
	tmp->First.numb = tmp->First.position = 0;
	if(tmp->Mnem.numb != 0){
		for(j = tmp->Mnem.numb + tmp->Mnem.position -1; j<n; j++){
			if(ptr_lex_table[i].example[j].Lexem == ",")
				{if(p == -1) p = j; k++;}
		}
		if(k > 1) {(*Error_lines)[i] = true;}
		if(p == -1)
			{if(ptr_lex_table[i].example[n-1].Type == "delimer")
				p = n-1;
			else
				p = n;}
		if(tmp->Mnem.numb + tmp->Mnem.position -1 < p){
			tmp->First.position = tmp->Mnem.numb + tmp->Mnem.position;
			tmp->First.numb = p + 1 - tmp->First.position;
			}
	}
}
//ЗАПОВНЕННЯ ПОЛЯ ДРУГОГО ОПЕРАНДУ
void SintaxAnalisis::second(int i, Str_struct *tmp, int n){
	int p = 0;
	tmp->Second.numb = tmp->Second.position = 0;
	if(tmp->First.numb != 0){
			if(ptr_lex_table[i].example[n-1].Type == "delimer")
				{p = n-1;}
			else
				{p = n;}
		if(tmp->First.numb + tmp->First.position < p){
			tmp->Second.position = tmp->First.numb + tmp->First.position + 1;
			tmp->Second.numb = p + 1 - tmp->Second.position;
			}
		if(tmp->Second.numb == 0 && ptr_lex_table[i].example[n-1].Lexem == ",")
			(*Error_lines)[i] = true;
	}
}
//ВИВІД ТАБЛИЦІ
void SintaxAnalisis::PrintStrTable(){
	int i;
	std::ofstream file_out;
	file_out.open("../other/str_table.txt");
	if(file_out.is_open()){
	file_out <<"=\tSyntaxAnalisis\t=\n";
	for(i = 0; i < (int)lines; i++){
		file_out << i << "\t{-------------------------------------------------------------------\n";
		file_out << "LINE_OF_ASM_CODE:\n"\
		<< (*file_deque)[i] << "Synatx:\n\n";
		file_out << "label"<<std::setw(10)<<"mnem "<<std::setw(20)<<"first"<<std::setw(22)<<"second\n";
		PrintStrLine(i,&file_out);
	}
	}
	file_out.close();
}
//ВИВІД РЯДКА ТАБЛИЦІ
void SintaxAnalisis::PrintStrLine(int i,std::ofstream *filename){
	if((*filename).is_open()){
		(*filename) << structs_of_strs[i].name;
		(*filename) << std::setw(10) << structs_of_strs[i].Mnem.position << std::setw(10);
		(*filename) << structs_of_strs[i].Mnem.numb << std::setw(10);
		(*filename) << structs_of_strs[i].First.position << std::setw(10);
		(*filename) << structs_of_strs[i].First.numb << std::setw(10);
		(*filename) << structs_of_strs[i].Second.position << std::setw(10);
		(*filename) << structs_of_strs[i].Second.numb << "\n";
		if((*Error_lines)[i] == true) (*filename) << " !!!error!!!\n";
		(*filename) <<"-----------------------------------------------------------------------}\n\n";
	}
}

std::deque<Str_struct>* SintaxAnalisis::getStrOfStrs(){
	return &structs_of_strs;
}

std::deque<int>* SintaxAnalisis::GetErrors(){
	return Error_lines;
}