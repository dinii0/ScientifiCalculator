#include "framework.h"
#include "DynamicNumbers.h"
#include "CalculateButtons.h"

#include <stack>
#include <unordered_map>

using namespace std;

vector<BUTTONS_ACRONYMS_CATEGORY> buttons_acronyms_list;
vector<string> buttons_numbers_values;

bool is_string_number(string text) {
    int i = text[0] == '-' ? 1 : 0;

    if (i == 1 && text.length() < 2)
        return false;

    for (/* Oppss */; i < text.length(); i++) {
        if (!isdigit(text[i]) && text[i] != '.')
            return false;
    }

    return true;
}

unordered_map<string, char> create_button_characters_map() {
    unordered_map<string, char> buttonCharMap;
    buttonCharMap["Module"] = '%';
    buttonCharMap["Division"] = '/';
    buttonCharMap["Multiplying"] = '*';
    buttonCharMap["Substraction"] = '-';
    buttonCharMap["Opening bracket"] = '(';
    buttonCharMap["Closing bracket"] = ')';
    buttonCharMap["Addition"] = '+';
    buttonCharMap["Dot"] = '.';
    return buttonCharMap;
}
void read_initiliased_buttons() {
    unordered_map<string, char> button_characters_map = create_button_characters_map();
    string buffer = "";

    for (int i = 0; i < BUTTONS_CALCULATE_TOTAL; i++) {
        if (buttons_calculator_list[i].variable_counter == 0) {
            BUTTONS_ACRONYMS_CATEGORY buffer_object;
            buffer_object.button_index = i;

            if (buttons_calculator_list[i].button_name.substr(0, 7) == "Number:") {
                buffer = buttons_calculator_list[i].button_name.substr(8);
                buffer_object.button_character = buffer[0];
            }
            else if (button_characters_map.find(buttons_calculator_list[i].button_name) != button_characters_map.end())
                buffer_object.button_character = button_characters_map[buttons_calculator_list[i].button_name];

            buttons_acronyms_list.push_back(buffer_object);
        }
    }
}
char get_button_element_symbol(int index) {
    for (const auto& button : buttons_acronyms_list) {
        if (button.button_index == index)
            return button.button_character;
    }

    return '\0';
}
int get_button_element_index(char symbol) {
    for (const auto& button : buttons_acronyms_list) {
        if (button.button_character == symbol)
            return button.button_index;
    }

    return -1;
}

int get_arithmetic_procedure(const string& operation) {
    switch (operation[0]) {
        case '*':
        case '/':
        case '%': {
            return 2;
        }
        case '+':
        case '-': {
            return 1;
        }
    }

    return 0;
}
double set_arithmetic_value(double first_value, const string& arithmetic_symbol, double second_value) {
    switch (arithmetic_symbol[0]) {
        case '+': return first_value + second_value;
        case '-': return first_value - second_value;
        case '*': return first_value * second_value;
        case '/': return first_value / second_value;
        case '%': return (first_value / 100) * second_value;
    }

    return 0.0;
}
double get_total_calculation(const vector<string>& expression) {
    stack<double> numbers;
    stack<string> operators;

    for (const string& token : expression) {
        if (token == "(") operators.push(token);
        else if (token == ")") {

            while (!operators.empty() && operators.top() != "(") {
                double second_value = numbers.top();
                numbers.pop();

                double first_value = numbers.top();
                numbers.pop();

                string arithmetic_operation = operators.top();
                operators.pop();

                numbers.push(set_arithmetic_value(first_value, arithmetic_operation, second_value));
            }
            operators.pop();
        }
        else if (!is_character_operator(token)) numbers.push(stod(token));
        else {

            while (!operators.empty() && is_character_operator(operators.top()) && get_arithmetic_procedure(token) <= get_arithmetic_procedure(operators.top())) {
                double second_value = numbers.top();
                numbers.pop();

                double first_value = numbers.top();
                numbers.pop();

                string arithmetic_operation = operators.top();
                operators.pop();

                numbers.push(set_arithmetic_value(first_value, arithmetic_operation, second_value));
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        double second_value = numbers.top();
        numbers.pop();

        double first_value = numbers.top();
        numbers.pop();

        string arithmetic_operation = operators.top();
        operators.pop();

        numbers.push(set_arithmetic_value(first_value, arithmetic_operation, second_value));
    }
    
    return numbers.top();
}