#pragma once
//图片展示控件
class CPictureCtrl : public CStatic
{
public:
    CPictureCtrl();
    virtual ~CPictureCtrl();

    BOOL LoadImageFile(const CString& strPath);

protected:
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    Gdiplus::Image* m_pImage;
public:
	void ShowImage(CString path);
	BOOL DownloadImageFromURL(const CString& strURL, const CString& strSavePath);//下载图片
};

