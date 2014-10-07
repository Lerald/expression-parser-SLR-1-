#ifndef CODE_BUFFER_H
#define CODE_BUFFER_H

#include <vector>
using namespace std;

enum Instruction_name {
	op_NOR,			op_OR,					op_XOR, 
	op_NAND,		op_AND,					op_NOT, 
	op_EQ,			op_NEQ,					op_SIGN_LT,		
	op_SIGN_GT,		op_SIGN_LEQ,			op_SIGN_GEQ,
	op_UNSIGN_LT,	op_UNSIGN_GT,			op_UNSIGN_LEQ,	
	op_UNSIGN_GEQ,	op_ADD,					op_SUB,
	op_MULT,		op_DIV,					op_CHANGE_SIGN,
	op_POWER,		op_LOAD_INT_CONST,		op_LOAD_INT_SIGN_CONST,
	op_LOAD_TRUE,	op_LOAD_FALSE,			op_LOG_TO_INT,
	op_CONDITION,	op_STOP
};

struct Intermediate_command {
	Instruction_name op;
	__int64 number_const;
};

class Code_buffer {
	public:
		Code_buffer() : current_operation_index(0)
		{
			code = vector<Intermediate_command>();
		}

		unsigned append(Intermediate_command* c);
		void writeToFile();
		
	private:
		
		unsigned current_temp_var;
		
		unsigned current_operation_index;
		vector<Intermediate_command> code;
};
#endif


