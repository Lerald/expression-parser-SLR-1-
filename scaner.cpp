#include <stdio.h>
#include "scaner.h"

inline int hex_digit_to_int(int digit)
{
    int temp = digit - '0';
    return (temp<=9) ? temp : ((temp&0xDF) - 7);
}

static const char categories_table[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 6, 9, 9, 9, 9, 6, 9, 5, 5, 6, 5, 9, 5, 9, 5,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 9, 6, 5, 6, 5,
  9, 4, 4, 4, 4, 4, 4, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 3, 9, 9, 9, 9, 9, 6, 9,
  9, 4, 4, 4, 4, 4, 4, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 3, 9, 9, 9, 6, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  8, 9, 9, 9, 9, 9, 8, 9, 7, 9, 9, 7, 9, 8, 8, 9,
  9, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 9, 9, 9
};

static const char* categories_name[] = {
	"SPACES"    	, "ZERO_DIGIT",	          "DIGIT_WITHOUT_ZERO"          ,  "LETTERS_X", 
	"HEX_DIGIT" 	, "ONE_LETTER_DELIMITER", "MULTILETTER_DELIMITER_BEGIN" ,  "KEYWORD_BEGIN"  ,    
	"KEYWORD_BODY"	, "OTHER" 
};

static const char* automata_names[] = {
	"a_START", "a_UNKNOWN", "a_DELIMITERS", "a_KEYWORD", "a_NUMBER"
};

Scaner::Scaner(const unsigned char* t, Error_count* e){
	text = pcurrent_char = lexem_begin = const_cast<unsigned char*>(t);
	current_line = 1;
	en = e;
	Lexem.number = 0;
	return;
}

unsigned int Scaner::get_line_number(){
	return current_line;
}

void Scaner::back(){
	pcurrent_char = lexem_begin; 
	return;
}

Scaner::Automaton_proc Scaner::procs[] = {
     &Scaner::start_proc     , &Scaner::unknown_proc, 
	 &Scaner::delimiters_proc, &Scaner::keyword_proc,
	 &Scaner::number_proc
};

/* Подпрограмма поиска символа в массиве из 4 символов. Если символ в массиве есть,
   то возвращается индекс соответствующего элемента. Иначе возвращается число 4.
   Искомый символ не должен быть равен нулю. */
int search_char(unsigned char c, char array[4]){
	unsigned int x = (c * 0x01010101) ^ (*((int*)array)); 
	/* Далее в полученном четырёхбайтовом целом числе вычисляем номер самого младшего
       нулевого байта и возвращаем этот номер. */
    /* Поиск этот будем осуществлять так, как предложено на стр.102 книги
     "Уоррен Г.С. Алгоритмические трюки для программистов.: Пер. с англ. --- М.: 
	  Вильямс, 2003." */
	unsigned int y = (x & 0x7f7f7f7f) + 0x7f7f7f7f;
	y = y | x;
	
	unsigned int t1, t2, t3, t4;
	t1 = (y >> 7)  & 1;
	t2 = (y >> 15) & t1;
	t3 = (y >> 23) & t2;
	t4 = (y >> 31) & t3;
    return t1 + t2 + t3 + t4;
}

typedef struct tag_ELEM{
	unsigned short int code;
    unsigned short int first_state;
    char               symbols[4];
} ELEM;


bool Scaner::unknown_proc(){
	return char_category == OTHER;
}

bool Scaner::start_proc(){
	bool t = true;
	Lexem.number = 0;
	switch(char_category){
		case SPACES:
			if(ch == '\n'){
				current_line++;
			}
			break;
			
		case OTHER: case KEYWORD_BODY:
			Automaton = a_UNKNOWN;
			Lexem.probably_code = UNKNOWN;
			printf("Нераспознаваемая лексема в строке %d.\n", current_line);
			en->increment_number_of_errors();
			break;
			
		case ONE_LETTER_DELIMITER:
			switch((char)ch){
				case '?':
					Lexem.probably_code = CONDITION;
					break;
					
				case ':':
					Lexem.probably_code = COLON;
					break;
					
				case '=':
					Lexem.probably_code = EQ;
					break;
					
				case '+':
					Lexem.probably_code = PLUS;
					break;
					
				case '-':
					Lexem.probably_code = MINUS;
					break;
					
				case '/':
					Lexem.probably_code = DIV;
					break;
					
				case '(':
					Lexem.probably_code = OPENED_BRACKET;
					break;
					
				case ')':
					Lexem.probably_code = CLOSED_BRACKET;
					break;
			}
			pcurrent_char++;
			t = false;
			break;
			
		case MULTILETTER_DELIMITER_BEGIN:
			Automaton = a_DELIMITERS;
			switch((char)ch){
				case '!':
					state = 0;
					break;
					
				case '|':
					state = 6;
					break;
					
				case '^':
					state = 8;
					break;
					
				case '&':
					state = 10;
					break;
				
				case '<':
					state = 12;
					break;
					
				case '>':
					state = 14;
					break;
					
				case '*':
					state = 16;
					break;
			}	
			lexem_begin = pcurrent_char - 1;		
			break;
						
		case KEYWORD_BEGIN:
			state = (((char)ch) == 'л')? 6: 0;
			Automaton = a_KEYWORD;
			lexem_begin = pcurrent_char - 1;
			break;	
			
		case ZERO_DIGIT: case DIGIT_WITHOUT_ZERO:
			state = char_category & 1;
            Lexem.number = ch - '0';
			Lexem.probably_code = t_INT; 
			Automaton = a_NUMBER;
			lexem_begin = pcurrent_char-1;
			break;	
		
	}
	return t;
}

ELEM a_DELIMITERS_jump_table[] = {
	{NOT	     , 1 , {'|','&','=',0} }, 	/*состояние 0   $ !   */
	{MAYBE_NOR   , 4 , {'|',0,0,0} }, 		/*состояние 1   $ !|...   */
	{MAYBE_NAND  , 5 , {'&',0,0,0} }, 		/*состояние 2   $ !&...   */
	{NEQ    	 , 0 , {0,0,0,0} }, 		/*состояние 3   $ !=   */
	{NOR		 , 0 , {0,0,0,0} }, 		/*состояние 4   $ !||   */
	{NAND		 , 0 , {0,0,0,0} }, 		/*состояние 5   $ !&&   */
	
	{MAYBE_OR  	 , 7 , {'|',0,0,0} }, 		/*состояние 6   $ |...   */
	{OR  		 , 0 , {0,0,0,0} }, 		/*состояние 7   $ ||   */
	
	{MAYBE_XOR  , 9 , {'^',0,0,0} }, 		/*состояние 8   $ ^...   */
	{XOR  		, 0 , {0,0,0,0} }, 			/*состояние 9   $ ^^   */
		
	{MAYBE_AND , 11 , {'&',0,0,0} }, 		/*состояние 10   $ &...   */	
	{AND       , 0 ,  {0,0,0,0}   }, 		/*состояние 11   $ &&     */	
	
	{LT		   , 13 , {'=',0,0,0} }, 		/*состояние 12   $ <...   */	
	{LEQ       , 0 ,  {0,0,0,0}   }, 		/*состояние 13   $ <=     */	
	
	{GT		   , 15 , {'=',0,0,0} }, 		/*состояние 14   $ >...   */	
	{GEQ       , 0 ,  {0,0,0,0}   }, 		/*состояние 15   $ >=     */	

	{MULT	   , 17 , {'*',0,0,0} }, 		/*состояние 16   $ *...   */	
	{DEGREE    , 0 ,  {0,0,0,0}   }, 		/*состояние 13   $ **     */	
};

bool Scaner::delimiters_proc(){
	bool t = false;
	Lexem.probably_code = static_cast<LEXEM_CODE>(a_DELIMITERS_jump_table[state].code);
	int y = search_char(ch, a_DELIMITERS_jump_table[state].symbols);
	if(y != 4){
		state = a_DELIMITERS_jump_table[state].first_state + y;
		t = true;
	}
	return t;
}


ELEM a_KEYWORD_jump_table[] = {
	{UNKNOWN  , 1, {'с',0,0,0}}, /*состояние 0 $ и...    */ 
	{UNKNOWN  , 2, {'т',0,0,0}}, /*состояние 1 $ иc...   */
	{UNKNOWN  , 3, {'и',0,0,0}}, /*состояние 2 $ ист...  */
	{UNKNOWN  , 4, {'н',0,0,0}}, /*состояние 3 $ исти... */
	{UNKNOWN  , 5, {'а',0,0,0}}, /*состояние 4 $ истин...*/
	{kw_ISTINA, 0, {0,0,0,0}  }, /*состояние 5 $ истина  */
	
	{UNKNOWN  , 7, {'о',0,0,0}}, /*состояние 6 $ л...    */ 
	{UNKNOWN  , 8, {'ж',0,0,0}}, /*состояние 7 $ ло...   */
	{UNKNOWN  , 9, {'ь',0,0,0}}, /*состояние 8 $ лож...  */
	{kw_LOZH  , 0, {0,0,0,0}  }  /*состояние 9 $ ложь    */
};

bool Scaner::keyword_proc(){
	bool t = false;
	Lexem.probably_code = static_cast<LEXEM_CODE>(a_KEYWORD_jump_table[state].code);	
	if( (1<<char_category) & ( (1<<KEYWORD_BEGIN) | (1<<KEYWORD_BODY) ) ){
		int y = search_char(ch, a_KEYWORD_jump_table[state].symbols);		
		if(y != 4){
			state = a_KEYWORD_jump_table[state].first_state + y;
			t = true;
		}
	}
	return t;
}

static const char* op_as_string[] = {
	"", "", "", "!||", "||", "^^", "!&&", "&&"
};

void Scaner::diagnostic_for_delimiters(){
	if(Lexem.probably_code > UNKNOWN){
		Lexem.probably_code = static_cast<LEXEM_CODE>(Lexem.probably_code - MAYBE_NOR + NOR);
		printf("В строке %d ожидается операция %s\n", current_line, 
		       op_as_string[Lexem.probably_code]);
		en->increment_number_of_errors();
	}
	return;
}

bool Scaner::number_proc(){
	bool t = true;
	switch(state){
		case DEC_INT:
			if( (1<< char_category) & DEC_SET )
            {
                Lexem.number = Lexem.number * 10 + (ch - '0');
                if(Lexem.number > MAX_NUMBER)
                {
                    printf("Слишком большое целое число в строке %d.\n", current_line);
                    en->increment_number_of_errors();
                    state = OUT_OF_RANGE_DEC;
                }
            }
            else
            {
                t = false;
			}
			break;
			
		case MB_HEX:
			if( (1<<char_category) & DEC_SET)
            {
                state = DEC_INT;
                Lexem.number = ch - '0';
            }
            else if( char_category == LETTERS_X)
            {
                state = HEX_START;
            }
            else
            {
                t = false;
            }
			break;

		case HEX_START:
			switch(char_category){
                case ZERO_DIGIT: case HEX_DIGIT: case DIGIT_WITHOUT_ZERO:
                    Lexem.number = Lexem.number * 16 + hex_digit_to_int(ch);
                    state = HEX;
                	break;

                default:
                    printf("В строке %d ожидается шестнадцатиричная цифра.\n", current_line);
                    en->increment_number_of_errors();
                    Lexem.number = 0;
                    t = false;
			}
			break;

		case HEX:
			if( Lexem.number > MAX_NUMBER )
			{
				printf("Слишком большое целое число в строке %d.\n", current_line);
				en->increment_number_of_errors();
				state = OUT_OF_RANGE_HEX;
			}
			else
			{
				if( (1<<char_category) & HEX_SET)
				{
					Lexem.number = Lexem.number * 16 + hex_digit_to_int(ch);
				}
				else
				{
				    t = false;
				}
			}
			break;
				
		case OUT_OF_RANGE_DEC:
			if( !((1<<char_category) & DEC_SET) )
			{
				t = false;
			}
			break;
				
		case OUT_OF_RANGE_HEX:
			if( !((1<<char_category) & HEX_SET) )
			{
				t = false;
			}
			break;
	}
	return t;
}

returned_lexem Scaner::get_current_lexem(){
	Automaton = a_START;
	Lexem.probably_code = NONE;
	lexem_begin = pcurrent_char;
	state = 0;
	
	bool t = true;
	
	while(ch = *pcurrent_char++){
		char_category = static_cast<CATEGORY>(categories_table[ch]);
		t = (this->*procs[Automaton])();

		if(!t){
			pcurrent_char--; 
			if(Automaton == a_DELIMITERS){
				diagnostic_for_delimiters();
			}
			return Lexem;
		}
	}
    pcurrent_char--;
	switch(Automaton){
		case a_KEYWORD:
			Lexem.probably_code = static_cast<LEXEM_CODE>(a_KEYWORD_jump_table[state].code);
			break;
			
		case a_DELIMITERS:
			Lexem.probably_code = static_cast<LEXEM_CODE>(a_DELIMITERS_jump_table[state].code);
			diagnostic_for_delimiters();
			break;
		
		case a_NUMBER:
			switch(state){
				case HEX_START:
					if( !((1<<char_category) & HEX_SET) )
					{

                        	printf("В строке %d ожидается шестнадцатиричная цифра.\n", current_line);
                        	en->increment_number_of_errors();
                        	Lexem.number = 0;
					}
					break;
			}
			break;
			
		default:
			;
	}
	return Lexem; 
}

