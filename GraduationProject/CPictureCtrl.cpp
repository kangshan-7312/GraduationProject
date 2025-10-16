#include "pch.h"
#include "CPictureCtrl.h"
#include "CPictureShowDlg.h"
#include <io.h> // 添加此头文件
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

CPictureCtrl::CPictureCtrl()
{
    m_pImage = nullptr;
}

CPictureCtrl::~CPictureCtrl()
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = nullptr;
    }
}

BEGIN_MESSAGE_MAP(CPictureCtrl, CStatic)
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPictureCtrl::LoadImageFile(const CString& strPath)
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = nullptr;
    }

    // 转换为宽字符
    LPCWSTR wPath = CT2W(strPath);
    m_pImage = new Gdiplus::Image(wPath);

    if (m_pImage->GetLastStatus() != Ok)
    {
        delete m_pImage;
        m_pImage = nullptr;
        return FALSE;
    }

    Invalidate(); // 触发重绘
    return TRUE;
}

void CPictureCtrl::OnPaint()
{
    CPaintDC dc(this);
    if (!m_pImage) return;

    Graphics graphics(dc.GetSafeHdc());

    CRect rect;
    GetClientRect(&rect);
    int wndW = rect.Width();
    int wndH = rect.Height();

    UINT imgW = m_pImage->GetWidth();
    UINT imgH = m_pImage->GetHeight();

    double imgRatio = (double)imgW / imgH;
    double wndRatio = (double)wndW / wndH;

    int drawW = 0, drawH = 0;
    int offsetX = 0, offsetY = 0;

    if (imgRatio > wndRatio)
    {
        drawW = wndW;
        drawH = (int)(wndW / imgRatio);
        offsetX = 0;
        offsetY = (wndH - drawH) / 2;
    }
    else
    {
        drawH = wndH;
        drawW = (int)(wndH * imgRatio);
        offsetX = (wndW - drawW) / 2;
        offsetY = 0;
    }

    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.DrawImage(m_pImage, offsetX, offsetY, drawW, drawH);
}

void CPictureCtrl::OnSize(UINT nType, int cx, int cy)
{
    CStatic::OnSize(nType, cx, cy);
    Invalidate();
}

void CPictureCtrl::ShowImage(CString path)
{
    // 判断文件是否存在
    if (_access(path, 0) != 0)
    {
        MessageBox(_T("指定的图片文件不存在！"), _T("错误"), MB_ICONERROR);
        return;
    }
    else
    {
        int x = MessageBox(_T("图片预览完，按Q/q键关闭"), _T("提示"), MB_YESNO | MB_ICONINFORMATION);
        if (x == IDYES)
        {
            CPictureShowDlg dlg;
            dlg.image_path = _T(path);
            dlg.DoModal();
        }
    }
}

BOOL CPictureCtrl::DownloadImageFromURL(const CString& strURL, const CString& strSavePath)
{
    // 使用CA2A将CString转换为LPCSTR
    USES_CONVERSION;
    LPCSTR szURL = T2CA(strURL);
    LPCSTR szSavePath = T2CA(strSavePath);

    HRESULT hr = URLDownloadToFileA(
        NULL,           // 无绑定上下文
        szURL,          // URL（LPCSTR）
        szSavePath,     // 本地保存路径（LPCSTR）
        0,              // 保留，必须为0
        NULL            // 回调接口
    );
    return SUCCEEDED(hr);
}


