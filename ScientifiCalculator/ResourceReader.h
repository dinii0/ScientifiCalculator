#ifndef RESOURCE_READER_H
#define RESOURCE_READER_H

Gdiplus::Bitmap* load_image_from_resource(HINSTANCE hInst, int ICON_INDEX, LPCWSTR file_prefix);
Size get_textual_size(Gdiplus::Bitmap* bitmap_background);

#endif 