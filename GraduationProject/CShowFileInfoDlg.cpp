// CShowFileInfoDlg.cpp: 实现文件
//

#include "pch.h"
#include "GraduationProject.h"
#include "afxdialogex.h"
#include "CShowFileInfoDlg.h"
#include "CAPICall.h"
#include "string"
#include "json.hpp"
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")
using json = nlohmann::json;



// CShowFileInfoDlg 对话框
IMPLEMENT_DYNAMIC(CShowFileInfoDlg, CDialogEx)

CShowFileInfoDlg::CShowFileInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

CShowFileInfoDlg::~CShowFileInfoDlg()
{
}

void CShowFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_show_file_info_name);
	DDX_Control(pDX, IDC_EDIT5, m_show_file_info_path);
	DDX_Control(pDX, IDC_EDIT6, m_show_file_info_exa);
	DDX_Control(pDX, IDC_EDIT7, m_show_file_info_size);
	DDX_Control(pDX, IDC_EDIT11, m_show_file_info_attributes);
	DDX_Control(pDX, IDC_EDIT16, m_show_file_info_creationTime);
	DDX_Control(pDX, IDC_EDIT13, m_show_file_info_lastAccessTime);
	DDX_Control(pDX, IDC_EDIT18, m_show_file_info_lastWriteTime);
	DDX_Control(pDX, IDC_EDIT3, m_show_file_info_hash);
	DDX_Control(pDX, IDC_EDIT8, m_show_file_info_hash_fun);
	DDX_Control(pDX, IDC_EDIT9, m_key);
	DDX_Control(pDX, IDC_COMBO1, m_comboTest);
	DDX_Control(pDX, IDC_EDIT10, m_outpath);
}


BEGIN_MESSAGE_MAP(CShowFileInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CShowFileInfoDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CShowFileInfoDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CShowFileInfoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON9, &CShowFileInfoDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CShowFileInfoDlg 消息处理程序

void CShowFileInfoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_comboTest.GetCurSel();
	m_key.GetWindowText(key);
	m_outpath.GetWindowText(outpath);
	if (index != CB_ERR)
	{
		CString text;
		m_comboTest.GetLBText(index, text);

		if (text == _T("AES (带Key)"))
		{

			if (key.IsEmpty() || outpath.IsEmpty())
			{
				AfxMessageBox(_T("请输入 Key,或你的输出目录为空!"));
				return;
			}
			else
			{
				if (PathFileExists(outpath))
				{
					FileEncryptionAPI("AES");
				}
				else 
				{
					AfxMessageBox(_T("你的输出目录不存在!"));
					return;
				}
			}

		}
		else if (text == _T("DES (带Key)") || outpath.IsEmpty())
		{
			if(key.IsEmpty())
			{
				AfxMessageBox(_T("请输入 Key,或你的输出目录为空!"));
				return;
			}
			else
			{
				if (PathFileExists(outpath))
				{
					FileEncryptionAPI("DES");
				}
				else
				{
					AfxMessageBox(_T("你的输出目录不存在!"));
					return;
				}
			}
		}
		else if (text == _T("BASE64 (不带Key)"))
		{
			if(outpath.IsEmpty())
			{
				AfxMessageBox(_T("你的输出目录为空!"));
				return;
			}
			if (PathFileExists(outpath))
			{
				FileEncryptionAPI("base64");
			}
			else
			{
				AfxMessageBox(_T("你的输出目录不存在!"));
				return;
			}
		}
		else
		{
			AfxMessageBox(_T("未知选项"));
		}
	}
}

BOOL CShowFileInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	m_comboTest.AddString(_T("AES (带Key)"));
	m_comboTest.AddString(_T("DES (带Key)"));
	m_comboTest.AddString(_T("BASE64 (不带Key)"));
	m_comboTest.SetCurSel(0);   // 默认选中第一项

	//////////////////////////////////文件信息 JSON 数据处理//////////////////////////////////
	FILE_INFO_DATA fi = FileInfoJsonData();
	//AfxMessageBox(fi.file_name + "\n" + fi.file_path + "\n" + fi.file_exa + "\n" + fi.file_size + "\n" + fi.file_attributes + "\n" + fi.file_creationTime + "\n" + fi.file_lastAccessTime + "\n" + fi.file_lastWriteTime);
	m_show_file_info_name.SetWindowText(fi.file_name);
	m_show_file_info_path.SetWindowText(fi.file_path);
	m_show_file_info_exa.SetWindowText(fi.file_exa);
	m_show_file_info_size.SetWindowText(fi.file_size);
	m_show_file_info_attributes.SetWindowText(fi.file_attributes);
	m_show_file_info_creationTime.SetWindowText(fi.file_creationTime);
	m_show_file_info_lastAccessTime.SetWindowText(fi.file_lastAccessTime);
	m_show_file_info_lastWriteTime.SetWindowText(fi.file_lastWriteTime);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////文件哈希 JSON 数据处理//////////////////////////////////
	FILE_HASH_DATA hash_info = FileHashDataJsonData();
	m_show_file_info_hash.SetWindowText(hash_info.value);
	m_show_file_info_hash_fun.SetWindowText(hash_info.func);
	//////////////////////////////////////////////////////////////////////////////////////////
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 可选：封装一个 setter 方法
void CShowFileInfoDlg::SetFileInfo(int iItem, const CString& filePath)
{
	m_iItem = iItem;
	m_filePath = filePath;
}

CString CShowFileInfoDlg::UTF8ToCString(const std::string& utf8Str) // UTF-8 转 CString
{
	if (utf8Str.empty())
		return _T("");

	// UTF-8 转宽字符
	int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
	if (wideLen <= 0)
		return _T("");

	wchar_t* wideBuffer = new wchar_t[wideLen];
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, wideBuffer, wideLen);

#ifdef _UNICODE
	CString result(wideBuffer);
#else
	// 宽字符再转 ANSI（当前系统ACP）
	int ansiLen = WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
	char* ansiBuffer = new char[ansiLen];
	WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, ansiBuffer, ansiLen, NULL, NULL);
	CString result(ansiBuffer);
	delete[] ansiBuffer;
#endif

	delete[] wideBuffer;
	return result;
}

void CShowFileInfoDlg::DebugPrintBytes(const std::string& s) // 调试打印字节内容
{
	CString msg;
	msg.Format(_T("[Debug Bytes] len=%zu\n"), s.size());

	for (size_t i = 0; i < s.size(); ++i)
	{
		CString byteStr;
		byteStr.Format(_T("%02X "), (unsigned char)s[i]);
		msg += byteStr;

		if ((i + 1) % 16 == 0)
			msg += _T("\n");
	}

	// 弹出消息框查看，或者你也可以输出到多行编辑框
	AfxMessageBox(msg);
}

FILE_INFO_DATA CShowFileInfoDlg::FileInfoJsonData()  //文件信息 JSON 数据处理
{
	CAPICall api;
	FILE_INFO_DATA fi;
	std::string response;

	// 可选：自定义请求头
	api.SetHeader("User-Agent: MyMFCApp/1.0");

	// 构造 GET 参数
	std::map<std::string, std::string> params;
	params["action"] = "info";
	params["file_path"] = m_filePath;

	// 调用 GET 请求
	response = api.Get("http://localhost:5000/file_api", params);

	try
	{
		json j = json::parse(response);

		std::string status = j.value("status", "");
		if (status != "ok")
		{
			AfxMessageBox(_T("接口返回状态异常"));
			EndDialog(0);
			return fi;
		}

		json data = j["data"];

		std::string access_time = data.value("access_time", "");
		std::string create_time = data.value("create_time", "");
		std::string file_ext = data.value("file_ext", "");
		std::string file_name = data.value("file_name", "");
		std::string file_path = data.value("file_path", "");
		size_t file_size = data.value("file_size", 0);
		std::string modify_time = data.value("modify_time", "");

		// file_attr 是数组
		std::vector<std::string> file_attr;
		if (data.contains("file_attr") && data["file_attr"].is_array())
		{
			for (auto& item : data["file_attr"])
				file_attr.push_back(item.get<std::string>());
		}

		fi.file_name = UTF8ToCString(file_name);
		fi.file_path = UTF8ToCString(file_path);
		fi.file_exa = UTF8ToCString(file_ext);
		fi.file_size.Format(_T("%zu 字节"), file_size);
		// 拼接属性字符串
		CString attrStr;
		for (const auto& attr : file_attr)
		{
			attrStr += UTF8ToCString(attr) + _T(" ");
		}
		fi.file_attributes = attrStr;
		fi.file_creationTime = UTF8ToCString(create_time);
		fi.file_lastAccessTime = UTF8ToCString(access_time);
		fi.file_lastWriteTime = UTF8ToCString(modify_time);

		//AfxMessageBox(fi.file_name + "\n" + fi.file_path + "\n" + fi.file_exa + "\n" + fi.file_size + "\n" + fi.file_attributes + "\n" + fi.file_creationTime + "\n" + fi.file_lastAccessTime + "\n" + fi.file_lastWriteTime);
		return fi;
	}
	catch (json::parse_error&)
	{
		AfxMessageBox(_T("JSON 解析失败!"));
	}
}

FILE_HASH_DATA CShowFileInfoDlg::FileHashDataJsonData()
{
	CAPICall api;
	FILE_HASH_DATA hash_info;
	// 可选：自定义请求头
	api.SetHeader("User-Agent: MyMFCApp/1.0");

	// 构造 GET 参数
	std::map<std::string, std::string> params;
	params["action"] = "hash";
	params["method"] = "md5";
	params["file_path"] = m_filePath;

	// 调用 GET 请求
	std::string response = api.Get("http://localhost:5000/file_api", params);

	try
	{
		json j = json::parse(response);

		if (!j.is_object())
		{
			AfxMessageBox(_T("JSON不是对象"));
			return hash_info;
		}

		std::string hash_method = j.value("hash_method", "");
		std::string hash_value = j.value("hash_value", "");
		std::string status = j.value("status", "");

		hash_info.func = UTF8ToCString(hash_method);
		hash_info.value = UTF8ToCString(hash_value);
		CString cStatus = UTF8ToCString(status);

		//AfxMessageBox(hash_info.func + _T("\n") + hash_info.value + _T("\n") + cStatus);
		return hash_info;
	}
	catch (json::parse_error& e)
	{
		AfxMessageBox(_T("JSON 解析失败!"));
	}

}

void CShowFileInfoDlg::FileEncryptionAPI(CString cmethod)
{
	CAPICall api;
	std::string response;
	// 可选：自定义请求头
	api.SetHeader("User-Agent: MyMFCApp/1.0");
	// 构造 GET 参数
	std::map<std::string, std::string> params;
	params["action"] = "encrypt";
	params["method"] = cmethod;
	params["file_path"] = m_filePath;
	params["key"] = key;
	params["output_dir"] = outpath;
	// 调用 GET 请求
	response = api.Get("http://localhost:5000/file_api", params);
	// 处理响应
	//AfxMessageBox(_T("加密接口响应: ") + UTF8ToCString(response));
}

void CShowFileInfoDlg::FileDecryptionAPI(CString cmethod)
{
	CAPICall api;
	std::string response;
	// 可选：自定义请求头
	api.SetHeader("User-Agent: MyMFCApp/1.0");
	// 构造 GET 参数
	std::map<std::string, std::string> params;
	params["action"] = "decrypt";
	params["method"] = cmethod;
	params["file_path"] = m_filePath;
	params["key"] = key;
	params["output_dir"] = outpath;
	// 调用 GET 请求
	response = api.Get("http://localhost:5000/file_api", params);
	// 处理响应
	//AfxMessageBox(_T("解密接口响应: ") + UTF8ToCString(response));
}

void CShowFileInfoDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_comboTest.GetCurSel();
	if (index != CB_ERR)
	{
		CString text;
		m_comboTest.GetLBText(index, text);
		if(text == _T("BASE64 (不带Key)"))
		{
			m_key.SetWindowText(_T("N/A"));
			m_key.EnableWindow(FALSE); // 禁用编辑框
		}
		else
		{
			m_key.EnableWindow(TRUE); // 启用编辑框
			m_key.SetWindowText(_T("")); // 清空内容
		}
	}
}

void CShowFileInfoDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_comboTest.GetCurSel();
	if (index != CB_ERR)
	{
		CString text;
		m_comboTest.GetLBText(index, text);
		m_key.GetWindowText(key);
		m_outpath.GetWindowText(outpath);
		if(text == _T("AES (带Key)"))
		{
			if (key.IsEmpty() || outpath.IsEmpty())
			{
				AfxMessageBox(_T("请提供 AES 密钥，或你的输出目录为空!"));
				return;
			}
			if (PathFileExists(outpath))
			{
				FileDecryptionAPI("AES");
			}
			else
			{
				AfxMessageBox(_T("你的输出目录不存在!"));
				return;
			}
		}
		else if (text == _T("DES (带Key)") || outpath.IsEmpty())
		{
			if (key.IsEmpty())
			{
				AfxMessageBox(_T("请提供 DES 密钥，或你的输出目录为空!"));
				return;
			}
			if (PathFileExists(outpath))
			{
				FileDecryptionAPI("DES");
			}
			else
			{
				AfxMessageBox(_T("你的输出目录不存在!"));
				return;
			}
		}
		else if (text == _T("BASE64 (不带Key)"))
		{
			if (outpath.IsEmpty())
			{
				AfxMessageBox(_T("你的输出目录为空!"));
				return;
			}
			if (PathFileExists(outpath))
			{
				FileDecryptionAPI("base64");
			}
			else
			{
				AfxMessageBox(_T("你的输出目录不存在!"));
				return;
			}
		}
		else
		{
			AfxMessageBox(_T("未知选项"));
		}
	}
}

void CShowFileInfoDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	// 这里用的是“对话框A”自身窗口句柄
	SelectFolderModern(m_hWnd);
}


void CShowFileInfoDlg::SelectFolderModern(HWND hWnd)
{
	// 确保窗口有效且在 UI 线程
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(AfxGetApp()->m_pMainWnd); // optional

	IFileDialog* pFileDialog = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFileDialog));
	if (SUCCEEDED(hr) && pFileDialog)
	{
		DWORD dwOptions;
		if (SUCCEEDED(pFileDialog->GetOptions(&dwOptions)))
		{
			pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
		}

		// 使用对话框B的句柄作为 Owner
		hr = pFileDialog->Show(m_hWnd);
		if (SUCCEEDED(hr))
		{
			IShellItem* psi = nullptr;
			if (SUCCEEDED(pFileDialog->GetResult(&psi)) && psi)
			{
				PWSTR pszFilePath = nullptr;
				if (SUCCEEDED(psi->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)) && pszFilePath)
				{
					CString folderPath(pszFilePath);
					CoTaskMemFree(pszFilePath);
					// 确保控件有效再设置
					if (::IsWindow(m_outpath.m_hWnd))
						m_outpath.SetWindowText(folderPath);
					else
						AfxMessageBox(folderPath);
				}
				psi->Release();
			}
		}
		pFileDialog->Release();
	}
	else
	{
		AfxMessageBox(_T("无法创建文件对话框实例"));
	}
}