// Setup1.h : header file

#pragma once

#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupSpanning dialog
class CSetupSpanning : 
	public CPropertyPageImpl<CSetupSpanning>,
	public CMyPropertyPageBase<CSetupSpanning>,
	public CWinDataExchange<CSetupSpanning>,
	public Henden::CToolTipDialog<CSetupSpanning>
{
public:
	enum { IDD = IDD_SETUP_DISKSPANNING };

	BEGIN_MSG_MAP(CSetupSpanning)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupSpanning>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupSpanning>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupSpanning>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_DISKSPANNING, BN_CLICKED, OnDiskSpanning)
		COMMAND_HANDLER(IDC_DISKCLUSTERSIZE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_RESERVEBYTES, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_DISKSLICESIZE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_SLICESPERDISK, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupSpanning)
		DDX_CHECK(IDC_DISKSPANNING, m_bDiskSpanning)
		DDX_INT(IDC_DISKCLUSTERSIZE, m_nDiskClusterSize)
		DDX_INT(IDC_RESERVEBYTES, m_nReserveBytes)
		DDX_TEXT(IDC_DISKSLICESIZE, m_strDiskSliceSize)
		DDX_TEXT(IDC_SLICESPERDISK, m_strSlicesPerDisk)
	END_DDX_MAP()

	BOOL			m_bDiskSpanning;
	UINT			m_nDiskClusterSize;
	UINT			m_nReserveBytes;
	CString			m_strDiskSliceSize, m_strSlicesPerDisk;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"DiskSpanning");

		GetDlgItem(IDC_DISKCLUSTERSIZE).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_RESERVEBYTES).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_DISKSLICESIZE).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_SLICESPERDISK).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC1).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC4).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC5).EnableWindow(m_bDiskSpanning);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		script.SetPropertyBool("DiskSpanning",m_bDiskSpanning ? true : false);
		script.SetPropertyNumber("DiskClusterSize",m_nDiskClusterSize);
		script.SetPropertyNumber("ReserveBytes",m_nReserveBytes);
		script.SetPropertyString("DiskSliceSize",m_strDiskSliceSize);
		script.SetPropertyString("SlicesPerDisk",m_strSlicesPerDisk);

		return PSNRET_NOERROR;
	}

	CSetupSpanning(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew),
		CPropertyPageImpl<CSetupSpanning>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bDiskSpanning				= script.GetPropertyBool("DiskSpanning");
		m_nDiskClusterSize			= script.GetPropertyNumber("DiskClusterSize");
		m_nReserveBytes				= script.GetPropertyNumber("ReserveBytes");
		m_strDiskSliceSize			= script.GetPropertyString("DiskSliceSize");
		m_strSlicesPerDisk			= script.GetPropertyString("SlicesPerDisk");
	}

	LRESULT OnDiskSpanning(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);

		GetDlgItem(IDC_DISKCLUSTERSIZE).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_RESERVEBYTES).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_DISKSLICESIZE).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_SLICESPERDISK).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC1).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC4).EnableWindow(m_bDiskSpanning);
		GetDlgItem(IDC_STATIC5).EnableWindow(m_bDiskSpanning);
		return 0;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_DISKSPANNING, _L("Help|DiskSpanning|DiskSpanning","If set to yes, the disk spanning feature will be enabled. Instead of storing all the compressed file data inside SETUP.EXE, the compiler will split it into multiple SETUP-*.BIN files -- known as \"slices\" -- suitable for copying onto separate floppy disks, CD-ROMs, or DVD-ROMs."))
		TOOLTIP_HANDLER(IDC_DISKCLUSTERSIZE, _L("Help|DiskSpanning|DiskClusterSize","This specifies the cluster size of the disk media. The Setup Compiler needs to know this in order to properly fill each disk to capacity."))
		TOOLTIP_HANDLER(IDC_RESERVEBYTES, _L("Help|DiskSpanning|ReserveBytes","This specifies the minimum number of free bytes to reserve on the first disk. This is useful if you have to copy other files onto the first disk that aren't part of the setup program, such as a Readme file."))
		TOOLTIP_HANDLER(IDC_DISKSLICESIZE, _L("Help|DiskSpanning|DiskSliceSize","This specifies the maximum number of bytes per disk slice (SETUP-*.BIN file)."))
		TOOLTIP_HANDLER(IDC_SLICESPERDISK, _L("Help|DiskSpanning|SlicesPerDisk","The number of SETUP-*.BIN files to create for each disk. If this is 1 (the default setting), the files will be named SETUP-x.BIN, where x is the disk number. If this is greater than 1, the files will be named SETUP-xy.BIN, where x is the disk number and y is a unique letter."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*				m_pDoc;
	const bool			m_bNew;
};
