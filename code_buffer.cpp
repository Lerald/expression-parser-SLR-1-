#include <stdio.h>
#include "code_buffer.h"
#include <stdlib.h>

static const char* operation_names_as_strings[]={
	"op_NOR",			"op_OR",					"op_XOR", 
	"op_NAND",			"op_AND",					"op_NOT", 
	"op_EQ",			"op_NEQ",					"op_UNSIGN_LS",
	"op_UNSIGN_GT",		"op_UNSIGN_LEQ",			"op_UNSIGN_GEQ",
	"op_SIGN_LT",		"op_SIGN_GT",				"op_SIGN_LEQ",
	"op_SIGN_GEQ",		"op_ADD",					"op_SUB",
	"op_MULT",			"op_DIV",					"op_CHANGE_SIGN",
	"op_POWER",			"op_LOAD_INT_CONST",		"op_LOAD_INT_SIGN_CONST",
	"op_LOAD_TRUE",		"op_LOAD_FALSE",			"op_LOG_TO_INT",
	"op_CONDITION",				"op_STOP"	
};

unsigned Code_buffer::append(Intermediate_command* c)
{	
	code.push_back(*c);
	current_operation_index++;
	
	printf("Current intermediate code command: {op=%s, type=%d, %d}\n",
	       operation_names_as_strings[c->op]);
	
	return current_operation_index;	
}

void Code_buffer::writeToFile()
{
	FILE* pFile;
    pFile = fopen("file.mbin", "wb");
    
    int s = current_operation_index * sizeof(Intermediate_command);
    char *p = new char[s];
    char *q = p;
    
    for(int i = 0; i < code.size(); i++)
    {
    	*q = code[i].op;
    	printf("code command: {op=%s, %d}\n",
	       operation_names_as_strings[code[i].op], code[i].number_const);
    	q++;
    	if(code[i].op == op_LOAD_INT_CONST)  
    	{
    		*((__int64*)q) = code[i].number_const;
    		q += sizeof(__int64);
    	}
    }
    
    fwrite(&p, sizeof(char), s, pFile);
    
    fclose(pFile);
}
