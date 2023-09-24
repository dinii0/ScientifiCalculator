#ifndef TITLEBAR_H
#define TITLEBAR_H

#define BUTTON_MINIMAZE 17500
#define BUTTON_EXIT 17501

void paint_title_bar(HDC hdc);
void create_bar_buttons(HWND hWnd, int width);
void paint_exit_button(LPDRAWITEMSTRUCT lpdis);

#endif