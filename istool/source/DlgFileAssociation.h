// WTL
#pragma once

#include "MyDoc.h"
#include "IconComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFileAssociation dialog

class CDlgFileAssociation : 
	public CDialogImpl<CDlgFileAssociation>, 
	public CWinDataExchange<CDlgFileAssociation>,
	public CMyDialogBase<CDlgFileAssociation>
{
public:
	enum { IDD = IDD_FILE_ASSOCIATION };

	BEGIN_MSG_MAP(CDlgFileAssociation)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_FA_FILE_EXTENSION,EN_KILLFOCUS,OnKillFocusFaFileExtension)
		COMMAND_HANDLER(IDC_FA_ICON_FILENAME,CBN_DROPDOWN,OnDropdownFAIconFilename)
		COMMAND_HANDLER(IDC_FA_ICON_FILENAME,CBN_KILLFOCUS,OnKillfocusFAIconFilename)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgFileAssociation>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgFileAssociation)
		DDX_TEXT(IDC_FA_FILE_EXTENSION, m_strFileExtension)
		DDX_TEXT(IDC_FA_DOCUMENT_DESCRIPTION, m_strDocumentDescription)
		DDX_TEXT(IDC_FA_TYPE_NAME, m_strTypeName)
		DDX_TEXT(IDC_FA_ASSOCIATE_WITH, m_strAssociateWith)
		DDX_TEXT(IDC_FA_ICON_FILENAME, m_strIconFilename)
		DDX_CBINDEX(IDC_FA_ICON_INDEX, m_nIconIndex)
	END_DDX_MAP()

	CComboBox			m_wndIconFilename;
	CLargeIconComboBox	m_wndIconIndex;
	CComboBox			m_wndAssociateWith;
	CString				m_strFileExtension;
	CString				m_strDocumentDescription;
	CString				m_strTypeName;
	CString				m_strAssociateWith;
	CString				m_strIconFilename;
	int					m_nIconIndex;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			DoDataExchange(DDX_SAVE);
			if(m_strFileExtension.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Missing file extension."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				GetDlgItem(IDC_FA_FILE_EXTENSION).SetFocus();
				return 0;
			}

			if(m_strTypeName.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Missing type name."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				GetDlgItem(IDC_FA_TYPE_NAME).SetFocus();
				return 0;
			}

			if(m_strDocumentDescription.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Missing document description."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				GetDlgItem(IDC_FA_DOCUMENT_DESCRIPTION).SetFocus();
				return 0;
			}

			if(m_strAssociateWith.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Missing file to associate with."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				GetDlgItem(IDC_FA_ASSOCIATE_WITH).SetFocus();
				return 0;
			}

			CString strLine;

			strLine.Format("Root: HKCR; SubKey: %s; ValueType: string; ValueData: %s; Flags: uninsdeletekey",m_strFileExtension,m_strTypeName);
			m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_REGISTRY,strLine));

			strLine.Format("Root: HKCR; SubKey: %s; ValueType: string; ValueData: %s; Flags: uninsdeletekey",m_strTypeName,m_strDocumentDescription);
			m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_REGISTRY,strLine));

			strLine.Format("Root: HKCR; SubKey: %s; ValueType: string; ValueData: %s; Flags: uninsdeletevalue",
				m_strTypeName+"\\Shell\\Open\\Command",
				"\"\""+ m_strAssociateWith + "\"\" \"%1\"\"\"");
			m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_REGISTRY,strLine));

			// Default Icon
			if(!m_strIconFilename.IsEmpty()) {
				strLine.Format("Root: HKCR; Subkey: %s; ValueType: string; ValueData: %s,%d; Flags: uninsdeletevalue",
					m_strTypeName+"\\DefaultIcon",
					m_strIconFilename,
					m_nIconIndex
				);
				m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_REGISTRY,strLine));
			}

			m_pDoc->SetModifiedFlag();
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"FileAssoc");
		CenterWindow(GetParent());

		m_wndIconFilename.Attach(GetDlgItem(IDC_FA_ICON_FILENAME));
		m_wndIconIndex.SubclassWindow(GetDlgItem(IDC_FA_ICON_INDEX));
		m_wndAssociateWith.Attach(GetDlgItem(IDC_FA_ASSOCIATE_WITH));
		m_nIconIndex = -1;

		DoDataExchange(DDX_LOAD);
		m_pDoc->AddFilesExeFiles(m_wndAssociateWith);
		return TRUE;
	}

	CDlgFileAssociation(CMyDoc* pDoc) : m_pDoc(pDoc) {}

	LRESULT OnKillFocusFaFileExtension(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		if(!m_strFileExtension.IsEmpty() && m_strFileExtension[0]!='.') {
			m_strFileExtension = "."+m_strFileExtension;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	LRESULT OnDropdownFAIconFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_wndIconFilename.GetCount()<=0) {
			CWaitCursor wait;
			m_pDoc->AddFiles(m_wndIconFilename);
		}
		return 0;
	}

	LRESULT OnKillfocusFAIconFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_wndIconIndex.ResetContent();

		if(m_strIconFilename.IsEmpty()) {
			m_wndIconIndex.EnableWindow(FALSE);
			return 0;
		}

		// Go through files and find this file
		CScriptList files;
		script.GetList(CInnoScript::SEC_FILES,files);
		for(int nPos=0;nPos<files.GetSize();nPos++) {
			CScriptLine* pFile = files[nPos];
			CString strName;
			script.GetDestName(pFile,strName);

			if(!m_strIconFilename.CompareNoCase(strName)) {
				m_wndIconIndex.ResetContent();

				LPCTSTR pszFilename = pFile->GetParameter("Source");
				if(CMyUtils::IsFile(pszFilename)) {
					int nNum = (int)ExtractIcon(_Module.GetResourceInstance(), pszFilename, (UINT) -1);
					for(int i=0; i<nNum; i++) {
						HICON hIcon = ExtractIcon(_Module.GetResourceInstance(), pszFilename, i);
						m_wndIconIndex.AddIcon(hIcon,i);
					}
					m_wndIconIndex.SetCurSel(m_nIconIndex);
				}


				m_wndIconIndex.EnableWindow(TRUE);
				return 0;
			}
		}
		m_wndIconIndex.EnableWindow(FALSE);
		return 0;
	}

protected:
	CMyDoc*		m_pDoc;
};
