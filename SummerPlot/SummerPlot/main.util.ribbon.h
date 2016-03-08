#pragma once

#include <UIRibbon.h>

#include "main.controls.h"
#include "main.ribbon.uiapplication.h"

bool ribbonInit(HWND hWnd)
{
    HRESULT hr = CoCreateInstance(
        CLSID_UIRibbonFramework,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&ribbonFramework)
        );
    if (FAILED(hr)) return false;

    hr = UIApplication::CreateInstance(&ribbonApp);
    if (FAILED(hr)) return false;

    hr = ribbonFramework->Initialize(hWnd, ribbonApp);
    if (FAILED(hr)) return false;

    hr = ribbonFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
    if (FAILED(hr)) return false;

    return true;
}

void ribbonDestroy()
{
    if (ribbonFramework)
    {
        ribbonFramework->Destroy();
        ribbonFramework->Release();
        ribbonFramework = NULL;
    }

    if (ribbonApp)
    {
        ribbonApp->Release();
        ribbonApp = NULL;
    }
}