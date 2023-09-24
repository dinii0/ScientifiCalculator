#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define BUTTONS_CONFIG_CLASS L"AdditionalBox"

#define WINDOW_TEXT_BOX_ID 16500
#define WINDOW_IMPLEMENT_BUTTON 17000

extern std::vector<HWND> text_fields_list;

void get_global_size_address(HWND hWnd, RECT global_screenSize);
int get_num_opened_brackets();
bool is_number_unfinished();
bool is_window_open();
void register_buttons_special_class(HMODULE hInst);
bool build_buttons_window_menu(HMODULE hInst, int index);

#endif
