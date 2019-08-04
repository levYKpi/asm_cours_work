#include "gramar.h"
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

gramar::gramar(){
	_data = _code = "nothing";
	_data_n = _code_n = 0;
	segmants_error = false;
	table_comands_b = NULL;
	int k = 0x0;
	for(int j = 0; j < 8; j++){
		for(int i = 0; i < 8; i++){
			ad_int[i][j] = k;
			k++;
		}
		ad_int_E[reg32[j]] = j;
		ad_int_nE[reg8[j]] = j;
	}
}

gramar::~gramar(){
	delete table_comands_b;
	table_comands_b = NULL;
}

bool gramar::data_seg(){
	int i, j, n = s_table.size();
	bool f = true;
	if(n > 2){
		segmants_error = true;
		return false;
	}
	for(i = 0; i < n; i++){
		for(j = s_table[i].b + 1; j < s_table[i].e; j++){
			if(((*tmp_str_strs)[j].name == 1 &&\
			Lt_from_LA[j].example[1].Type == "type")||\
			((*tmp_str_strs)[j].name == 0 &&\
			(*tmp_str_strs)[j].Mnem.numb == 0 &&\
			(*tmp_str_strs)[j].First.numb == 0 &&\
			(*tmp_str_strs)[j].Second.numb == 0)){
				f = true;
			}else{
				f = false;
				break;
			}
		}
		if(f == true){
			_data = s_table[i].name;
			_data_pos = i;
			_data_n++;
		}
	}
	if(_data_n > 1){
		segmants_error = true;
		return false;
	}
	return (bool)_data_n;
}

bool gramar::code_seg(){
	_code_n = 0;
	int i, j, n = s_table.size();
	bool f = false;
	std::string t;
	if(n > 2){
		segmants_error = true;
		return false;
	}
	for(i = 0; i < n; i++){
		for(j = s_table[i].b + 1; j < s_table[i].e; j++){
			t = Lt_from_LA[j].example[0].Lexem;
			if((*tmp_str_strs)[j].Mnem.numb != 0){
				if(Lt_from_LA[j].example[(*tmp_str_strs)[j]\
				   .Mnem.position - 1].Type != "type"){
					f = true;
					break;
				}
			}else if((*tmp_str_strs)[j].name == 1 &&\
					 (*tmp_str_strs)[j].Mnem.numb == 0){
				if(Lt_from_LA[j].example[1].Lexem == ":"){
					f = true;
					break;
				}
			}else{
				f = false;
			}
		}
		if(f == true){
			_code = s_table[i].name;
			_code_pos = i;
			_code_n++;
		}
	}
	if(_code_n > 1){
		segmants_error = true;
		return false;
	}
	return (bool)_code_n;
}

void gramar::set_data(LexTable *Lt, unsigned int l,\
std::deque<int> *err_t, std::deque<Str_struct>* sa, std::deque<std::string>* fa){
	Lt_from_LA = Lt;
	lines = l;
	err_table = err_t;
	tmp_str_strs = sa;
	file_asm = fa;
	table_comands_b = new Command_bites[lines];
	for(int i = 0; i < (int)lines; i++){
		table_comands_b[i].pref_com = 0;
		table_comands_b[i].pref_s_adr = 0;
		table_comands_b[i].pref_s_op = 0;
		table_comands_b[i].pref_c_seg = 0;
		table_comands_b[i].code_op = 0;
		table_comands_b[i].modrm = 0;
		table_comands_b[i].sib = 0;
		table_comands_b[i].shift = 0;
		table_comands_b[i].znach = 0;
		table_comands_b[i].pref_com_n = 0;
		table_comands_b[i].pref_s_adr_n = 0;
		table_comands_b[i].pref_s_op_n = 0;
		table_comands_b[i].pref_c_seg_n = 0;
		table_comands_b[i].code_op_n = 0;
		table_comands_b[i].modrm_n = 0;
		table_comands_b[i].sib_n = 0;
		table_comands_b[i].shift_n = 0;
		table_comands_b[i].znach_n = 0;
	}
 }

void gramar::IdentifyTable(){
	unsigned int i;
	for(i = 0; i < lines; i++){
		if((*tmp_str_strs)[i].name == 1)
		{
			if(err_table->at(i) == 0){
				if(err_identify(&(Lt_from_LA[i].example[0].Lexem),i) == 0)
				{
					identify_table.push_back(Lt_from_LA[i].example[0].Lexem);
					this->PushIdentifySh(i);
				}else{
					identify_table.push_back("-");
					identify_t_sh.push_back(-2);
				}
			}else{
				identify_table.push_back("-");
				identify_t_sh.push_back(-2);
			}
		}else{
			identify_table.push_back("-");
			identify_t_sh.push_back(-2);
		}
	}
	std::ofstream _file;
	_file.open("../other/id_table.txt");
	if(_file.is_open())
		for(i = 0; i < lines; i++){
			_file << identify_table[i] <<"\t \t "<<identify_t_sh[i]<< "\n";
		}
}

bool gramar::err_identify(std::string *s, int i){
	int j, n;
	bool f = 0;
	n = identify_table.size();
	for(j = 0; j < n; j++){
		if(identify_table[j] == *s){
			if(Lt_from_LA[i].example[1].Lexem == "ENDS"){
				f = 1;
				break;
			}else{
				f = 1;
				(*err_table)[i] = 1;
				break;
			}
		}else{
			f = 0;
		}
	}
	return f;
}

void gramar::PushIdentifySh(int i){
	std::string t = Lt_from_LA[i].example[1].Lexem;
	if(Lt_from_LA[i].example[1].Lexem == "SEGMENT")
	{
		identify_t_sh.push_back(0);
	}else if(Lt_from_LA[i].example[1].Lexem == ":")
	{
		identify_t_sh.push_back(-1);
	}else if(Lt_from_LA[i].example[1].Lexem == "DB")
	{
		identify_t_sh.push_back(1);
	}else if(Lt_from_LA[i].example[1].Lexem == "DW")
	{
		identify_t_sh.push_back(2);
	}else if(Lt_from_LA[i].example[1].Lexem == "DD")
	{
		identify_t_sh.push_back(4);
	}else
	{
		identify_t_sh.push_back(-2);
		(*err_table)[i] = true;
	}
}

void gramar::CreateSegmentTable(){
	unsigned int i;
	struct SegmentTable tmp;
	std::string s_lex_tmp;
	bool seg_open = false;
	for(i = 0; i < lines; i++){
		if(Lt_from_LA[i].example[1].Lexem == "SEGMENT"){
			if(seg_open == false){
				seg_open = true;
				s_lex_tmp = Lt_from_LA[i].example[0].Lexem;
				tmp.b = i;
			}else{
				(*err_table)[i] = 1;
			}
		}
		if(Lt_from_LA[i].example[1].Lexem == "ENDS"){
			if(seg_open == true && Lt_from_LA[i].example[0]\
			   .Lexem == s_lex_tmp){
				seg_open = false;
				tmp.name = s_lex_tmp;
				tmp._category = 32;
				tmp.e = i;
				s_table.push_back(tmp);
			}else{
				(*err_table)[i] = 1;
			}
		}
	}
}

void gramar::assume(){
	s_r_table.cs_r = _code;
	s_r_table.ds_r = _data;
}

void gramar::CreateCommandSizeTable(){
	int s = -2; short ip = 0;
	long long* calc_ = 0;
	for(int i = 0; i<(int)lines; i++){
		if(identify_t_sh[i] >= 0){
			command_t_sh.push_back(identify_t_sh[i]);
			if(identify_t_sh[i] >= 1){
				if((*tmp_str_strs)[i].First.numb != 0)
					calc_ = calculator(1, i);
				if((*tmp_str_strs)[i].Second.numb != 0 \
				|| calc_ == NULL)
					(*err_table)[i]=1;
				else{
					if(identify_t_sh[i] == 1 \
					&& *calc_ >= -255 && *calc_ <= 255){
						table_comands_b[i].znach = *calc_ & 0xff;
						table_comands_b[i].znach_n = 1;
					}
					else if(identify_t_sh[i] == 2 \
					&& *calc_ >= -0xffff && *calc_ <= 0xffff){
						table_comands_b[i].znach = *calc_ & 0xffff;
						table_comands_b[i].znach_n = 2;
					}
					else if(identify_t_sh[i] == 4){
						if((*tmp_str_strs)[i].First.numb <= 2)
							table_comands_b[i].znach = (*calc_ & 0x000000ff)*0x1000000 \
							+ (*calc_ & 0x0000ff00)*0x100 + (*calc_ & 0x00ff0000)/0x100 \
							+ (*calc_ & 0xff000000)/0x1000000;
						else
							table_comands_b[i].znach = *calc_ & 0xffffffff;
						table_comands_b[i].znach_n = 4;
					}
					delete calc_;
				}
			}
		}else{
			if((*tmp_str_strs)[i].Mnem.numb != 0){
				if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "STI")
					s = sti_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "JNZ")
					s = jnz_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "DEC")
					s = dec_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "INC")
					s = inc_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "ADD")
					s = add_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "OR")
					s = or_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "AND")
					s = and_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "CMP")
					s = cmp_func(i);
				else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].Mnem.position - 1].Lexem == "MOV")
					s = mov_func(i);
				else if(Lt_from_LA[i].example[1].Lexem == "ENDS")
					s = -1;
				else if(Lt_from_LA[i].example[0].Lexem == "END")
					s = -3;
				command_t_sh.push_back(s);
				s = -2;
			}else{
				command_t_sh.push_back(s);
			}
		}
	}
	
	for(int i = 0; i<(int)lines; i++){
		if(command_t_sh[i] == 0){ ip = 0;}
		if(command_t_sh[i] >= -1){shift_table.push_back(ip); ip += command_t_sh[i];}
		else if(identify_t_sh[i] == -1){shift_table.push_back(ip);}
		else{shift_table.push_back(-2);}
	}
	
	for(int i = lines - 1; i >= 0; i--){
		if(command_t_sh[i] <= -2){
			if(Lt_from_LA[i].example[0].Lexem == "END") {
				if(find_label(&Lt_from_LA[i].example[1].Lexem) > i)
					segmants_error = 1;
				break;}
		}else {segmants_error = 1; break;}
	}
	
	std::ofstream f_shift;
	f_shift.open("../other/f_shift.txt");
	if(f_shift.is_open())
	for(int i = 0; i<(int)lines; i++){
		if(shift_table[i] >= 0)
			f_shift << std::setw(4) << std::setfill('0') << shift_table[i] << " ";
		if(table_comands_b[i].shift_n != 0 && table_comands_b[i].shift >= 0)
			table_comands_b[i].shift = shift_table[table_comands_b[i].shift];
		print_c(&f_shift , i);
	}
	
	if(segmants_error == false){
	f_shift<<"\n\n===============================================================================\n";
	f_shift<<"\tname \tcategory \tsize\n";
	
	f_shift<<"\t"<<s_table[_data_pos].name<<" \t\t\t"<<std::dec<<s_table[_data_pos]._category\
	<<" \t"<<std::hex<<std::setw(4)<<std::setfill('0')<<shift_table[s_table[_data_pos].e]<<"\n";
	
	f_shift<<"\t"<<s_table[_code_pos].name<<" \t\t\t"<<std::dec<<s_table[_code_pos]._category\
	<<" \t"<<std::hex<<std::setw(4)<<std::setfill('0')<<shift_table[s_table[_code_pos].e]<<"\n";
	f_shift<<"===============================================================================\n";

	
	for(int i = s_table[_data_pos].b; i<s_table[_data_pos].e; i++){
		if(identify_t_sh[i] == 1){

			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem\
			<<"      "<<std::setfill('0')<<std::setw(2);
			f_shift << "byte    "<< std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_data_pos].name<< "\n";
			
		}else if(identify_t_sh[i] == 2){
			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem\
			<<"      "<<std::setfill('0')<<std::setw(4);
			f_shift << "word    "<< std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_data_pos].name<< "\n";
		}else if(identify_t_sh[i] == 4){
			
			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem<<"  "<<std::setfill('0');
			
			f_shift << "    dword   "<< std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_data_pos].name<< "\n";
		}
	}
	
	for(int i = s_table[_code_pos].b; i<s_table[_code_pos].e; i++){
		if(identify_t_sh[i] == 1){
			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem\
			<<"      "<<std::setfill('0')<<std::setw(2);
			f_shift << "byte    "<< std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_code_pos].name<< "\n";
			
		}else if(identify_t_sh[i] == 2){
			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem\
			<<"      "<<std::setfill('0')<<std::setw(4);//<<std::hex<<(short)table_comands_b[i].znach;
			f_shift << "word    "<< std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_code_pos].name<< "\n";
			
		}else if(identify_t_sh[i] == 4){
			
			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem<<"  "<<std::setfill('0');
			
			f_shift << "    dword   "<< std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_code_pos].name<< "\n";
		}
		if(identify_t_sh[i] == -1){
			f_shift <<std::setw(10)<<std::setfill(' ')<<Lt_from_LA[i].example[0].Lexem<<"      label   ";
			f_shift << std::setw(4)<<std::setfill('0') << shift_table[i] << "    ";
			f_shift <<s_table[_code_pos].name<< "\n";
		}
	}
	}else{
		f_shift <<"--------------------------------segments_error---------------------------------\n";
	}
	f_shift<<"===============================================================================";
	f_shift << "\n\t\tassume:\n"<<\
	"\tcs: " << s_r_table.cs_r << "\n"<<\
	"\tss: " << s_r_table.ss_r << "\n"<<\
	"\tds: " << s_r_table.ds_r << "\n"<<\
	"\tes: " << s_r_table.es_r << "\n"<<\
	"\tfs: " << s_r_table.fs_r << "\n"<<\
	"\tgs: " << s_r_table.gs_r << "\n";
	
	f_shift.close();
}

void gramar::print_c(std::ofstream *f, int i){
	int pr = 32;
	if(f->is_open()){
		(*f) << std::hex;
		if(table_comands_b[i].pref_com_n != 0){
			pr-=table_comands_b[i].pref_com_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].pref_com_n)<<\
			table_comands_b[i].pref_com<<" ";pr--;
		}
		if(table_comands_b[i].pref_s_adr_n != 0){
			pr-=table_comands_b[i].pref_s_adr_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].pref_s_adr_n)<<\
			table_comands_b[i].pref_s_adr<<" ";pr--;
		}
		if(table_comands_b[i].pref_s_op_n != 0){
			pr-=table_comands_b[i].pref_s_op_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].pref_s_op_n)<<\
			table_comands_b[i].pref_s_op<<" ";pr--;
		}
		if(table_comands_b[i].pref_c_seg_n != 0){
			pr-=table_comands_b[i].pref_c_seg_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].pref_c_seg_n)<<\
			table_comands_b[i].pref_c_seg<<" :";pr-=2;
		}
		if(table_comands_b[i].code_op_n != 0){
			pr-=table_comands_b[i].code_op_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].code_op_n)<<\
			table_comands_b[i].code_op<<" ";pr--;
		}
		if(table_comands_b[i].modrm_n != 0){
			pr-=table_comands_b[i].modrm_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].modrm_n)<<\
			table_comands_b[i].modrm<<" ";pr--;
		}
		if(table_comands_b[i].sib_n != 0){
			pr-=table_comands_b[i].sib_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].sib_n)<<\
			table_comands_b[i].sib<<" ";pr--;
		}
		if(table_comands_b[i].shift_n != 0){
			pr-=table_comands_b[i].shift_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].shift_n)<<\
			table_comands_b[i].shift<<" ";pr--;
		}
		if(table_comands_b[i].znach_n != 0){
			pr-=table_comands_b[i].znach_n*2;
			(*f)<<std::setfill('0')<<std::setw(2*table_comands_b[i].znach_n)<<\
			table_comands_b[i].znach<<" ";pr--;
		}
		(*f) << std::setfill(' ');
		(*f) << std::setw(pr) << " ";
		if((*file_asm)[i].length() >= 48)
			(*file_asm)[i].insert(47,"\n                                ");
		(*f) << (*file_asm)[i];
		if((*err_table)[i] !=0 )
			(*f) << "--------------------------------error( "<<std::dec<<i+1\
			<<" )-------------------------------------\n";
	}
}

int gramar::sti_func(int i){
	if((*tmp_str_strs)[i].First.numb == 0 \
	&& (*tmp_str_strs)[i].Second.numb == 0){
		table_comands_b[i].code_op_n = 1;
		table_comands_b[i].code_op = 0xFB;
		return 1;
	}else{
		(*err_table)[i] = 1; return -1;
	}
}

int gramar::add_func(int i){
	std::string a, b;
	if((*tmp_str_strs)[i].First.numb == 1 \
	&& (*tmp_str_strs)[i].Second.numb == 1){
		a = Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Lexem;
		b = Lt_from_LA[i].example[(*tmp_str_strs)[i].Second.position-1].Lexem;
		table_comands_b[i].code_op_n = 1;
		table_comands_b[i].modrm_n = 1;
		if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Type == "reg8"\
		&& Lt_from_LA[i].example[(*tmp_str_strs)[i].Second.position-1].Type == "reg8"){
			table_comands_b[i].code_op = 0x02;
			table_comands_b[i].modrm = 0xC0 + ad_int[ad_int_nE[b]][ad_int_nE[a]];
			return 2;
		}else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Type == "reg32"\
			  && Lt_from_LA[i].example[(*tmp_str_strs)[i].Second.position-1].Type == "reg32"){
			table_comands_b[i].code_op = 0x03;
			table_comands_b[i].modrm = 0xC0 + ad_int[ad_int_E[b]][ad_int_E[a]];
			return 2;
		}else{
			(*err_table)[i] = 1; return -1;
		}
	}else{
		(*err_table)[i] = 1; return -1;
	}
}

int gramar::mov_func(int i){
	std::string tmp;
	long long *calc;
	if((*tmp_str_strs)[i].First.numb == 1 
	&& (*tmp_str_strs)[i].Second.numb > 0){
		tmp = Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Lexem;
		table_comands_b[i].code_op_n = 1;
		calc = calculator(2, i);
		if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Type == "reg8"){
			table_comands_b[i].code_op = 0xB0 + ad_int_nE[tmp];
			if(calc != NULL){
				if(*calc >= -255 && *calc <= 255){
					table_comands_b[i].znach = *calc & 0xff;
					table_comands_b[i].znach_n = 1;
					delete calc;
				}else
					(*err_table)[i] = 1;
			}
			return 2;
		}else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Type == "reg32"){
			table_comands_b[i].code_op = 0xB8 + ad_int_E[tmp];
			if(calc != NULL){
				table_comands_b[i].znach = *calc & 0xffffffff;
				table_comands_b[i].znach_n = 4;
				delete calc;
			}
			return 5;
		}else{
			(*err_table)[i] = 1; if(calc != 0) delete calc; return -1;
		}
	}else{
		(*err_table)[i] = 1; return -1;
	}
}

int gramar::dec_func(int i){
	std::string tmp;
	if((*tmp_str_strs)[i].Second.numb == 0 \
	&& (*tmp_str_strs)[i].First.numb == 1){
		tmp = Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Lexem;
		table_comands_b[i].code_op_n = 1;
		if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Type == "reg8"){
			table_comands_b[i].code_op = 0xFE;
			table_comands_b[i].modrm_n = 1;
			table_comands_b[i].modrm = 0xC8 + ad_int_nE[tmp];
			return 2;
		}else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position-1].Type == "reg32"){
			table_comands_b[i].code_op = 0x48 + ad_int_E[tmp];
			return 1;
		}else{
			(*err_table)[i] = 1; return -1;
		}
	}else{
		(*err_table)[i] = 1; return -1;
	}
}

int gramar::inc_func(int i){
	int mem_f = -1;
	if((*tmp_str_strs)[i].Second.numb == 0 \
	&& (*tmp_str_strs)[i].First.numb >= 4){
		table_comands_b[i].code_op_n = 1;
		mem_f = mem_op_func(1, i, 0);
		if(mem_f != -1){
			table_comands_b[i].code_op += 0xFE;
			return mem_f + 1;
		}else{
			return -1;
		}
	}else{
		(*err_table)[i] = 1;
		return -1;
	}
}

int gramar::or_func(int i){
	std::string tmp_t;
	int k;
	int tmp_pos;
	int mem_f = -1;
	if((*tmp_str_strs)[i].First.numb == 1 \
	&& (*tmp_str_strs)[i].Second.numb >= 4){
		table_comands_b[i].code_op_n = 1;
		tmp_t = Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position -1].Type;
		tmp_pos = (*tmp_str_strs)[i].Second.position + (*tmp_str_strs)[i].Second.numb -4;
		if(tmp_t == "reg8"||\
		tmp_t == "reg32"){
			if(tmp_t == "reg8"){
				mem_f = mem_op_func(2, i, 1);
				table_comands_b[i].code_op = 0x0A;
				if(Lt_from_LA[i].example[tmp_pos].Lexem == "*") k = 0x04;
				else k = 0x80;
				table_comands_b[i].modrm += k + 8*ad_int_nE[Lt_from_LA[i].example\
				[(*tmp_str_strs)[i].First.position -1].Lexem];
			}
			else if(tmp_t == "reg32"){
				mem_f = mem_op_func(2, i, 4);
				table_comands_b[i].code_op = 0x0B;
				if(Lt_from_LA[i].example[tmp_pos].Lexem == "*") k = 0x04;
				else k = 0x80;
				table_comands_b[i].modrm += k + 8*ad_int_E[Lt_from_LA[i].example\
				[(*tmp_str_strs)[i].First.position -1].Lexem];
			}else{
				(*err_table)[i] = 1;
				return -1;}
				return mem_f + 1;
		}else{
			(*err_table)[i] = 1;
			return -1;
		}
	}else{
		(*err_table)[i] = 1;
		return -1;
	}
}

int gramar::and_func(int i){
	int mem_f = -1;
	int k, tmp_pos;
	std::string tmp_t;
	if((*tmp_str_strs)[i].Second.numb == 1 \
	&& (*tmp_str_strs)[i].First.numb >= 4){
		table_comands_b[i].code_op_n = 1;
		tmp_t = Lt_from_LA[i].example[(*tmp_str_strs)[i].Second.position -1].Type;
		tmp_pos = (*tmp_str_strs)[i].First.position + (*tmp_str_strs)[i].First.numb -4;
		if(tmp_t == "reg8"||tmp_t == "reg32"){
			if(tmp_t == "reg8"){
				mem_f = mem_op_func(1, i, 1);
				table_comands_b[i].code_op = 0x20;
				if(Lt_from_LA[i].example[tmp_pos].Lexem == "*") k = 0x04;
				else k = 0x80;
				table_comands_b[i].modrm += k + 8*ad_int_nE[Lt_from_LA[i].example\
				[(*tmp_str_strs)[i].Second.position -1].Lexem];
			}
			else if(tmp_t == "reg32"){
				mem_f = mem_op_func(1, i, 4);
				table_comands_b[i].code_op = 0x21;
				if(Lt_from_LA[i].example[tmp_pos].Lexem == "*") k = 0x04;
				else k = 0x80;
				table_comands_b[i].modrm += k + 8*ad_int_E[Lt_from_LA[i].example\
				[(*tmp_str_strs)[i].Second.position -1].Lexem];
			}else{
				(*err_table)[i] = 1;
				return -1;
			}
			return mem_f + 1;
		}else{
			(*err_table)[i] = 1;
			return -1;
		}
	}else{
		(*err_table)[i] = 1;
		return -1;
	}
}

int gramar::cmp_func(int i){
	long long* calc_ = 0;
	if((*tmp_str_strs)[i].Second.numb > 0 \
	&& (*tmp_str_strs)[i].First.numb == 1){
		table_comands_b[i].code_op_n = 1;
		calc_ = calculator(2,i);
		if(calc_ == NULL)
			return -1;
		if(Lt_from_LA[i].example[(*tmp_str_strs)[i].\
		First.position-1].Lexem == "AL"){
			if(*calc_ >= -255 && *calc_ <= 255){
				table_comands_b[i].znach_n = 1;
				table_comands_b[i].code_op = 0x3C;
				table_comands_b[i].znach = *calc_ & 0xff;
			}else{
				(*err_table)[i] = 1;
			}
			delete calc_;return 2;
		}else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].\
		First.position-1].Lexem == "EAX" && *calc_ > 255){
			table_comands_b[i].znach_n = 4;
			table_comands_b[i].code_op = 0x3D;
			table_comands_b[i].znach = *calc_ & 0xff;
			delete calc_;return 5;
		}else if(Lt_from_LA[i].example[(*tmp_str_strs)[i].\
		First.position-1].Type == "reg8"){
			if(*calc_ >= -255 && *calc_ <= 255){
				table_comands_b[i].znach_n = 1;
				table_comands_b[i].modrm_n = 1;
				table_comands_b[i].znach = *calc_ & 0xff;
				table_comands_b[i].modrm = 0xF8 \
				+ ad_int_nE[Lt_from_LA[i].example[\
				(*tmp_str_strs)[i].First.position-1].Lexem];
				table_comands_b[i].code_op = 0x80;
				delete calc_;return 3;
			}else{
				delete calc_;
				(*err_table)[i] = 1;
				return -1;
			}
		}else if(Lt_from_LA[i].example[\
		(*tmp_str_strs)[i].First.position-1].Type == "reg32"){
			if(*calc_ >= -255 && *calc_ <= 255){
				table_comands_b[i].znach_n = 1;
				table_comands_b[i].modrm_n = 1;
				table_comands_b[i].znach = *calc_ & 0xff;
				table_comands_b[i].modrm = 0xF8 \
				+ ad_int_E[Lt_from_LA[i].example[\
				(*tmp_str_strs)[i].First.position-1].Lexem];
				table_comands_b[i].code_op = 0x83;
				delete calc_;return 3;
			}else{
				table_comands_b[i].znach_n = 4;
				table_comands_b[i].modrm_n = 1;
				table_comands_b[i].znach = *calc_ & 0xffffffff;
				table_comands_b[i].modrm = 0xF8 \
				+ ad_int_E[Lt_from_LA[i].example[\
				(*tmp_str_strs)[i].First.position-1].Lexem];
				table_comands_b[i].code_op = 0x81;
				delete calc_;return 6;
			}
		}else{
			delete calc_;
			(*err_table)[i] = 1;
			return -1;
		}
	}
	(*err_table)[i] = 1;
	return -1;
}

int gramar::mem_op_func(int op, int i, int r){
	std::map <std::string, int> c_seg;
		c_seg[segment_reg[0]] = 0x2E;c_seg[segment_reg[1]] = 0x36;
		c_seg[segment_reg[2]] = 0x3E;c_seg[segment_reg[3]] = 0x26;
		c_seg[segment_reg[4]] = 0x64;c_seg[segment_reg[5]] = 0x65;
	int beg_op; int find_m = -1; int numb;
	int siz;
	if(op == 1){
		beg_op = (*tmp_str_strs)[i].First.position - 1;
		numb = (*tmp_str_strs)[i].First.numb;
	}else if(op == 2){
		beg_op = (*tmp_str_strs)[i].Second.position - 1;
		numb = (*tmp_str_strs)[i].Second.numb;
	}else{
		(*err_table)[i] = 1;
		return -1;
	}
	table_comands_b[i].modrm_n = 1;
	table_comands_b[i].shift_n = 4;
	if(numb == 4){
		if(Lt_from_LA[i].example[beg_op].Type == "unknown"\
		&& Lt_from_LA[i].example[beg_op + 1].Lexem == "["\
		&& Lt_from_LA[i].example[beg_op + 2].Type == "reg32"\
		&& Lt_from_LA[i].example[beg_op + 3].Lexem == "]"){
			find_m = find_mem(&(Lt_from_LA[i].example[beg_op].Lexem));
			if(find_m != -1){
				siz = 5;
				if(!segmants_error){
					if(find_m > s_table[_code_pos].b && find_m < s_table[_code_pos].e){
						table_comands_b[i].pref_c_seg_n = 1;
						table_comands_b[i].pref_c_seg = 0x2E;
						siz++;
					}else if(find_m < s_table[_data_pos].b || find_m > s_table[_data_pos].e){
						(*err_table)[i] = 1; siz = -1;
					}
				}
				table_comands_b[i].shift = find_m;
				if(r == 0){
					if(identify_t_sh[find_m] == 4)
						table_comands_b[i].code_op++;
					table_comands_b[i].modrm = 0x80 + ad_int_E[Lt_from_LA[i].example[beg_op + 2].Lexem];
					if(ad_int_E[Lt_from_LA[i].example[beg_op + 2].Lexem] == 0x84){
						table_comands_b[i].sib = 0x24;
						table_comands_b[i].sib_n = 1;
					}
				}
				if(r == 1){
					if(identify_t_sh[find_m] == 1)
						table_comands_b[i].modrm += ad_int_E[Lt_from_LA[i].example[beg_op + 2].Lexem];
					else
						(*err_table)[i] = 1;
				}if(r == 4){
					if(identify_t_sh[find_m] == 4)
						table_comands_b[i].modrm += ad_int_E[Lt_from_LA[i].example[beg_op + 2].Lexem];
					else
						(*err_table)[i] = 1;
				}
				return siz;
			}
		}
	}
	else if(numb == 6){
		if(Lt_from_LA[i].example[beg_op].Type == "unknown"\
		&& Lt_from_LA[i].example[beg_op + 1].Lexem == "["\
		&& Lt_from_LA[i].example[beg_op + 2].Type == "reg32"\
		&& Lt_from_LA[i].example[beg_op + 3].Lexem == "*"\
		&&(Lt_from_LA[i].example[beg_op + 4].Lexem == "2" ||\
		   Lt_from_LA[i].example[beg_op + 4].Lexem == "4" ||\
		   Lt_from_LA[i].example[beg_op + 4].Lexem == "8" )\
		&& Lt_from_LA[i].example[beg_op + 5].Lexem == "]"){
			find_m = find_mem(&(Lt_from_LA[i].example[beg_op].Lexem));
			table_comands_b[i].sib_n = 1;
			if(find_m != -1)
				{siz = 6;
				if(!segmants_error){
					if(find_m > s_table[_code_pos].b && find_m < s_table[_code_pos].e){
						table_comands_b[i].pref_c_seg_n = 1;
						table_comands_b[i].pref_c_seg = 0x2E;
						siz++;
					}else if(find_m < s_table[_data_pos].b || find_m > s_table[_data_pos].e){
						(*err_table)[i] = 1; siz = -1;
					}
				}
				table_comands_b[i].shift = find_m;
				int k = 0;
				if(Lt_from_LA[i].example[beg_op + 4].Lexem == "2") k = 0x45;
				if(Lt_from_LA[i].example[beg_op + 4].Lexem == "4") k = 0x85;
				if(Lt_from_LA[i].example[beg_op + 4].Lexem == "8") k = 0xC5;
				if(Lt_from_LA[i].example[beg_op + 2].Lexem == "ESP") (*err_table)[i] = 1;
				else table_comands_b[i].sib = k + 8*ad_int_E[Lt_from_LA[i].example[beg_op + 2].Lexem];
				if(r == 0){
					if(identify_t_sh[find_m] == 4)
						table_comands_b[i].code_op++;
					table_comands_b[i].modrm = 0x04;
				}
				if(r == 1){
					if(identify_t_sh[find_m] != 1)
						(*err_table)[i] = 1;
				}if(r == 4){
					if(identify_t_sh[find_m] != 4)
						(*err_table)[i] = 1;
				}
				return siz;
			}
		}
		else if(Lt_from_LA[i].example[beg_op].Type == "segment_reg"\
		&& Lt_from_LA[i].example[beg_op+1].Lexem == ":"\
		&& Lt_from_LA[i].example[beg_op+2].Type == "unknown"\
		&& Lt_from_LA[i].example[beg_op + 3].Lexem == "["\
		&& Lt_from_LA[i].example[beg_op + 4].Type == "reg32"\
		&& Lt_from_LA[i].example[beg_op + 5].Lexem == "]"){
			find_m = find_mem(&(Lt_from_LA[i].example[beg_op+2].Lexem));
			if(find_m != -1){
				siz = 6;
				if(!segmants_error)
				if((find_m < s_table[_data_pos].b || find_m > s_table[_data_pos].e)\
				&& (find_m < s_table[_code_pos].b && find_m > s_table[_code_pos].e)){
					(*err_table)[i] = 1; siz = -1;
				}
				table_comands_b[i].shift = find_m;
				table_comands_b[i].pref_c_seg = c_seg[Lt_from_LA[i].example[beg_op].Lexem];
				table_comands_b[i].pref_c_seg_n = 1;
				if(r == 0){
					if(identify_t_sh[find_m] == 4)
						table_comands_b[i].code_op++;
					table_comands_b[i].modrm = 0x80 + ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem];
					if(ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem] == 0x84){
						table_comands_b[i].sib = 0x24;
						table_comands_b[i].sib_n = 1;
					}
				}
				if(r == 1){
					if(identify_t_sh[find_m] == 1)
						table_comands_b[i].modrm += ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem];
					else
						(*err_table)[i] = 1;
				}if(r == 4){
					if(identify_t_sh[find_m] == 4)
						table_comands_b[i].modrm += ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem];
					else
						(*err_table)[i] = 1;
				}
				return siz;
			}
		}
	}
	else if(numb == 8){
		if(Lt_from_LA[i].example[beg_op].Type == "segment_reg"\
		&& Lt_from_LA[i].example[beg_op+1].Lexem == ":"\
		&& Lt_from_LA[i].example[beg_op+2].Type == "unknown"\
		&& Lt_from_LA[i].example[beg_op + 3].Lexem == "["\
		&& Lt_from_LA[i].example[beg_op + 4].Type == "reg32"\
		&& Lt_from_LA[i].example[beg_op + 5].Lexem == "*"\
		&&(Lt_from_LA[i].example[beg_op + 6].Lexem == "2" ||\
		   Lt_from_LA[i].example[beg_op + 6].Lexem == "4" ||\
		   Lt_from_LA[i].example[beg_op + 6].Lexem == "8" )\
		&& Lt_from_LA[i].example[beg_op + 7].Lexem == "]"){
			table_comands_b[i].sib_n = 1;
			find_m = find_mem(&(Lt_from_LA[i].example[beg_op+2].Lexem));
			if(find_m != -1)
				{siz = 7;
				if(!segmants_error)
				if((find_m < s_table[_data_pos].b || find_m > s_table[_data_pos].e)\
				&& (find_m < s_table[_code_pos].b && find_m > s_table[_code_pos].e)){
					(*err_table)[i] = 1; siz = -1;
				}
				table_comands_b[i].shift = find_m;
				table_comands_b[i].pref_c_seg = c_seg[Lt_from_LA[i].example[beg_op].Lexem];
				table_comands_b[i].pref_c_seg_n = 1;
				int k = 0;
				if(Lt_from_LA[i].example[beg_op + 6].Lexem == "2") k = 0x45;
				if(Lt_from_LA[i].example[beg_op + 6].Lexem == "4") k = 0x85;
				if(Lt_from_LA[i].example[beg_op + 6].Lexem == "8") k = 0xC5;
				if(Lt_from_LA[i].example[beg_op + 4].Lexem == "ESP") (*err_table)[i] = 1;
				else table_comands_b[i].sib = k + 8*ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem];
				if(r == 0){
					if(identify_t_sh[find_m] == 4)
						table_comands_b[i].code_op++;
					table_comands_b[i].modrm = 0x04;
				}
				if(r == 1){
					if(identify_t_sh[find_m] == 1)
						if(Lt_from_LA[i].example[beg_op + 4].Lexem == "ESP") (*err_table)[i] = 1;
						else table_comands_b[i].sib = k + 8*ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem];
					else
						(*err_table)[i] = 1;
				}if(r == 4){
					if(identify_t_sh[find_m] == 4)
						if(Lt_from_LA[i].example[beg_op + 4].Lexem == "ESP") (*err_table)[i] = 1;
						else table_comands_b[i].sib = k + 8*ad_int_E[Lt_from_LA[i].example[beg_op + 4].Lexem];
					else
						(*err_table)[i] = 1;
				}
				return siz;
			}
		}
	}
	(*err_table)[i] = 1;
	return -1;
}

long long gramar::find_mem(std::string *s){
	long long f  = -1;
	for(int i = 0; i < (int)lines; i++){
		if(identify_t_sh[i]>0)
			if(Lt_from_LA[i].example[0].Lexem == *s)
				{f = i; break;}
	}
	return f;
}

long long* gramar::calculator(int op, int i){
	long long* ret;
	std::stack<char>		operation;
	std::stack<long long>	simbol;
	std::map<char,int>		priority;
	priority['('] =  0;priority[')'] =  1;priority['+'] =  2;
	priority['-'] =  2;priority['*'] =  3;priority['/'] =  3;
	//priority['^'] =  4;
	std::string	formula,	number;
	char		c,			si;
	int			ii,			end_f;
	long long	a,			b;
	int			beg,		end_op;
	
	if(op == 1){
		beg = (*tmp_str_strs)[i].First.position - 1;
		end_op = beg + (*tmp_str_strs)[i].First.numb;
	}else if(op == 2){
		beg = (*tmp_str_strs)[i].Second.position - 1;
		end_op = beg + (*tmp_str_strs)[i].Second.numb;
	}else{
		(*err_table)[i] = 1;
		return NULL;
	}
	for(ii = beg; ii < end_op; ii++){
		std::ostringstream stm ;
		int abc;
		if(Lt_from_LA[i].example[ii].Type == "Bin")
			{abc = strtol(Lt_from_LA[i].example[ii].Lexem.c_str(),0,2);
			stm << abc;formula += stm.str();}
		else if(Lt_from_LA[i].example[ii].Type == "Dec")
			{abc = strtol(Lt_from_LA[i].example[ii].Lexem.c_str(),0,10);
			stm << abc;formula += stm.str();}
		else if(Lt_from_LA[i].example[ii].Type == "Hex")
			{abc = strtol(Lt_from_LA[i].example[ii].Lexem.c_str(),0,16);
			stm << abc;formula += stm.str();}
		else if(Lt_from_LA[i].example[ii].Type == "math_simbol")
			{formula += Lt_from_LA[i].example[ii].Lexem;}
		else
			{(*err_table)[i] = 1; return NULL;}
	}
	//знищення унарних плюсів і мінусів//////////
	while(-1 != (i = formula.find("(-"))){
		formula.insert(i+1,"0");
	}
	while(-1 != (i = formula.find("--"))){
		formula.erase(i,i+2);
		formula.insert(i,"+");
	}
	if('-' == formula.at(0))
		formula.insert(0,"0");
	while(-1 != (i = formula.find("(+"))){
		formula.insert(i+1,"0");
	}
	if('+' == formula.at(0))
		formula.insert(0,"0");
	number.erase();
	//обчислення в секах/////////////////////////////////////////////////////////////////
	for(i=0,end_f=formula.length();i<end_f;i++){		//поки рядок не закінчився///////
		c = formula.at(i);								//зчитуємо символ////////////////
		if(c == '('||c == ')'||c == '+'||c == '-'||c == '*'||c == '/'/*||c == '^'*/){
														//якщо-операція//////////////////
			if(!number.empty()){						//рядок не пустий////////////////
				simbol.push(strtod(number.c_str(),0));	//записати число в стек//////////
				number.erase();							//очищення рядка для чисел///////
			}
			/////////////////////////////////////////////////////////////////////////////
			
			if(!operation.empty()){						//стек операцій не пустий////////
				if(priority[operation.top()]>=priority[c] && c !='('){//пріоритет менший/
					if(c == ')'){						//не дорівнює '(', але == ')'////
						while('(' != operation.top()){	//тоді, згорнути дужку до числа//
							b = simbol.top(); simbol.pop();
							a = simbol.top(); simbol.pop();
							si = operation.top(); operation.pop();
							simbol.push(func_operation(a,b,si));
						}
						operation.pop();				//і вивільнити '(' //////////////
					}else{								//не == ')' -> згортаємо 2 числа/
					b = simbol.top(); simbol.pop();
					a = simbol.top(); simbol.pop();
					si = operation.top(); operation.pop();
					simbol.push(func_operation(a,b,si));
					i--;								//повертаємося, щоб перевірити///
					}									//як змінився пріоритет операцій/
				}else{									//пріоритет більший, або '('/////
					if(c == ')'){						//згортаємо дужки. Пр.: ((А)).../
						operation.pop();
					}else{								//заштовхуємо операцію///////////
						operation.push(c);
					}
				}
			}else{										//стек операцій пусий////////////
				operation.push(c);						//записати в стек операцію///////
			}
		}else{											//якщо не операція, а число//////
			number += c;									//зчитування числа///////////
			if((!number.empty()) && i == end_f - 1){		//якщо кінець рядка формули//
				simbol.push(strtod(number.c_str(),0));		//запис числа в стек/////////
				number.erase();								//очистити рядок для чисел///
			}
		}
	}
	while(!operation.empty()){
		b = simbol.top(); simbol.pop();
		a = simbol.top(); simbol.pop();
		si = operation.top(); operation.pop();
		simbol.push(func_operation(a,b,si));
	}
	ret = new long long;
	*(ret) = simbol.top();
	return ret;
}

long long gramar::func_operation(long long a, long long b, char c){
	switch (c){
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;
		default:  return 0;
		}
}

int gramar::jnz_func(int i){
	if((*tmp_str_strs)[i].Second.numb != 0){
		(*err_table)[i] = 1;
		return -1;
	}
	if((*tmp_str_strs)[i].First.numb == 1){
		table_comands_b[i].code_op_n = 2;
		if(Lt_from_LA[i].example[(*tmp_str_strs)[i].First.position - 1].Lexem == "$"){
			table_comands_b[i].code_op = 0x75FE;
			return 2;
		}else{
			table_comands_b[i].code_op = 0x0F85;
			table_comands_b[i].shift = find_label(&(Lt_from_LA[i].example[1].Lexem));
			table_comands_b[i].shift_n = 4;
			return 6;
		}
	}else{
		(*err_table)[i] = 1;
		return -1;
	}
}

long long gramar::find_label(std::string* st){
	long long f  = -1;
	for(int i = s_table[_code_pos].b + 1; i < s_table[_code_pos].e; i++){
		if(identify_t_sh[i]==-1)
			if(Lt_from_LA[i].example[0].Lexem == *st)
				{f = i; break;}
	}
	return f;
}