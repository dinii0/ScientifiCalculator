#ifndef DYNAMICNUMBERS_H
#define DYNAMICNUMBERS_H

#include <vector>
#include <string>

#define is_character_operator(X) (X == "+" || X == "-" || X == "*" || X == "/" || X == "%")

struct BUTTONS_ACRONYMS_CATEGORY {
    char button_character;
    int button_index;
};
extern std::vector<BUTTONS_ACRONYMS_CATEGORY> buttons_acronyms_list;

extern std::vector<std::string> buttons_numbers_values;

bool is_string_number(std::string text);
void read_initiliased_buttons();
char get_button_element_symbol(int index);
int get_button_element_index(char symbol);
double get_total_calculation(const std::vector<std::string>& expression);

#endif