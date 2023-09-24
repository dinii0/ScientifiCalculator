#include "resource.h"

#include "framework.h"
#include "BackgroundMode.h"
#include "ResourceReader.h"

#include <commctrl.h>
#include <fstream>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "msimg32.lib")

background_mode_vars background_info;

void set_rect_position(RECT *clientRect, int number) {
    clientRect->right += number;
    clientRect->top -= number;
    clientRect->bottom += number;
    clientRect->left -= number;
}

void paint_gradientfill_button(HDC hdc, RECT clientRect) {
    int index_mode = (int) background_info.is_user_dark;
    HBRUSH circle_outline_color = CreateSolidBrush(colors_table[index_mode][COLORING_OUTLINE]);
    TRIVERTEX vertices[2] = { 0 };

    set_rect_position(&clientRect, 3);
    FillRect(hdc, &clientRect, circle_outline_color);
    set_rect_position(&clientRect, -3);

    vertices[0].x = clientRect.left;
    vertices[0].y = clientRect.top;
    vertices[0].Red = GetRValue(colors_table[index_mode][COLORING_OBJECT]) << 8;
    vertices[0].Green = GetGValue(colors_table[index_mode][COLORING_OBJECT]) << 8;
    vertices[0].Blue = GetBValue(colors_table[index_mode][COLORING_OBJECT]) << 8;
    vertices[0].Alpha = 0x0000;

    vertices[1].x = clientRect.right;
    vertices[1].y = clientRect.bottom;
    vertices[1].Red = GetRValue(colors_table[index_mode][COLORING_WINDOW]) << 8;
    vertices[1].Green = GetGValue(colors_table[index_mode][COLORING_WINDOW]) << 8;
    vertices[1].Blue = GetBValue(colors_table[index_mode][COLORING_WINDOW]) << 8;
    vertices[1].Alpha = 0x0000;

    GRADIENT_RECT gradientRect = { 0, 1 };
    GradientFill(hdc, vertices, 2, &gradientRect, 1, GRADIENT_FILL_RECT_H);

    DeleteObject(circle_outline_color);
}
void spin_gdiplus_image(HDC hdc, RECT clientRect, HICON hIcon) {

    Graphics graphics(hdc);

    float angle = background_info.is_user_dark == false ? 180.0f : 0.0f;

    XFORM xform_null;
    xform_null.eM11 = 1.0f;
    xform_null.eM12 = 0.0f;
    xform_null.eM21 = 0.0f;
    xform_null.eM22 = 1.0f;
    xform_null.eDx = 0.0f;
    xform_null.eDy = 0.0f;

    SetGraphicsMode(hdc, GM_ADVANCED);
    XFORM xform;
    xform.eDx = (float)(clientRect.right - clientRect.left) / 2;
    xform.eDy = (float)(clientRect.bottom - clientRect.top) / 2;
    xform.eM11 = cos(angle * 3.14159f / 180.0f);
    xform.eM12 = sin(angle * 3.14159f / 180.0f);
    xform.eM21 = -sin(angle * 3.14159f / 180.0f);
    xform.eM22 = cos(angle * 3.14159f / 180.0f);

    SetWorldTransform(hdc, &xform);
    DrawIconEx(hdc, -19, -19, hIcon, clientRect.right, clientRect.bottom, 0, NULL, DI_NORMAL);
    SetWorldTransform(hdc, &xform_null);
}

void change_button_background() {
    background_info.is_user_dark = background_info.is_user_dark == true ? false : true;
    InvalidateRect(background_info.hbutton_mode, NULL, TRUE);
}
LRESULT CALLBACK background_mode_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    switch (uMsg) {
        case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

            RECT rect;
            GetClientRect(hWnd, &rect);

            paint_gradientfill_button(hdc, rect);
            spin_gdiplus_image(hdc, rect, background_info.hicon);

			EndPaint(hWnd, &ps);
			break;
        }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
void create_background_button(HWND hWnd, int width) {
    background_info.hbutton_mode = CreateWindow(L"BUTTON", L"X", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, width - 55, 45, 40, 40, hWnd, (HMENU)BACKGROUND_BUTTON, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
	SetWindowSubclass(background_info.hbutton_mode, background_mode_proc, 0, 0);
}
void start_gdiplus_hooks(HMODULE hInst) {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&background_info.gdiplusToken, &gdiplusStartupInput, NULL);

    Gdiplus::Bitmap* bitmap_background;
    bitmap_background = load_image_from_resource(hInst, IDB_PNG1, L"PNG");
    bitmap_background->GetHICON(&background_info.hicon);
}
void delete_gdiplus_hooks() {
    Gdiplus::GdiplusShutdown(background_info.gdiplusToken);
}
