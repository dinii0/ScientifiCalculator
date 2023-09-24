#ifndef CALCULATEBUTTONS_H
#define CALCULATEBUTTONS_H

#define BUTTONS_CALCULATE_ID 10010

#define BUTTONS_CALCULATE_TOTAL 48
#define BUTTONS_CALCULATE_SIZE 80
#define BUTTONS_CALCULATE_POSITIONX 500
#define BUTTONS_CALCULATE_PIXELS_SEPERATION 12

#include <string>

struct BUTTONS_CALCULATOR_CATEGORY {
    std::string button_name;
    int variable_counter;
};

extern BUTTONS_CALCULATOR_CATEGORY buttons_calculator_list[BUTTONS_CALCULATE_TOTAL];

void create_calculate_buttons(HWND hWnd, HMODULE hInst, int width, int height);

#endif