#include "error_count.h"
#include <stdio.h>
#include <limits.h>


#ifndef SCANER_H
#define SCANER_H

#define MAX_NUMBER ULLONG_MAX
#define DEC_SET ((1<<ZERO_DIGIT) + (1<<DIGIT_WITHOUT_ZERO))
#define HEX_SET ((1<<ZERO_DIGIT) + (1<<DIGIT_WITHOUT_ZERO) + (1<<HEX_DIGIT))

enum LEXEM_CODE {
	NONE  		 	, CONDITION 	 , COLON     , NOR     	 ,		OR		 	,
	XOR 			, NAND	  		 , AND	  	 , NOT	  	 ,		EQ		 	,
	NEQ			 	, LT  	  		 , GT	  	 ,	LEQ	  	 ,		GEQ		 	,
	PLUS  		 	, MINUS	  		 , MULT	  	 ,	DIV	  	 ,		DEGREE	 	, 
	OPENED_BRACKET  , CLOSED_BRACKET , kw_ISTINA ,	kw_LOZH  ,		t_INT		 ,
	UNKNOWN		 	, MAYBE_NOR		 , MAYBE_OR  , MAYBE_XOR ,		MAYBE_NAND  ,
	MAYBE_AND   	
};

struct returned_lexem{
	LEXEM_CODE probably_code;
	__int128 number;
};

class Scaner {
	public:
		void       back();
		returned_lexem get_current_lexem();
		
		unsigned int get_line_number();
		
		Scaner(const unsigned char* t, Error_count* e);
		
	private:
		enum CATEGORY {
			SPACES, 	ZERO_DIGIT,	     DIGIT_WITHOUT_ZERO   , LETTERS_X, HEX_DIGIT , ONE_LETTER_DELIMITER, 
							MULTILETTER_DELIMITER_BEGIN,	 KEYWORD_BEGIN, KEYWORD_BODY	, 	OTHER 
		};	
		
		enum Automaton_name {
			a_START, a_UNKNOWN, a_DELIMITERS, a_KEYWORD, a_NUMBER
		};
		
		enum a_NUMBER_states { DEC_INT, MB_HEX, HEX_START, HEX, OUT_OF_RANGE_DEC, OUT_OF_RANGE_HEX};
		
		Automaton_name Automaton;
		unsigned int   state;
		
		typedef bool (Scaner::*Automaton_proc)();
		static Automaton_proc procs[];
		
		bool start_proc();
		bool unknown_proc();
		bool delimiters_proc();
		bool keyword_proc();
		bool number_proc();
		
		void diagnostic_for_delimiters();
		
		unsigned char* text;
		unsigned char* pcurrent_char;
		unsigned char* lexem_begin;
		
		unsigned char  ch;
		CATEGORY       char_category;
		
		returned_lexem Lexem;
		
		unsigned int   current_line;
		
		Error_count*   en;	
};

#endif

