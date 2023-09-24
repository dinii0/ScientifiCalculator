#include "framework.h"
#include "resource.h"

#include "WarningBox.h"
#include "BackgroundMode.h"
#include "TableNumber.h"
#include "ResourceReader.h"
#include "ConvertData.h"
#include "bass/c/bass.h"

#include <mmsystem.h>
#include <CommCtrl.h>

#pragma comment(lib, "bass/c/x64/bass.lib")
#pragma comment(lib, "winmm.lib")

HMODULE hDLL = LoadLibrary(L"bass.dll");

HWND warning_hWnd;
RECT warning_size;
std::string showing_text;

const void* file_mp3;
DWORD file_mp3_size;

bool is_audio_emiting = false;

void CALLBACK bass_sound_callend(HSYNC handle, DWORD channel, DWORD data, void* user) {
    BASS_StreamFree(channel);
    is_audio_emiting = false;
    //BASS_Free();
}

void paint_warning_box(HDC hdc, HWND hWnd) {
    HBRUSH brush_outline = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_OUTLINE]);
    HBRUSH brush_background = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_TABLE]);

    FillRect(hdc, &warning_size, brush_outline);
    optimise_rect_position(&warning_size, -3);
    FillRect(hdc, &warning_size, brush_background);
    optimise_rect_position(&warning_size, 3);

    if (showing_text.length()) {
        TCHAR* buffer = convert_string_to_tchar(showing_text);

        SIZE textSize;
        GetTextExtentPoint32(hdc, buffer, wcslen(buffer), &textSize);
        int text_width = (warning_size.right - textSize.cx) / 2, text_height = (warning_size.bottom - textSize.cy) / 2;

        TextOut(hdc, text_width + (warning_size.left / 2), text_height + (warning_size.top / 2), buffer, static_cast<int>(_tcslen(buffer)));
    }

    DeleteObject(brush_outline);
    DeleteObject(brush_background);
}
void create_warning_box(HWND hWnd, int width, int height) {
    warning_size.left = 4;
    warning_size.top = 42;
    warning_size.right = (width / 2);
    warning_size.bottom = 90;

    warning_hWnd = hWnd;
    
    if (waveOutGetNumDevs() == 0)
        MessageBox(NULL, L"We couldn't detect any audio devices. This app will emit sounds from time to time and you won't be able to hear it!", L"Error", MB_OK | MB_ICONERROR);

    BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);
    BASS_SetConfig(BASS_CONFIG_UNICODE, true);

    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        MessageBox(NULL, L"There is something wrong with one of your audio drivers, please restart the application.\nIf the error keeps being mentioned contact the developer!", L"Error", MB_OK | MB_ICONERROR);

    HRSRC file_resource = FindResource(nullptr, MAKEINTRESOURCE(IDR_MP3_RESOURCE), RT_RCDATA);
    if (file_resource) {
        HGLOBAL memory = LoadResource(nullptr, file_resource);
        if (memory) {
            file_mp3 = LockResource(memory);
            file_mp3_size = SizeofResource(nullptr, file_resource);
        }
    }
}
void warning_delete_memory() {
    FreeLibrary(hDLL);
    BASS_Free();
}
void send_warning_message(std::string text, bool sound) {
    showing_text = text;
    InvalidateRect(warning_hWnd, &warning_size, FALSE);

    if (!sound || is_audio_emiting)
        return;

    HSTREAM stream = BASS_StreamCreateFile(TRUE, file_mp3, 0, file_mp3_size, BASS_SAMPLE_FLOAT);
    if (!stream) {
        BASS_Free();
        return;
    }

    BASS_ChannelSetSync(stream, BASS_SYNC_END, 0, bass_sound_callend, NULL);
    BASS_ChannelPlay(stream, false);
    is_audio_emiting = true;
}