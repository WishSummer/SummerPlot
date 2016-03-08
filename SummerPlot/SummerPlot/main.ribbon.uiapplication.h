#pragma once

#include <UIRibbon.h>

#include "main.ribbon.uicommandhandler.h"
#include "main.controls.h"

class UIApplication : public IUIApplication
{
public:
    static HRESULT CreateInstance(IUIApplication **ppApplication)
    {
        *ppApplication = NULL;

        HRESULT hr = S_OK;

        UIApplication* pApplication = new UIApplication();

        if (pApplication != NULL)
        {
            *ppApplication = static_cast<IUIApplication *>(pApplication);
        }
        else
        {
            hr = E_OUTOFMEMORY;
        }

        return hr;
    }

    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv)
    {
        if (iid == __uuidof(IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }
        else if (iid == __uuidof(IUIApplication))
        {
            *ppv = static_cast<IUIApplication*>(this);
        }
        else
        {
            *ppv = NULL;
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&m_cRef);
    }
    IFACEMETHODIMP_(ULONG) Release()
    {
        LONG cRef = InterlockedDecrement(&m_cRef);
        if (cRef == 0)
        {
            delete this;
        }

        return cRef;
    }

    STDMETHOD(OnCreateUICommand)(UINT nCmdID,
        UI_COMMANDTYPE typeID,
        IUICommandHandler** ppCommandHandler)
    {
        if (NULL == m_pCommandHandler)
        {
            HRESULT hr = UICommandHandler::CreateInstance(&m_pCommandHandler);
            if (FAILED(hr)) return hr;
        }

        return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
    }

    STDMETHOD(OnViewChanged)(UINT viewId,
        UI_VIEWTYPE typeId,
        IUnknown* pView,
        UI_VIEWVERB verb,
        INT uReasonCode)
    {
        HRESULT hr = E_NOTIMPL;

        if (UI_VIEWTYPE_RIBBON == typeId)
        {
            switch (verb)
            {
            case UI_VIEWVERB_SIZE:
                SendMessageW(hWindow, WM_RIBBON_RESIZE, 0, 0);
                hr = S_OK;
                break;
            case UI_VIEWVERB_CREATE:
                hr = pView->QueryInterface(&ribbon);
                break;
            case UI_VIEWVERB_DESTROY:
                hr = S_OK;
                ribbon = NULL;
                break;
            }
        }

        return hr;
    }

    STDMETHOD(OnDestroyUICommand)(UINT32 commandId,
        UI_COMMANDTYPE typeID,
        IUICommandHandler* commandHandler)
    {
        return E_NOTIMPL;
    }

private:
    UIApplication()
        : m_cRef(1)
        , m_pCommandHandler(NULL)
    {
    }

    ~UIApplication()
    {
        if (m_pCommandHandler)
        {
            m_pCommandHandler->Release();
            m_pCommandHandler = NULL;
        }
    }

    LONG m_cRef;
    IUICommandHandler * m_pCommandHandler;
};
