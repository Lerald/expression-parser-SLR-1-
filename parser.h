#include "parser_stack.h"
#include "scaner.h"
#include "error_count.h"
#include "code_buffer.h"

#ifndef PARSER_H
#define PARSER_H
enum Action_names {act_ERROR, act_REDUCE, act_SHIFT, act_OK};

struct Action_info {
	short        is_undefined;
	short		 argument;
	Action_names action;			 
};

#define MAXIMAL_LENGTH_OF_RULE 5

#define LOGICAL_TYPE 		1
#define UNSIGN_INT_TYPE 	2

class Parser {
	public:
		void parse();
		
		void diagnostic_for_reduce(unsigned state);
		void diagnostic_for_shift(unsigned state);
		
		Parser(const unsigned char* t, Error_count* e, Code_buffer* cg);
		~Parser();	
	private:
		Parser_stack* 		stack;
		Scaner*        		scan;
		Error_count*   		ec;
		Code_buffer*        gen;
		Action_info         ai;
		returned_lexem 		lc;
		
		Parser_stack_elem  body[MAXIMAL_LENGTH_OF_RULE];
		
		void shift(short shifted_state, short is_undef);
		void reduce(short rule_for_reduce, short is_undef);
};
#endif
