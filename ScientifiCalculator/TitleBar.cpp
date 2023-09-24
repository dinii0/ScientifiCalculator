#include "framework.h"
#include "BackgroundMode.h"
#include "TitleBar.h"

void paint_title_bar(HDC hdc) {
    HBRUSH background_brush = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_OBJECT]);
    HBRUSH circle_outline_color = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH old_brush = (HBRUSH)SelectObject(hdc, background_brush);

    HWND hWnd = WindowFromDC(hdc);

    RECT rect;
    GetClientRect(hWnd, &rect);
    rect.left = rect.top = 0;
    rect.bottom = 25;

    FillRect(hdc, &rect, background_brush);
    FrameRect(hdc, &rect, circle_outline_color);

    SelectObject(hdc, old_brush);
    DeleteObject(background_brush);
    DeleteObject(circle_outline_color);
}

void create_bar_buttons(HWND hWnd, int width) {
    CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | WS_CLIPCHILDREN, width - 22, 4, 20, 17, hWnd, (HMENU)BUTTON_EXIT, NULL, NULL);
    CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | WS_CLIPCHILDREN, width - 45, 2, 20, 17, hWnd, (HMENU)BUTTON_MINIMAZE, NULL, NULL);
}

void paint_exit_button(LPDRAWITEMSTRUCT lpdis) {
    if (lpdis->CtlType == ODT_BUTTON) {
        int index_mode = (int)background_info.is_user_dark;
        RECT rcButton = lpdis->rcItem;
        HBRUSH brush = CreateSolidBrush(colors_table[index_mode][COLORING_OBJECT]);

        FillRect(lpdis->hDC, &rcButton, brush);
        SetTextColor(lpdis->hDC, colors_table[index_mode][COLORING_TEXT]);
        SetBkMode(lpdis->hDC, TRANSPARENT);

        if (lpdis->CtlID == BUTTON_EXIT)
            DrawText(lpdis->hDC, L"  x  ", -1, &rcButton, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        else if (lpdis->CtlID == BUTTON_MINIMAZE)
            DrawText(lpdis->hDC, L"  _  ", -1, &rcButton, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        DeleteObject(brush);
    }
}