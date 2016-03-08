#pragma once

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

int getEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;
    UINT  size = 0;

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) return -1;

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return -1;
}

void exportHBITMAP(HBITMAP bitmap, const WCHAR * format, const WCHAR * dest)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    Bitmap *image = new Bitmap(bitmap, NULL);

    CLSID clsid;
    int retVal = getEncoderClsid(format, &clsid);

    ULONG quality = 100;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &quality;

    image->Save(dest, &clsid, &encoderParameters);
    delete image;

    GdiplusShutdown(gdiplusToken);
}
