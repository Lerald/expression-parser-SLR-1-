#include <stdio.h>
#include "parser_stack.h"

bool Parser_stack :: empty() const{
	return top_index == 0;
}

size_t Parser_stack :: size() const{
	return top_index;
}

Parser_stack_elem Parser_stack :: top(){
	return s[top_index-1];
}

void Parser_stack :: pop(){
	s.pop_back();
	if(top_index != 0){top_index--;};
	return;
}

void Parser_stack :: multi_pop(size_t number_of_elems){
	s.erase(s.end() - number_of_elems,  s.end());
	if(top_index >= number_of_elems){
		top_index -= number_of_elems;
	}
	else{
		top_index = 0;
	}
	return;
}

void Parser_stack :: get_elems(Parser_stack_elem result[], size_t number_of_elems){
	if(top_index >= number_of_elems){
		size_t temp = top_index - number_of_elems;
		for(size_t i = temp; i < top_index; i++ ){
			result[i - temp] = s[i];
		}
	}
	return;
}

void Parser_stack :: push(const Parser_stack_elem& elem){
	s.push_back(elem);
	top_index++;
	return;
}

void Parser_stack::print_stack() const{
	printf("%s","States and args: ");
	for(size_t i = 0; i < top_index; i++){
		printf("{state=%d, type=%d}",s[i].state, s[i].type);
	}
	printf("%s","\n");
	return;
}

