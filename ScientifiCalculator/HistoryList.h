#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#define HISTORY_LIST_ID 1200
#define HISTORY_FONT_HEIGHT 35

#include <string>

void update_histroy_list(std::string text);
void create_history_list(HWND hWnd, int width, int height);
void paint_history_outerline(HDC hdc);
HBRUSH colorize_history_list(WPARAM wParam);

#endif