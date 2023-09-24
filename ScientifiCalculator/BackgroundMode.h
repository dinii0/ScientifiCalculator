#ifndef BACKGROUNDMODE_H
#define BACKGROUNDMODE_H

#define BACKGROUND_BUTTON 2750

enum COLORS_MAXIMUM {
    COLORING_LIGHT = 0,
    COLORING_DARK = 1
};
enum COLORS_TYPE {
    COLORING_MAIN = 0,
    COLORING_TEXT = 1,
    COLORING_BLINK = 2,
    COLORING_OUTLINE = 3,
    COLORING_OBJECT = 4,
    COLORING_TABLE = 5,
    COLORING_WINDOW = 6
};
const COLORREF colors_table[COLORS_MAXIMUM::COLORING_DARK + 1][COLORS_TYPE::COLORING_WINDOW + 1] = {
    { RGB(249, 240, 241), RGB(16, 16, 16), RGB(255, 0, 0), RGB(216, 155, 12), RGB(253, 161, 42), RGB(255, 207, 132), RGB(215, 145, 0) },
    { RGB(57, 57, 57), RGB(0, 0, 0), RGB(255, 0, 0), RGB(35, 100, 67), RGB(1, 167, 85), RGB(127, 255, 192), RGB(2, 196, 99) }
};

struct background_mode_vars {
    HWND hbutton_mode = 0;
    HICON hicon = 0;
    ULONG_PTR gdiplusToken = 0;
    COLORREF user_color_mode = 0;
    bool is_user_dark = false;
};
extern background_mode_vars background_info;

void paint_gradientfill_button(HDC hdc, RECT clientRect);
void create_background_button(HWND hWnd, int width);
void spin_gdiplus_image(HDC hdc, RECT clientRect, HICON hIcon);
void start_gdiplus_hooks(HMODULE hInst);
void change_button_background();
void delete_gdiplus_hooks();

#endif

