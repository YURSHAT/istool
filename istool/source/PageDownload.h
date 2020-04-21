#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageDownload dialog

class CPageDownload : 
	public CPropertyPageImpl<CPageDownload>,
	public CMyPropertyPageBase<CPageDownload>,
	public CWinDataExchange<CPageDownload>,
	public Henden::CToolTipDialog<CPageDownload>
{
public:
	enum { IDD = IDD_DOWNLOAD };

	BEGIN_MSG_MAP(CPageDownload)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageDownload>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageDownload>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageDownload>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_DOWNLOAD_SOURCE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_DOWNLOAD_DESTDIR, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_DOWNLOAD_DESTNAME, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageDownload)
		DDX_TEXT(IDC_DOWNLOAD_SOURCE, m_strSource)
		DDX_TEXT(IDC_DOWNLOAD_DESTDIR, m_strDestDir)
		DDX_TEXT(IDC_DOWNLOAD_DESTNAME, m_strDestName)
	END_DDX_MAP()

	CString		m_strSource, m_strDestDir, m_strDestName;
	CComboBox2	m_wndDestDir;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Download");
		m_wndDestDir.SubclassWindow(GetDlgItem(IDC_DOWNLOAD_DESTDIR));
		m_pDoc->AddDirConstants(m_wndDestDir);
		m_pDoc->AddDirs(m_wndDestDir);
		theApp.m_shell.SHAutoComplete(GetDlgItem(IDC_DOWNLOAD_SOURCE),SHACF_URLALL);

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];

			if(m_strSource.CompareNoCase(SAFESTR(pItem->GetParameter("Source"))))
				m_strSource.Empty();
			if(m_strDestDir.CompareNoCase(SAFESTR(pItem->GetParameter("DestDir"))))
				m_strDestDir.Empty();
			if(m_strDestName.CompareNoCase(SAFESTR(pItem->GetParameter("DestName"))))
				m_strDestName.Empty();
		}
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize()==1;

		if(bForce) {
			if(m_strSource.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Download|NeedSource","You must enter download source."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
			if(m_strDestDir.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Download|NeedDestDir","You must enter destination directory."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
			if(m_strDestName.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Download|NeedDestName","You must enter the destination name."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
			if(!m_strDestDir.Left(5).CompareNoCase("{app}")) {
				AtlMessageBox(m_hWnd,_L("Download|WarnAppDir","Using the application directory as destination is not recommended.\n\nThe directory might not have been created yet."),IDR_MAINFRAME,MB_OK|MB_ICONWARNING);
			}
		}

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];
			if(bForce || !m_strSource.IsEmpty()) pItem->SetParameter("Source",m_strSource);
			if(bForce || !m_strDestDir.IsEmpty()) pItem->SetParameter("DestDir",m_strDestDir);
			if(bForce || !m_strDestName.IsEmpty()) pItem->SetParameter("DestName",m_strDestName);

			// Flags
			//CInnoScriptEx::SetFlag(pItem,"Flags","abortonerror",m_bAbortOnError);
			//CInnoScriptEx::SetFlag(pItem,"Flags","runminimized",m_bRunMinimized);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	CString m_strTitle;
	CPageDownload(CScriptList& list,CMyDoc* pDoc,const bool bNew) : 
		m_list(list), m_pDoc(pDoc), m_bNew(bNew), m_wndDestDir(pDoc)
	{
		m_strTitle = _L("DialogTitles|Download","Download");
		SetTitle((LPCTSTR)m_strTitle);

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CInnoScript::CLine* pItem = m_list[nPos];
			m_strSource = pItem->GetParameter("Source");
			m_strDestDir = pItem->GetParameter("DestDir");
			m_strDestName = pItem->GetParameter("DestName");
			break;
		}
	}

	CScriptList&	m_list;
	CMyDoc*			m_pDoc;
	const bool		m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_DOWNLOAD_SOURCE,_L("Help|Download|Source","The address of the file you want to download. Must begin with http:// or ftp://"))
		TOOLTIP_HANDLER(IDC_DOWNLOAD_DESTDIR,_L("Help|Download|DestDir","The destination directory for the file."))
		TOOLTIP_HANDLER(IDC_DOWNLOAD_DESTNAME,_L("Help|Download|DestName","The destination name for the file."))
	END_TOOLTIP_MAP()
};
