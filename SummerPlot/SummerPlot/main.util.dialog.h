#pragma once

#include <windows.h>
#include <string>

std::wstring showSaveDialog(const WCHAR * filter, const WCHAR * defaultExt)
{
    WCHAR szFile[MAX_PATH];

    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = defaultExt;

    if (!GetSaveFileNameW(&ofn))
    {
        return L"";
    }
    else
    {
        return std::wstring(szFile);
    }
}