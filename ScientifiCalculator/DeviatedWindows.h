#ifndef DEVIATEDWINDOWS_H
#define DEVIATEDWINDOWS_H

#include <string>

#define MATRIX_COLOMNS_ID 15012
#define GCF_ROWS_ID 15013

HWND get_input_window_handler();
void destroy_input_windows();
void create_gcf_input_windows(int fields, HWND hWnd);
void create_matrix_input_windows(int rows, HWND hWnd);
void paint_deviated_windows(std::string name_formula, HDC hdc);
void create_deviated_windows(std::string name_formula, HWND hWnd);

#endif
