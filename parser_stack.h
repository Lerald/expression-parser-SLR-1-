#ifndef PARSER_STACK_H
#define PARSER_STACK_H

#include <vector>
using namespace std;

struct Parser_stack_elem {
	short state;
	short is_undefined;
	int   type;
};

class Parser_stack {
	public:
		Parser_stack() : top_index(0){
			s = vector<Parser_stack_elem>();
		};		
		Parser_stack_elem top();
		void pop();
		void push(const Parser_stack_elem& elem);
		void get_elems(Parser_stack_elem result[], size_t number_of_elems);
		void multi_pop(size_t number_of_elems);
		bool empty() const;
		size_t size() const;
		void print_stack() const;
	private:
		vector<Parser_stack_elem> s;
		size_t top_index;
};

#endif

