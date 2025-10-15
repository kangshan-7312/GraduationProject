#include "pch.h"
#include "CPictureCtrl.h"

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