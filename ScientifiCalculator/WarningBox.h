#ifndef WARNINGBOX_H
#define WARNINGBOX_H

#include <string>

void paint_warning_box(HDC hdc, HWND hWnd);
void create_warning_box(HWND hWnd, int width, int height);
void send_warning_message(std::string text, bool sound);
void warning_delete_memory();

#endif