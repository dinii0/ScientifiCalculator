#include "framework.h"
#include "resource.h"

#include "ResourceReader.h"
#include <commctrl.h>

// Reference: https://stackoverflow.com/a/66238748
Gdiplus::Bitmap* load_image_from_resource(HINSTANCE hInst, int ICON_INDEX, LPCWSTR file_prefix) {
    IStream* pStream = nullptr;
    Gdiplus::Bitmap* pBmp = nullptr;
    HGLOBAL hGlobal = nullptr;

    HRSRC hrsrc = FindResourceW(hInst, MAKEINTRESOURCE(ICON_INDEX), file_prefix);
    if (hrsrc) {
        DWORD dwResourceSize = SizeofResource(hInst, hrsrc);
        if (dwResourceSize > 0) {
            HGLOBAL hGlobalResource = LoadResource(hInst, hrsrc);
            if (hGlobalResource) {
                void* imagebytes = LockResource(hGlobalResource);
                hGlobal = ::GlobalAlloc(GHND, dwResourceSize);
                if (hGlobal) {
                    void* pBuffer = ::GlobalLock(hGlobal);
                    if (pBuffer) {
                        memcpy(pBuffer, imagebytes, dwResourceSize);
                        HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
                        if (SUCCEEDED(hr)) {
                            hGlobal = nullptr;
                            pBmp = new Gdiplus::Bitmap(pStream);
                        }
                    }
                }
            }
        }
    }

    if (pStream) {
        pStream->Release();
        pStream = nullptr;
    }

    if (hGlobal) {
        GlobalFree(hGlobal);
        hGlobal = nullptr;
    }

    return pBmp;
}

// Reference: ChatGPT
Size get_textual_size(Gdiplus::Bitmap* bitmap_background) {
    Size imageSize(0, 0);

    if (bitmap_background) {
        int width = bitmap_background->GetWidth();
        int height = bitmap_background->GetHeight();

        // Iterate through the pixels to find the non-transparent bounds
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                Color pixelColor;
                bitmap_background->GetPixel(x, y, &pixelColor);

                // Check if the pixel is not transparent
                if (pixelColor.GetAlpha() > 0) {
                    imageSize.Width = max(imageSize.Width, x + 1);
                    imageSize.Height = max(imageSize.Height, y + 1);
                }
            }
        }
    }

    return imageSize;
}