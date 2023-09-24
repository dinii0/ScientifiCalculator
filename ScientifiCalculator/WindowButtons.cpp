#include "framework.h"

#include "resource.h"
#include "WindowButtons.h"
#include "CalculateButtons.h"
#include "BackgroundMode.h"
#include "TitleBar.h"
#include "ConvertData.h"
#include "ResourceReader.h"
#include "DynamicNumbers.h"
#include "MathFormulas.h"
#include "TableNumber.h"
#include "DeviatedWindows.h"
#include "WarningBox.h"

#include <commctrl.h>
#include <vector>

HMODULE global_instance;
HWND screensize_handler;
RECT screensize_repaint;
std::vector<HWND> text_fields_list;
int registered_index = -1, bracket_counter = 0;
bool is_window_opened = false, is_negative_symbol = false, is_decimal_unfinished = false;
LRESULT CALLBACK ButtonWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void get_global_size_address(HWND hWnd, RECT global_screenSize) {
    screensize_handler = hWnd;
    screensize_repaint = global_screenSize;
}
int get_num_opened_brackets() {
    return bracket_counter;
}
bool is_number_unfinished() {
    return is_decimal_unfinished || is_negative_symbol;
}
bool is_window_open() {
    return is_window_opened;
}

void register_buttons_special_class(HMODULE hInst) {
    WNDCLASS window_class = { 0 };
    window_class.lpfnWndProc = ButtonWindowProc;
    window_class.hInstance = hInst;
    window_class.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SCIENTIFICALCULATOR));
    window_class.lpszClassName = BUTTONS_CONFIG_CLASS;
    window_class.hbrBackground = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_MAIN]);
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&window_class);
}

bool build_buttons_window_menu(HMODULE hInst, int index) {
    registered_index = index -= BUTTONS_CALCULATE_ID;
    if (buttons_calculator_list[index].variable_counter == 0) {
        if (buttons_calculator_list[index].button_name == "Clear equation") {
            if (!buttons_numbers_values.size()) {
                send_warning_message("YOU CAN'T CLEAR AN EQUATION IF THERE ARE NO EQUATIONS IN THE SCREEN.", true);
                return false;
            }

            buttons_numbers_values.clear();
            return false;
        }
        if (buttons_calculator_list[index].button_name == "Delete the previous number") {
            if (!buttons_numbers_values.size()) {
                send_warning_message("YOU CAN'T DELETE THE PREVIOUS NUMBER/SYMBOL IF THERE ARE NO NUMBER/SYMBOLS TO BEGGIN WITH", true);
                return false;
            }

            std::string buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);
            if (buffer.length() > 1) {
                buffer.pop_back();
                buttons_numbers_values.at(buttons_numbers_values.size() - 1) = buffer;
            }
            else {
                if (buffer[0] == ')') ++bracket_counter;
                else if (buffer[0] == '(') --bracket_counter;
                else if (buffer[0] == '-' && is_negative_symbol) is_negative_symbol = false;

                buttons_numbers_values.pop_back();
            }

            return false;
        }

        if(is_limit_reached())
            return false;

        std::string stringular_buffer = "";
        char character_buffer = get_button_element_symbol(index);

        if (character_buffer != '\0') {
            if (!buttons_numbers_values.size()) {
                stringular_buffer = character_buffer;

                if (character_buffer == '.') {
                    send_warning_message("PLEASE ADD A NUMBER BEFORE TRYING TO CONVERT IT AS A FLOAT!!!", true);
                    return false;
                }
                else if (character_buffer == ')') {
                    send_warning_message("PLEASE ADD A BRACKET BEFORE CLOSING!!!", true);
                    return false;
                }
                else if (character_buffer == '-') is_negative_symbol = true;
                else if (!is_string_number(stringular_buffer) && stringular_buffer != "(") {
                    send_warning_message("PLEASE ADD A NUMBER BEFORE ADDING A SYMBOL!!!", true);
                    return false;
                }
                else if (character_buffer == '(') ++bracket_counter;

                buttons_numbers_values.push_back(stringular_buffer);
            }
            else {
                if (isdigit(character_buffer)) {
                    stringular_buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);
                    if (isdigit(stringular_buffer[0]) || (stringular_buffer[0] == '-' && is_negative_symbol)) {
                        stringular_buffer += character_buffer;
                        buttons_numbers_values.at(buttons_numbers_values.size() - 1) = stringular_buffer;

                        if (is_negative_symbol)
                            is_negative_symbol = false;

                        if (is_decimal_unfinished)
                            is_decimal_unfinished = false;
                    }
                    else if (stringular_buffer[0] == ')') {
                        buttons_numbers_values.push_back("*");
                        stringular_buffer = character_buffer;
                        buttons_numbers_values.push_back(stringular_buffer);
                    }
                    else {
                        stringular_buffer = character_buffer;
                        buttons_numbers_values.push_back(stringular_buffer);
                    }
                }
                else {
                    if (is_negative_symbol || is_decimal_unfinished) {
                        send_warning_message("PLEASE ADD A NUMBER BEFORE ADDING A SYMBOL!!!", true);
                        return false;
                    }
                    if (character_buffer == '.') {
                        stringular_buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);

                        if (!is_string_number(stringular_buffer)) {
                            send_warning_message("PLEASE ADD A NUMBER BEFORE TRYING TO CONVERT IT AS A FLOAT!!!", true);
                            return false;
                        }

                        for (int i = 0; i < stringular_buffer.length(); i++) {
                            if (stringular_buffer[i] == '.') {
                                send_warning_message("PLEASE ADD ANOTHER NUMBER BECAUSE THE PREVIOUS ONE IS ALREADY A FLOAT", true);
                                return false;
                            }
                        }

                        is_decimal_unfinished = true;
                        stringular_buffer += character_buffer;
                        buttons_numbers_values.at(buttons_numbers_values.size() - 1) = stringular_buffer;
                    }
                    else if (character_buffer == '(') {
                        stringular_buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);
                        if (is_string_number(stringular_buffer) || stringular_buffer[0] == ')')
                            buttons_numbers_values.push_back("*");

                        bracket_counter++;
                        stringular_buffer = character_buffer;
                        buttons_numbers_values.push_back(stringular_buffer);
                    }
                    else if (character_buffer == ')') {
                        if (!bracket_counter) {
                            send_warning_message("PLEASE ADD A BRACKET BEFORE CLOSING!!!", true);
                            return false;
                        }

                        stringular_buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);
                        if (is_character_operator(stringular_buffer)) {
                            send_warning_message("PLEASE ADD A NUMBER OR AN OPENING BRACKET AFTER YOU ADDED A SYMBOL!!!", true);
                            return false;
                        }

                        bracket_counter--;
                        stringular_buffer = character_buffer;
                        buttons_numbers_values.push_back(stringular_buffer);
                    }
                    else if (character_buffer == '-') {
                        stringular_buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);

                        if (is_character_operator(stringular_buffer)) {
                            send_warning_message("PLEASE ADD A NUMBER BEFORE USING A ARITHMETIC SYMBOL", true);
                            return false;
                        }
                        
                        if (stringular_buffer[0] == '(') 
                            is_negative_symbol = true;

                        stringular_buffer = character_buffer;
                        buttons_numbers_values.push_back(stringular_buffer);
                    }
                    else {
                        stringular_buffer = buttons_numbers_values.at(buttons_numbers_values.size() - 1);

                        if (!is_string_number(stringular_buffer) && stringular_buffer[0] != ')' && character_buffer != '-') {
                            send_warning_message("PLEASE ADD A NUMBER BEFORE CLOSING A BRACKET", true);
                            return false;
                        }
                        else if (is_character_operator(stringular_buffer) && stringular_buffer[0] == '(') {
                            send_warning_message("PLEASE, THE ONLY ARITHMETIC SYMBOL YOU CAN ADD AFTER AN OPENING BRACKET IS MINUS", true);
                            return false;
                        }

                        stringular_buffer = character_buffer;
                        buttons_numbers_values.push_back(stringular_buffer);
                    }
                }
            }
        }
        send_warning_message("", false);
        return false;
    }

    if (is_limit_reached())
        return false;
	
    int screenWidth = GetSystemMetrics(SM_CXSCREEN), screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int positionX = (screenWidth - WINDOW_WIDTH) / 2, positionY = (screenHeight - WINDOW_HEIGHT) / 2;

    HWND hWnd = CreateWindow(BUTTONS_CONFIG_CLASS, L"Button Operation", CW_USEDEFAULT, positionX, positionY, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInst, nullptr);

    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_CAPTION);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SYSMENU);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & CS_HREDRAW);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & CS_VREDRAW);
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    global_instance = hInst;
    UpdateWindow(hWnd);
    is_window_opened = true;
    return true;
}

LRESULT CALLBACK ButtonWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int index_mode = (int)background_info.is_user_dark;
    switch (message) {
        case WM_CREATE: {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            int pos_width = clientRect.right - clientRect.left;
            create_bar_buttons(hWnd, pos_width);
            
            if(buttons_calculator_list[registered_index].variable_counter < 0) create_deviated_windows(buttons_calculator_list[registered_index].button_name, hWnd);
            else {
                HWND hWnd_buffer;
                for (int i = 1; i <= buttons_calculator_list[registered_index].variable_counter; i++) {
                    hWnd_buffer = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 6, (45 + (i * 100)), 210, 20, hWnd, (HMENU)WINDOW_TEXT_BOX_ID, NULL, NULL);
                    text_fields_list.push_back(hWnd_buffer);
                }
            }

            CreateWindow(L"BUTTON", L"o", WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, clientRect.right / 6, clientRect.bottom - (clientRect.bottom / 6), 430, 50, hWnd, (HMENU)WINDOW_IMPLEMENT_BUTTON, NULL, NULL);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            paint_title_bar(hdc);
            RECT screenRect;
            GetClientRect(hWnd, &screenRect);

            HBRUSH circle_outline_color = CreateSolidBrush(RGB(0, 0, 0));
            FrameRect(hdc, &screenRect, circle_outline_color);

            SetTextColor(hdc, colors_table[index_mode][COLORING_TEXT]);
            SetBkMode(hdc, TRANSPARENT);

            HFONT font_new = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
            HFONT font_old = static_cast<HFONT>(SelectObject(hdc, font_new));

            LPCWSTR index_name = convert_string_to_lpcwstr(buttons_calculator_list[registered_index].button_name);
            TextOut(hdc, 6, 4, index_name, static_cast<int>(_tcslen(index_name)));
            delete[] index_name;
            
            if (buttons_calculator_list[registered_index].variable_counter > 0) {
                TCHAR description_text[] = _T("Write a replacable number for your variable!");
                TextOut(hdc, 6, 26, description_text, static_cast<int>(_tcslen(description_text)));

                TCHAR* text_converted;
                std::string prevariable_text = "";
                for (int i = 1; i <= buttons_calculator_list[registered_index].variable_counter; i++) {
                    prevariable_text = "Write a replacable number for ";

                    if (buttons_calculator_list[registered_index].button_name == "Binomial Power" || buttons_calculator_list[registered_index].button_name == "Sum of powers") {
                        if (i == 3) prevariable_text += 'X';
                        else prevariable_text += char(i + 64);
                    }
                    else prevariable_text += char(i + 87);

                    text_converted = convert_string_to_tchar(prevariable_text);
                    TextOut(hdc, 6, (26 + (i * 100)), text_converted, static_cast<int>(_tcslen(text_converted)));
                    delete[] text_converted;
                }
            }
            else paint_deviated_windows(buttons_calculator_list[registered_index].button_name, hdc);

            Gdiplus::Bitmap* bitmap_background;
            HICON button_icon;
            bitmap_background = load_image_from_resource(global_instance, IDB_BUTTON_PNG2 + registered_index, L"PNG");
            bitmap_background->GetHICON(&button_icon);

            Size size_text = get_textual_size(bitmap_background);
            if(buttons_calculator_list[registered_index].button_name == "Greatest Common Factor") DrawIconEx(hdc, 80, 110, button_icon, 480, 480, 0, NULL, DI_NORMAL);
            else DrawIconEx(hdc, WINDOW_WIDTH - size_text.Width, -25, button_icon, 480, 480, 0, NULL, DI_NORMAL);

            HBITMAP hBmp = nullptr;
            bitmap_background->GetHBITMAP(Color(0, 0, 0, 0), &hBmp);
            
            delete bitmap_background;
            DestroyIcon(button_icon);
            SelectObject(hdc, font_old);
            DeleteObject(font_new);
            DeleteObject(circle_outline_color);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_ERASEBKGND: {
            HBRUSH hNewBrush = CreateSolidBrush(colors_table[index_mode][COLORING_MAIN]);

            HDC hdc = (HDC)wParam;
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);

            RECT rect;
            GetClientRect(hWnd, &rect);
            FillRect(hdc, &rect, hNewBrush);

            SelectObject(hdc, hOldBrush);
            DeleteObject(hNewBrush);

            return TRUE;
        }
        case WM_CTLCOLOREDIT: {
            HWND hWnd_buffer = reinterpret_cast<HWND>(lParam);
            HDC hdc = reinterpret_cast<HDC>(wParam);

            SetBkColor(hdc, colors_table[index_mode][COLORING_OBJECT]);
            SetTextColor(hdc, colors_table[index_mode][COLORING_TEXT]);
            return reinterpret_cast<LRESULT>(GetStockObject(NULL_BRUSH));
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);

            switch (wmId) {
                case BUTTON_EXIT: {
                    is_window_opened = false;
                    DestroyWindow(hWnd);
                    break;
                }
                case BUTTON_MINIMAZE: {
                    ShowWindow(hWnd, SW_MINIMIZE);
                    break;
                }
                case MATRIX_COLOMNS_ID: {
                    if (HIWORD(wParam) == EN_CHANGE) {
                        HWND deviated_hWnd = get_input_window_handler();

                        TCHAR buffer[64];
                        GetWindowText(deviated_hWnd, buffer, sizeof(buffer) / sizeof(buffer[0]));

                        std::string numerical_text = convert_tchar_to_string(buffer);

                        destroy_input_windows();
                        if (numerical_text[0] == '\0')
                            break;

                        if (!is_string_number(numerical_text)) {
                            send_warning_message("Please add a number", true);
                            break;
                        }

                        int count = stoi(numerical_text);
                        if (count > 9) {
                            send_warning_message("You cannot have more than 9 colomns/rows", true);
                            break;
                        }
                        if (count < 2) {
                            send_warning_message("You you only have from 2 to 9 colomns/rows", true);
                            break;
                        }
                        
                        create_matrix_input_windows(count, hWnd);
                    }
                    break;
                }
                case GCF_ROWS_ID: {
                    if (HIWORD(wParam) == EN_CHANGE) {
                        HWND deviated_hWnd = get_input_window_handler();

                        TCHAR buffer[64];
                        GetWindowText(deviated_hWnd, buffer, sizeof(buffer) / sizeof(buffer[0]));

                        std::string numerical_text = convert_tchar_to_string(buffer);

                        destroy_input_windows();
                        if (numerical_text[0] == '\0')
                            break;

                        if (!is_string_number(numerical_text)) {
                            send_warning_message("Please add a number", true);
                            break;
                        }

                        int count = stoi(numerical_text);
                        if (count < 1) {
                            send_warning_message("You should write a number greater than zero", true);
                            break;
                        }
                        if (count > 180) {
                            send_warning_message("You cannot add more than 180 field boxes! Lower your number.", true);
                            break;
                        }

                        create_gcf_input_windows(count, hWnd);
                    }
                    break;
                }
                case WINDOW_IMPLEMENT_BUTTON: {
                    TCHAR buffer[64];
                    std::vector<double> textLength;
                    std::string numeral_text = "";
                    double decoy = 0.0;

                    if (!text_fields_list.size()) {
                        send_warning_message("Please write your values inside our fields before trying to implement them", true);
                        return DefWindowProc(hWnd, message, wParam, lParam);
                    }

                    for (HWND hWnd : text_fields_list) {
                        GetWindowText(hWnd, buffer, sizeof(buffer) / sizeof(buffer[0]));
                        decoy = convert_tchar_to_double(buffer);
                        textLength.push_back(decoy);
                        numeral_text = convert_tchar_to_string(buffer);

                        if (!numeral_text.length()) {
                            send_warning_message("All fields should be written with a number!", true);
                            return DefWindowProc(hWnd, message, wParam, lParam);
                        }
                        if (numeral_text[numeral_text.length() - 1] == '.') {
                            send_warning_message("One of the fields is an unfinished number!", true);
                            return DefWindowProc(hWnd, message, wParam, lParam);
                        }
                        if (numeral_text[0] == '.') {
                            send_warning_message("One of the fields has an unrecognizable number!", true);
                            return DefWindowProc(hWnd, message, wParam, lParam);
                        }
                        if (numeral_text[0] == '-' && numeral_text.length() < 2) {
                            send_warning_message("One of the fields is an unfinished negative number!", true);
                            return DefWindowProc(hWnd, message, wParam, lParam);
                        }
                        if(!is_string_number(numeral_text)) {
                            send_warning_message("One of the fields does not have numbers in it!", true);
                            return DefWindowProc(hWnd, message, wParam, lParam);
                        }
                    }

                    // Prepare for some hardcoded shii;
                    double result = 0.0;
                    if (buttons_calculator_list[registered_index].button_name == "Square Root") result = calc_root(textLength.at(0), 2.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Logarithmic with Base 10") result = calc_logarithmic(textLength.at(0), 10.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Exponential with Exponent 2") result = calc_exponential(textLength.at(0), 2.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Variation (without repetition)") result = calc_variation_permutation(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Greatest Common Factor") result = calc_greatest_common_factor(textLength);
                    else if (buttons_calculator_list[registered_index].button_name == "Sigma") result = calc_sigma(textLength.at(0), textLength.at(1), textLength.at(2));
                    else if (buttons_calculator_list[registered_index].button_name == "Pi") result = calc_pi(textLength.at(0), textLength.at(1), textLength.at(2));
                    else if (buttons_calculator_list[registered_index].button_name == "Cubic root") result = calc_root(textLength.at(0), 3.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Logarithmic with Base 5") result = calc_logarithmic(textLength.at(0), 5.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Exponential with Exponent 3") result = calc_exponential(textLength.at(0), 3.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Binomial Coefficient") result = calc_combination_binominal_coefficient(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Premutation") result = calc_variation_permutation(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Factorial") result = calc_factoriel(textLength.at(0));
                    else if (buttons_calculator_list[registered_index].button_name == "Binomial Power") result = calc_binomial_power(textLength.at(0), textLength.at(1), textLength.at(2));
                    else if (buttons_calculator_list[registered_index].button_name == "Sum of powers") result = calc_sum_power(textLength.at(0), textLength.at(1), textLength.at(2));
                    else if (buttons_calculator_list[registered_index].button_name == "Fourth root") result = calc_root(textLength.at(0), 4.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Logarithmic with Base 2") result = calc_logarithmic(textLength.at(0), 2.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Exponential with Exponent 4") result = calc_exponential(textLength.at(0), 4.0);
                    else if (buttons_calculator_list[registered_index].button_name == "Natural logarithmic") result = calc_natural_logarithmic(textLength.at(0));
                    else if (buttons_calculator_list[registered_index].button_name == "Combinations") result = calc_combination_binominal_coefficient(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Cosine") result = cos(textLength.at(0));
                    else if (buttons_calculator_list[registered_index].button_name == "Sine") result = sin(textLength.at(0));
                    else if (buttons_calculator_list[registered_index].button_name == "Tangent") result = tan(textLength.at(0));
                    else if (buttons_calculator_list[registered_index].button_name == "Desired root") result = calc_root(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Logarithmic with desired Base") result = calc_logarithmic(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Exponential with desired Expontent") result = calc_exponential(textLength.at(0), textLength.at(1));
                    else if (buttons_calculator_list[registered_index].button_name == "Sign function") result = calc_sgn(textLength.at(0));
                    else if (buttons_calculator_list[registered_index].button_name == "Matrix determinant") result = calc_matrix(textLength, calc_root(textLength.size(), 2.0));

                    bool is_number_negative = result < 0 ? true : false;

                    if (is_negative_symbol && is_number_negative)
                        result *= -1;

                    if (buttons_numbers_values.size()) {
                        std::string text = buttons_numbers_values.at(buttons_numbers_values.size() - 1);

                        if (text.length() > 1 || isdigit(text[0])) buttons_numbers_values.push_back("+");
                        else if (text[0] == ')') buttons_numbers_values.push_back("*");
                    }

                    const std::string overflow_text = remove_zero_traces(result);
                    if (!is_negative_symbol) {
                        if(is_number_negative)
                            buttons_numbers_values.push_back("(");

                        buttons_numbers_values.push_back(overflow_text);
                        
                        if(is_number_negative)
                            buttons_numbers_values.push_back(")");
                    }
                    else {
                        buttons_numbers_values.at(buttons_numbers_values.size() - 1) += overflow_text;
                        is_negative_symbol = false;
                    }
                    is_window_opened = false;
                    text_fields_list.clear();
                    InvalidateRect(screensize_handler, &screensize_repaint, TRUE);
                    send_warning_message("", false);
                    DestroyWindow(hWnd);
                    break;
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
            paint_exit_button(lpdis);

            if (lpdis->CtlID == WINDOW_IMPLEMENT_BUTTON) {
                HWND hWnd_buffer = GetDlgItem(hWnd, WINDOW_IMPLEMENT_BUTTON);

                RECT rect_button;
                GetClientRect(hWnd_buffer, &rect_button);
                paint_gradientfill_button(lpdis->hDC, rect_button);

                RECT rcButton = lpdis->rcItem;
                DrawText(lpdis->hDC, L"Implement the number into calculator", -1, &rcButton, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }

            break;
        }
        case WM_CLOSE: {
            is_window_opened = false;
            text_fields_list.clear();
            DestroyWindow(hWnd);
            break;
        }
        default: {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}
