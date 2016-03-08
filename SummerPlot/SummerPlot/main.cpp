#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <windowsx.h>

#include <string>

#include "Token.h"
#include "Parser.h"
#include "Tokenizer.h"

#include "main.h"

int __stdcall _tWinMain(HINSTANCE hInst, HINSTANCE  h0, LPWSTR lpCmdLine, int nCmdShow)
{
    srand(time(NULL));

    BOOL ret;
    MSG msg;

    if (CoInitialize(0)) return FALSE;

    InitCommonControls();
    hWindow = CreateDialogParamW(hInst, MAKEINTRESOURCEW(IDD_DIALOG_MAIN), 0, DialogProc, 0);
    ShowWindow(hWindow, nCmdShow);

    while (ret = GetMessage(&msg, 0, 0, 0) != 0)
    {
        if (ret == -1)
        {
            return -1;
        }
        if (!IsDialogMessage(hWindow, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CoUninitialize();

    return 0;
}