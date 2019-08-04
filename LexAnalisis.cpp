#include "LexAnalisis.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <locale>
#include <iomanip>
#include <stdlib.h>
//КОНСТРУКТОР + ВКАЗІНИК НА ІМ'Я ФАЙЛУ
LexAnalisis::LexAnalisis(const char *filename){
	if(!file_deque.empty())
		file_deque.clear();
	if(!file_deque_upper.empty())
		file_deque_upper.clear();
	if(!Error_table.empty())
		Error_table.clear();
	lex_table = NULL;
	lines_numb = 0;
	this->ReadFile(filename);
}

LexAnalisis::LexAnalisis(){
	if(!file_deque.empty())
		file_deque.clear();
	if(!file_deque_upper.empty())
		file_deque_upper.clear();
	if(!Error_table.empty())
		Error_table.clear();
	lex_table = NULL;
	lines_numb = 0;
}

//ДЕСТРУКТОР ВИТИРАЄ ДЕКИ І ЗНИЩУЄ ПАМ'ЯТЬ ВИДІЛЕНУ ПІД ТАБЛИЦЮ ЛЕКСЕМ
LexAnalisis::~LexAnalisis(){
	if(!file_deque.empty())
		file_deque.clear();
	if(!file_deque_upper.empty())
		file_deque_upper.clear();
	if(!Error_table.empty())
		Error_table.clear();
	if(lex_table != NULL){
		delete [] lex_table;
	lex_table = NULL;
	lines_numb = 0;
	}
		
}
//ЗЧИТУЄ ФАЙЛ У ДЕК БКЗ ЗМІН
void LexAnalisis::ReadFile(const char *filename){
	std::ifstream file_asm;		//ФАЙЛ
	std::string tmp;			//ДЛДАТКОВИЙ РЯДОК
	file_asm.open(filename);	//ВІДКРИТТЯ ФАЙЛУ
	if(file_asm.is_open())
		while(std::getline(file_asm,tmp)){	//ЗЧИТУВАННЯ В TMP
			tmp += '\n';					//В КІНЕЦЬ ДОДАЄМО ПЕРЕХІД НА НОВИЙ РЯДОК
			file_deque.push_back(tmp);		//ЗАПИСУЄМО В ДЕК
			lines_numb++;
		}
	else {std::cout << "ERROR! file\n";
			exit(0);}
	file_asm.close();			//ЗАКРИВАЄМО ФАЙЛ
}
//ЗАПИСУЄ З ДЕКУ ДЛЯ ЗВИЧАЙНОГО ФАЙЛУ У ДЕК В ЯКОМУ ВСЕ З ВЕЛИКИХ ЛІТЕР
void LexAnalisis::FileToUpper(){
	std::string tmp;			//ДОДАТКОВИЙ РЯДОК
	int i,n;
	std::deque<std::string>::iterator it, itE;	//ІТЕРАТОРИ ДЛЯ ПЕРШОГО ДЕКУ
	if(!file_deque.empty()){//ЯКЩО ДЕК НЕ ПУСТИЙ ТО БУДЕМО ЗАПИСУВАТИ В ДЕК
		it = file_deque.begin();
		itE = file_deque.end();
	
		if(!file_deque_upper.empty())
			file_deque_upper.clear();	//ЯКЩО НОВИЙ ДЕК НЕ ПУСТИЙ, ТО ОЧИЩАЄМО
		while(it != itE){
			n = (int)(*it).length();	//ДОВЖИНА РЯДКА З ПЕРШОГО ДЕКУ
			for(i = 0; i < n; i++)		//ПОСИМВОЛЬНЕ ПЕРЕТВОРЕННЯ МАЛИХ ЛІТЕР НА ВЕЛИКІ
				tmp += toupper((*it).at(i));
			file_deque_upper.push_back(tmp);
			tmp.erase();
			it++;
		}
	}
}
//ДРУК ЗЧИТАНОГО ФАЙЛУ БЕЗ ЗМІН НА ЕКРАН
void LexAnalisis::PrintFile(){
	std::deque<std::string>::iterator it, itE;	//ІТЕРАТОРИ ДЛЯ ДЕКУ 1
	if(!file_deque.empty()){					//ЯКЩО ДЕК НЕ ПУСТИЙ
		it = file_deque.begin();
		itE = file_deque.end();
		while(it != itE){						//ВИВІД НА ЕКРАН
			std::cout << *(it++);				// << std::endl;
		}
	}
}
//ВИВІД НА ЕКРАН ЗМІНЕНОГО ДЕКУ З ВЕЛИКИМИ ЛІТЕРАМИ
void LexAnalisis::PrintFileUpper(){
	std::deque<std::string>::iterator it, itE;	//ІТЕРАТОРИ
	if(!file_deque_upper.empty()){				//ЯКЩО ДЕК НЕ ПУСТИЙ
		it = file_deque_upper.begin();
		itE = file_deque_upper.end();
		while(it != itE){						//ВИВІД НА ЕКРАН
			std::cout << *(it++);// << std::endl;
		}
	}
}

std::deque<std::string>* LexAnalisis::GetFileDeque(){
	return &file_deque;							//ПОВЕРТАЄМО АДРЕСУ ДЕКУ 1
}

std::deque<std::string>* LexAnalisis::GetFileDequeUpper(){
	return &file_deque_upper;					//ПОВЕРТАЄМО АДРЕСУ ДЕКУ 2
}
//ФОРМУВАННЯ ТАБЛИЦІ ЛЕКСЕМ ДЛЯ І-ГО РЯДКА
bool LexAnalisis::LexTableString(int i){//////////////////////////////////////////////////////////////////
	LexTableExample tmp;
	int j=0,k=1,n;
	char c;
	n = (int)file_deque_upper[i].length();
	while(j<n){															//ПОСИМВОЛЬНЕ ЗЧИТУВАННЯ
		c = file_deque_upper[i].at(j);
		if(std::string::npos != language.find_first_of(c))				//НЕДОПУСТИМИЙ СИМВОЛ
			{
			if(set_delimers.find(c) == set_delimers.end())				//РОЗДІЛЮВАЧ
				{
				if(set_one_simbol.find(c) == set_one_simbol.end())		//ОДНОСИМВОЛЬНА
					{
					if(set_math_simbol.find(c) == set_math_simbol.end())//МАТЕМАТИЧНИЙ СИМВОЛ
						{
						tmp.Lexem += c;		//ДОДАТИ СИМВОЛ ДО РЯДКА
						}
					else
						{
						if(!tmp.Lexem.empty())
							{tmp.numb = k;
							LexTablePushLex(i,&tmp);
							tmp.Lexem.erase();
							k++;}
						tmp.Lexem = c;
						tmp.numb = k;
						tmp.Type = "math_simbol";
						lex_table[i].example.push_back(tmp);
						tmp.Lexem.erase();
						k++;
						}
					}
				else
					{
					if(!tmp.Lexem.empty())
						{tmp.numb = k;
						LexTablePushLex(i,&tmp);
						tmp.Lexem.erase();
						k++;}
					tmp.Lexem = c;
					tmp.numb = k;
					tmp.Type = "one_simbol";
					lex_table[i].example.push_back(tmp);
					tmp.Lexem.erase();
					k++;
					}
				}
			else
				{
				if(!tmp.Lexem.empty())
					{tmp.numb = k;
					LexTablePushLex(i,&tmp);
					tmp.Lexem.erase();
					k++;}
				if(c == ';')
					{
					tmp.Lexem = c;
					tmp.numb = k;
					tmp.Type = "delimer";
					lex_table[i].example.push_back(tmp);
					tmp.Lexem.erase();
					return false;//-1;
					}
				}
			}
		else
			{
			return true;//i;		//ЯКЩО ПОМИЛКА
			}
		j++;
	}
	return false;//-1;
}
//ФОРМУВАННЯ ТАБЛИЦІ ЛЕКСЕМ
void LexAnalisis::CreateLexTable(){
	this->FileToUpper();							//ОБОВ'ЯЗКОВЕ СТВОРЕННЯ ФАЙЛУ КАПСОМ
	unsigned int i = 0;
	//Error E;
	lex_table = new LexTable[lines_numb];			//ВИДІЛЕННЯ ПАМ'ЯТІ ДЛЯ ТАБЛИЦІ
	while(i < lines_numb){
		//E.j = LexTableString(i);
		//if(E.j != -1)
		//	{E.i = i; Error_table.push_back(E);}	//ЯКЩО БУЛА ЗНАЙДЕНА ПОМИЛКА
		Error_table.push_back(false);
		if(true == LexTableString(i)){
			Error_table[i] = true;
		}
		i++;
	}
	return;
}
//ПОШУК В ТАБЛИЦЯХ ЛЕКСЕМ
void LexAnalisis::LexTablePushLex(int i,LexTableExample *tmp){
	if(set_reg32.find(tmp->Lexem) != set_reg32.end())
		{
		tmp->Type = "reg32";
		lex_table[i].example.push_back(*tmp);
		}
	else if(set_reg8.find(tmp->Lexem) != set_reg8.end())
		{
		tmp->Type = "reg8";
		lex_table[i].example.push_back(*tmp);
		}
	else if(set_segment_reg.find(tmp->Lexem) != set_segment_reg.end())
		{
		tmp->Type = "segment_reg";
		lex_table[i].example.push_back(*tmp);
		}
	else if(set_commands.find(tmp->Lexem) != set_commands.end())
		{
		tmp->Type = "command";
		lex_table[i].example.push_back(*tmp);
		}
	else if(set_directives.find(tmp->Lexem) != set_directives.end())
		{
		tmp->Type = "directive";
		lex_table[i].example.push_back(*tmp);
		}
	else if(set_types.find(tmp->Lexem) != set_types.end())
		{
		tmp->Type = "type";
		lex_table[i].example.push_back(*tmp);
		}
	else
		{						//ЯКЩО НЕ ЗНАЙДЕНО В ТАБЛИЦЯХ
		LexTableUnKnown(i,tmp);
		}
}
//ДЛЯ НЕ ЗНАЙДЕНИХ В ТАБЛИЦЯХ
void LexAnalisis::LexTableUnKnown(int i, LexTableExample *tmp){
	//Error E;									//ДЛЯ ПОМИЛОК
	int j, n;
	bool f = false;
	tmp->Type = "unknown";						//НЕВИЗНАЧЕНО
	if(tmp->Lexem.find(".") != std::string::npos) Error_table[i] = true;
	n = tmp->Lexem.length();
	if('B' == tmp->Lexem.at(n-1)){				//БІНАРНЕ ЧИСЛО
		f = true;
		for(j=0;j<n-1;j++){
			if(!FindBin(tmp->Lexem.at(j)))
			{f = false; break;}
		}
		if(f == true)
			tmp->Type = "Bin";
	}
	if(FindDec(tmp->Lexem.at(0)) &&\
		'H' == tmp->Lexem.at(n-1) && f == false){
		f = true;						//ШІСТНАДЦЯТКОВЕ ЧИСЛО
		for(j=1;j<n-1;j++){
			if(!FindHex(tmp->Lexem.at(j)))
			{f = false; break;}
		}
		if(f == true)
			tmp->Type = "Hex";
	}
	if(f == false){						//ДЕСЯТКОВЕ ЧИСЛО
		if('D' == tmp->Lexem.at(n-1) && n > 1)
			{n--;}
		f = true;
		for(j=0;j<n;j++){
			if(!FindDec(tmp->Lexem.at(j)))
			{f = false; break;}
		}
		if(f == true)
			tmp->Type = "Dec";
	}
	if(f == false && (FindDec(tmp->Lexem.at(0)) || n > 8))
		{tmp->Type = "error";				//БІЛЬШЕ 8 СИМВОЛІВ
											//АБО ШІСТНАДЦЯТКОВЕ [0] НЕ З ЦИФРИ
		Error_table[i]=true;}
	lex_table[i].example.push_back(*tmp);
}
//ПОШУК СИМВОЛІВ БІНАРНОГО ЧИСЛА
bool LexAnalisis::FindBin(char c){
	const char binNumb[2] = {'0','1'};
	bool f = true;
	for(int i =0; i < 2; i++){
		if(c != binNumb[i])
			f = false;
		else
			{f = true; break;}
	}return f;
}
//ПОШУК СИМВОЛІВ ШІСТНАДЦЯТКОВОГО ЧИСЛА
bool LexAnalisis::FindHex(char c){
	const char hexNumb[16] \
	= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	bool f = true;
	for(int i =0; i < 16; i++){
		if(c != hexNumb[i])
			f = false;
		else
			{f = true; break;}
	}return f;
}
//ПОШУК СИМВОЛІВ дЕСЯТКОВОГО ЧИСЛА
bool LexAnalisis::FindDec(char c){
	const char decNumb[10] = {'0','1','2','3','4','5','6','7','8','9'};
	bool f = true;
	for(int i =0; i < 10; i++){
		if(c != decNumb[i])
			f = false;
		else
			{f = true; break;}
	}return f;
}
//ВИВІД РЯДКА ТАБЛИЦІ ЛЕКСЕМ
void LexAnalisis::PrintStringLex(int i, std::ofstream *filename){
	int j,n;
	n = lex_table[i].example.size();
	if(filename->is_open())//*
	for(j = 0; j<n; j++){
		(*filename) <<std::setw(15)<< lex_table[i].example[j].numb <<\
		std::setw(15) << lex_table[i].example[j].Lexem << std::setw(15)\
		<< lex_table[i].example[j].Lexem.length() << std::setw(15)\
		<< lex_table[i].example[j].Type << "\n";
	}
}
//ВИВІД ТАБЛИЦІ ЛЕКСЕМ
void LexAnalisis::PrintLexTable(){
	std::ofstream out_file;
	int i;
	out_file.open("../other/LexAnalisis.txt");
	out_file <<"=\tLexAnalisis\t=\n";
	if(out_file.is_open())
	for(i = 0; i<(int)lines_numb; i++){
		out_file << "==================================================================begin:"<< i << "\n";
		PrintStringLex(i,&out_file);
		out_file << "====================================================================end:"<< i << "\n";
	}
	out_file.close();
}
//ВКАЗІВНИК НА ТАБЛИЦЮ
LexTable* LexAnalisis::GetLexTable(){
	return lex_table;
}
//ВКАЗІВНИК НА ТАБЛИЦЮ ПОМИЛОК
std::deque<int>* LexAnalisis::GetErrorTable(){/////////////////////////
	return &Error_table;
}
//КІЛЬКІСТЬ РЯДКІВ
unsigned int LexAnalisis::GetNumOfLines(){
	return lines_numb;
}