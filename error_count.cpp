#include "error_count.h"
#include <stdio.h>

int Error_count::get_number_of_errors(){
	return number_of_errors;
}

void Error_count::increment_number_of_errors(){
	++number_of_errors;
	return;
} 

void Error_count::print_number_of_errors(){
	printf("Всего ошибок: %d",number_of_errors);
	return;
}

