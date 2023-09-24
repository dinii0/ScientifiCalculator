#ifndef TABLENUMBER_H
#define TABLENUMBER_H

#define DIFFERENCIAL_TABLE_SPACE 30

#include <string>

bool is_limit_reached();
void optimise_rect_position(RECT* clientRect, int number);
std::string remove_zero_traces(double result);
RECT read_table_dimensions(RECT screenSize);
void paint_table(HWND hWnd, HDC hdc);

#endif