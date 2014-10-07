#include <stdio.h>
#include "error_count.h"
#include "parser.h"

const unsigned char* test_text = (unsigned char*)"истина !|| ((ложь !&& истина) && (10 = 0xA))";

int main(int argc, char** argv) {
	freopen ("myfile.txt","w",stdout);
	Error_count ec; 
	Code_buffer buffer;
	
	Parser syntax_analyzer(test_text, &ec, &buffer);
	
	syntax_analyzer.parse();
	
	ec.print_number_of_errors();
	fclose (stdout);		
	return 0;
}
