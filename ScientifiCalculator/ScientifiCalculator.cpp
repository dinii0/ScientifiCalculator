#include "framework.h"
#include "ScientifiCalculator.h"
#include "BackgroundMode.h"
#include "ResourceReader.h"
#include "TitleBar.h"
#include "CalculateButtons.h"
#include "WindowButtons.h"
#include "TableNumber.h"
#include "DynamicNumbers.h"
#include "WarningBox.h"
#include "HistoryList.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst; 
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING]; 

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SCIENTIFICALCULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    register_buttons_special_class(hInst);
    start_gdiplus_hooks(hInst);
    read_initiliased_buttons();

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCIENTIFICALCULATOR));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCIENTIFICALCULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_MAIN]));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SCIENTIFICALCULATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

RECT global_screenSize;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            create_warning_box(hWnd, clientRect.right, clientRect.bottom);
            create_history_list(hWnd, clientRect.right, clientRect.bottom);

            int pos_width = clientRect.right - clientRect.left, pos_height = clientRect.bottom;
            create_background_button(hWnd, pos_width);
            create_bar_buttons(hWnd, pos_width);
            create_calculate_buttons(hWnd, hInst, pos_width, pos_height);
            global_screenSize = read_table_dimensions(clientRect);
            
            LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
            style &= ~(WS_CAPTION | WS_SYSMENU);
            SetWindowLongPtr(hWnd, GWL_STYLE, style);
            SetMenu(hWnd, NULL);

            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);

            switch (wmId) {
                case BUTTON_EXIT: {
                    DestroyWindow(hWnd);
                    break;
                }
                case HISTORY_LIST_ID: {
                    HWND hWnd_button = GetDlgItem(hWnd, HISTORY_LIST_ID);
                    InvalidateRect(hWnd_button, NULL, TRUE);

                    if (!buttons_numbers_values.size()) {
                        send_warning_message("YOU CANNOT SAVE EQUATIONS IF THERE ARE NO EQUATIONS TO BEGIN WITH!", true);
                        break;
                    }
                    if (!(buttons_numbers_values.size() >= 2 && !get_num_opened_brackets() && !is_number_unfinished() && (is_string_number(buttons_numbers_values.at(buttons_numbers_values.size() - 1)) || buttons_numbers_values.at(buttons_numbers_values.size() - 1)[0] == ')'))) {
                        send_warning_message("YOUR EQUATION IS NOT READY TO BE SAVED BECAUSE NO RESULT CAN BE TAKEN FROM IT! FINISH IT!", true);
                        break;
                    }

                    std::string whole_equation = buttons_numbers_values.at(0);

                    for(int i = 1; i < buttons_numbers_values.size(); i++) 
                        whole_equation += " " + buttons_numbers_values.at(i);

                    whole_equation += " = " + remove_zero_traces(get_total_calculation(buttons_numbers_values));
                    update_histroy_list(whole_equation);
                    break;
                }
                case BUTTON_MINIMAZE: {
                    ShowWindow(hWnd, SW_MINIMIZE);
                    break;
                }
                case BACKGROUND_BUTTON: {
                    if (!is_window_open()) {
                        change_button_background();
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    else {
                        HWND hWnd_button = GetDlgItem(hWnd, BACKGROUND_BUTTON);
                        InvalidateRect(hWnd_button, NULL, TRUE);
                    }

                    break;
                }
                default: {
                    if (wmId >= BUTTONS_CALCULATE_ID && wmId < BUTTONS_CALCULATE_ID + BUTTONS_CALCULATE_TOTAL && !is_window_open()) {
                        build_buttons_window_menu(hInst, wmId);
                        InvalidateRect(hWnd, &global_screenSize, TRUE);
                        get_global_size_address(hWnd, global_screenSize);
                    }
                    
                    HWND hWnd_button = GetDlgItem(hWnd, wmId);
                    InvalidateRect(hWnd_button, NULL, TRUE);

                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT screenRect;
            GetClientRect(hWnd, &screenRect);

            HBRUSH brush = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_MAIN]);
            FillRect(hdc, &screenRect, brush);

            paint_title_bar(hdc);
            paint_history_outerline(hdc);

            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SCIENTIFICALCULATOR));
            DrawIconEx(hdc, 5, 5, hIcon, 15, 15, 0, NULL, DI_NORMAL);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, colors_table[(int)background_info.is_user_dark][COLORING_TEXT]);

            paint_warning_box(hdc, hWnd);

            HFONT font_new = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
            HFONT font_old = static_cast<HFONT>(SelectObject(hdc, font_new));

            TCHAR title_name[] = _T("Scientific Calculator");
            TextOut(hdc, 25, 4, title_name, static_cast<int>(_tcslen(title_name)));

            SelectObject(hdc, font_old);
            paint_table(hWnd, hdc);
            
            DeleteObject(font_new);
            DeleteObject(brush);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_CTLCOLORSTATIC: {
            return (LRESULT)colorize_history_list(wParam);
        }
        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
            paint_exit_button(lpdis);
            break;
        }
        case WM_DESTROY: {
            delete_gdiplus_hooks();
            PostQuitMessage(0);
            warning_delete_memory();
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}