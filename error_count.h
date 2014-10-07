#ifndef ERROR_COUNT_H
#define ERROR_COUNT_H
class Error_count {
	public:
		int get_number_of_errors();		
		void increment_number_of_errors();		
		void print_number_of_errors();	
			
		Error_count() : number_of_errors(0){
		}
				
	private:
		int number_of_errors;
};
#endif
