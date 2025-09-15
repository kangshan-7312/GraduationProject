#include "pch.h"
#include "CMachineID.h"
#include <comdef.h>

bool CMachineID::InitCOM()
{
    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return false;

    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    return SUCCEEDED(hr) || hr == RPC_E_TOO_LATE;
}

CString CMachineID::GetCPUID()
{
    return QueryWMI(L"Win32_Processor", L"ProcessorId").c_str();
}

CString CMachineID::GetMainBoardID()
{
    return QueryWMI(L"Win32_BaseBoard", L"SerialNumber").c_str();
}

CString CMachineID::GetSystemID()
{
    return QueryWMI(L"Win32_OperatingSystem", L"SerialNumber").c_str();
}

CString CMachineID::GetMACID()
{
    PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
    }

    CString strMac = _T("");
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
    {
        for (UINT i = 0; i < pAdapterInfo->AddressLength; i++)
        {
            CString part;
            part.Format(_T("%02X"), pAdapterInfo->Address[i]);
            strMac += part;
        }
    }
    if (pAdapterInfo) free(pAdapterInfo);
    return strMac;
}

CString CMachineID::GetUniqueID()
{
    CString uniqueID;
    uniqueID = GetCPUID() + _T("_") +
        GetMainBoardID() + _T("_") +
        GetSystemID() + _T("_") +
        GetMACID();
    return uniqueID;
}

std::string CMachineID::QueryWMI(const wchar_t* wszClass, const wchar_t* wszProperty)
{
    std::string result;

    CComPtr<IWbemLocator> pLoc;
    CComPtr<IWbemServices> pSvc;
    HRESULT hr = CoCreateInstance(
        CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc
    );
    if (FAILED(hr)) return result;

    hr = pLoc->ConnectServer(
        CComBSTR(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        NULL,
        0,
        NULL,
        NULL,
        &pSvc
    );
    if (FAILED(hr)) return result;

    hr = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hr)) return result;

    CComPtr<IEnumWbemClassObject> pEnumerator;
    std::wstring query = L"SELECT " + std::wstring(wszProperty) + L" FROM " + wszClass;

    hr = pSvc->ExecQuery(
        CComBSTR(L"WQL"),
        CComBSTR(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );
    if (FAILED(hr)) return result;

    CComPtr<IWbemClassObject> pObj;
    ULONG uReturn = 0;
    if (pEnumerator)
    {
        hr = pEnumerator->Next(WBEM_INFINITE, 1, &pObj, &uReturn);
        if (uReturn == 0) return result;

        VARIANT vtProp;
        VariantInit(&vtProp);
        if (SUCCEEDED(pObj->Get(wszProperty, 0, &vtProp, 0, 0)))
        {
            if (vtProp.vt == VT_BSTR && vtProp.bstrVal != nullptr)
            {
                char buffer[256] = { 0 };
                WideCharToMultiByte(CP_ACP, 0, vtProp.bstrVal, -1, buffer, sizeof(buffer), NULL, NULL);
                result = buffer;
            }
        }
        VariantClear(&vtProp);
    }

    return result;
}
