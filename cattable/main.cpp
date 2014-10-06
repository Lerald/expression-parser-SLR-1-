#include <stdio.h>

int main(int argc, char** argv) {
	enum char_categories {
							SPACES, 	ZERO_DIGIT,	     DIGIT_WITHOUT_ZERO   , LETTER_X, HEX_DIGIT , ONE_LETTER_DELIMITER, 
							MULTILETTER_DELIMITER_BEGIN,	 KEYWORD_BEGIN, KEYWORD_BODY	, 	OTHER };

    int table[256];

    unsigned char* digits_chars = (unsigned char*)"0123456789";
    
    unsigned char* keyword_begin_chars = (unsigned char*)"ил";	
  	unsigned char* one_letter_delimiters_chars = (unsigned char*)"?:=+-/()";
  	unsigned char* multiletter_delimiter_begin_chars = (unsigned char*)"!|&^<>*";
  	unsigned char* hex_num_chars = (unsigned char*)"ABCDEFabcdef";
	unsigned char* letter_chars = (unsigned char*)"стнаожь";
  	for(int i = 0; i<33; table[i++] = SPACES);
  	for(int i = 33; i<256; table[i++] = OTHER);
  	  	
  	for( ; *digits_chars; table[*digits_chars++] = DIGIT_WITHOUT_ZERO);
	for( ; *hex_num_chars; table[*hex_num_chars++] = HEX_DIGIT);
	
  	for( ; *keyword_begin_chars; table[*keyword_begin_chars++] = KEYWORD_BEGIN);
	for( ; *letter_chars; table[*letter_chars++] = KEYWORD_BODY);
	
  	for( ; *one_letter_delimiters_chars; table[*one_letter_delimiters_chars++] =
																					ONE_LETTER_DELIMITER);
	for( ; *multiletter_delimiter_begin_chars; table[*multiletter_delimiter_begin_chars++] = MULTILETTER_DELIMITER_BEGIN)

table['x'] = LETTER_X;
table['X'] = LETTER_X;
table['0'] = ZERO_DIGIT;

	for(int i = 0; i<256; i+=16){	     
		printf("  %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
	            table[i]   ,  table[i+1] ,  table[i+2] ,  table[i+3] ,
	            table[i+4] ,  table[i+5] ,  table[i+6] ,  table[i+7] ,
	            table[i+8] ,  table[i+9] ,  table[i+10],  table[i+11],
	            table[i+12],  table[i+13],  table[i+14],  table[i+15]);
	}
	return 0;
}



