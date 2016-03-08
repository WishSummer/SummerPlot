#pragma once

#include <UIRibbon.h>

#include "main.handler.ribbon.h"

class UICommandHandler : public IUICommandHandler
{
public:
    static HRESULT CreateInstance(IUICommandHandler **ppCommandHandler)
    {
        if (!ppCommandHandler)
        {
            return E_POINTER;
        }

        *ppCommandHandler = NULL;

        HRESULT hr = S_OK;

        UICommandHandler* pCommandHandler = new UICommandHandler();

        if (pCommandHandler != NULL)
        {
            *ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
        }
        else
        {
            hr = E_OUTOFMEMORY;
        }

        return hr;
    }

    STDMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&m_cRef);
    }

    STDMETHODIMP_(ULONG) Release()
    {
        LONG cRef = InterlockedDecrement(&m_cRef);
        if (cRef == 0)
        {
            delete this;
        }

        return cRef;
    }

    STDMETHODIMP QueryInterface(REFIID iid, void** ppv)
    {
        if (iid == __uuidof(IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }
        else if (iid == __uuidof(IUICommandHandler))
        {
            *ppv = static_cast<IUICommandHandler*>(this);
        }
        else
        {
            *ppv = NULL;
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;
    }

    STDMETHOD(UpdateProperty)(UINT nCmdID, REFPROPERTYKEY key, const PROPVARIANT* pCurrent, PROPVARIANT* pNew)
    {
        return ribbonOnUpdateProperty(nCmdID, key, pCurrent, pNew);
    }

    STDMETHOD(Execute)(UINT nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* pValue, IUISimplePropertySet* pCommandExecutionProperties)
    {
        ribbonOnExecute(nCmdID, verb, key, pValue, pCommandExecutionProperties);
        return S_OK;
    }

private:
    UICommandHandler()
        : m_cRef(1)
    {
    }

    LONG m_cRef;
};
