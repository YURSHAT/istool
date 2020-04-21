#pragma once

#include "MyDoc.h"
#include "Edit2.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageRegistry dialog

class CPageRegistry : 
	public CPropertyPageImpl<CPageRegistry>,
	public CMyPropertyPageBase<CPageRegistry>,
	public CWinDataExchange<CPageRegistry>,
	public Henden::CToolTipDialog<CPageRegistry>
{
public:
	enum { IDD = IDD_REGISTRY };

	BEGIN_MSG_MAP(CPageRegistry)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageRegistry>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageRegistry>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageRegistry>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_VALUETYPE, CBN_SELCHANGE, OnSelChangeValueType)
		COMMAND_HANDLER(IDC_ROOT, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETEVALUE, BN_CLICKED, OnUninsdeletevalue)
		COMMAND_HANDLER(IDC_UNINSDELETEKEY, BN_CLICKED, OnUninsdeletekey)
		COMMAND_HANDLER(IDC_ROOT, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_VALUENAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_VALUEDATA, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSCLEARVALUE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETEKEYIFEMPTY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SUBKEY, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_SUBKEY, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_PRESERVESTRINGTYPE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGISTRY_DELETEKEY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGISTRY_DELETEVALUE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGISTRY_NOERROR, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_REGISTRY_DONT_CREATE_KEY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETEKEY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETEVALUE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_PERMISSIONS, EN_CHANGE, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageRegistry)
		DDX_CBINDEX(IDC_ROOT, m_nRoot)
		DDX_TEXT(IDC_SUBKEY, m_strSubkey)
		DDX_CHECK(IDC_CREATEVALUEIFDOESNTEXIST, m_nCreateValueIfDoesntExist)
		DDX_CBINDEX(IDC_VALUETYPE, m_nValueType)
		DDX_CHECK(IDC_UNINSCLEARVALUE, m_nUninsClearValue)
		DDX_CHECK(IDC_UNINSDELETEKEY, m_nUninsDeleteKey)
		DDX_CHECK(IDC_UNINSDELETEKEYIFEMPTY, m_nUninsDeleteKeyIfEmpty)
		DDX_CHECK(IDC_UNINSDELETEVALUE, m_nUninsDeleteValue)
		DDX_TEXT(IDC_VALUEDATA, m_strValueData)
		DDX_TEXT(IDC_VALUENAME, m_strValueName)
		DDX_CHECK(IDC_PRESERVESTRINGTYPE, m_bPreserveStringType)
		DDX_CHECK(IDC_REGISTRY_DELETEKEY, m_nDeleteKey)
		DDX_CHECK(IDC_REGISTRY_DELETEVALUE, m_nDeleteValue)
		DDX_CHECK(IDC_REGISTRY_NOERROR, m_nNoError)
		DDX_CHECK(IDC_REGISTRY_DONT_CREATE_KEY, m_nDontCreateKey)
		DDX_TEXT(IDC_PERMISSIONS, m_strPermissions)
	END_DDX_MAP()

	CComboBox2	m_wndSubkey;
	CEdit2	m_wndValueName;
	CEdit2	m_wndValueData;
	int		m_nRoot;
	CString	m_strSubkey;
	int		m_nCreateValueIfDoesntExist;
	int		m_nValueType;
	int		m_nUninsClearValue;
	int		m_nUninsDeleteKey;
	int		m_nUninsDeleteKeyIfEmpty;
	int		m_nUninsDeleteValue;
	CString	m_strValueData;
	CString	m_strValueName;
	BOOL	m_bPreserveStringType;
	int		m_nDeleteKey;
	int		m_nDeleteValue;
	int		m_nNoError;
	int		m_nDontCreateKey;
	CString	m_strPermissions;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Registry");
		m_wndSubkey.SubclassWindow(GetDlgItem(IDC_SUBKEY));
		m_wndValueName.SubclassWindow(GetDlgItem(IDC_VALUENAME));
		m_wndValueData.SubclassWindow(GetDlgItem(IDC_VALUEDATA));

		CComboBox box;
		box.Attach(GetDlgItem(IDC_ROOT));
		box.AddString("HKEY_CLASSES_ROOT");
		box.AddString("HKEY_CURRENT_USER");
		box.AddString("HKEY_LOCAL_MACHINE");
		box.AddString("HKEY_USERS");
		box.AddString("HKEY_CURRENT_CONFIG");
		box.Detach();

		box.Attach(GetDlgItem(IDC_VALUETYPE));
		box.AddString("none");
		box.AddString("string");
		box.AddString("expandsz");
		box.AddString("multisz");
		box.AddString("dword");
		box.AddString("binary");
		box.AddString("qword");
		box.Detach();

		if(!_stricmp("none",SAFESTR(CInnoScriptEx::GetRegistryValueType(m_nValueType))))
			m_wndValueData.EnableWindow(FALSE);
		
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pItem = m_list[nPos];

			if(m_nRoot != CInnoScriptEx::GetRegistryRoot(SAFESTR(pItem->GetParameter("Root")))) m_nRoot = -1;
			if(m_strSubkey.CompareNoCase(SAFESTR(pItem->GetParameter("Subkey")))) m_strSubkey.Empty();
			if(m_nValueType != CInnoScriptEx::GetRegistryValueType(SAFESTR(pItem->GetParameter("ValueType")))) m_nValueType = -1;
			if(m_strValueName.CompareNoCase(SAFESTR(pItem->GetParameter("ValueName")))) m_strValueName.Empty();
			if(m_strValueData.CompareNoCase(SAFESTR(pItem->GetParameter("ValueData")))) m_strValueData.Empty();
			if(m_strPermissions.CompareNoCase(SAFESTR(pItem->GetParameter("Permissions")))) m_strPermissions.Empty();

			if(m_nCreateValueIfDoesntExist != (pItem->GetParameterFlag("Flags","createvalueifdoesntexist") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CREATEVALUEIFDOESNTEXIST).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nCreateValueIfDoesntExist = 2;
			}

			if(m_nUninsClearValue != (pItem->GetParameterFlag("Flags","uninsclearvalue") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSCLEARVALUE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsClearValue = 2;
			}

			if(m_nUninsDeleteKey != (pItem->GetParameterFlag("Flags","uninsdeletekey") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSDELETEKEY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsDeleteKey = 2;
			}

			if(m_nUninsDeleteKeyIfEmpty != (pItem->GetParameterFlag("Flags","uninsdeletekeyifempty") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSDELETEKEYIFEMPTY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsDeleteKeyIfEmpty = 2;
			}

			if(m_nUninsDeleteValue != (pItem->GetParameterFlag("Flags","uninsdeletevalue") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSDELETEVALUE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsDeleteValue = 2;
			}

			if(m_bPreserveStringType != (pItem->GetParameterFlag("Flags","preservestringtype") ? TRUE : FALSE)) {
				GetDlgItem(IDC_PRESERVESTRINGTYPE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_bPreserveStringType = 2;
			}

			if(m_nDeleteKey != (pItem->GetParameterFlag("Flags","deletekey") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGISTRY_DELETEKEY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nDeleteKey = 2;
			}

			if(m_nDeleteValue != (pItem->GetParameterFlag("Flags","deletevalue") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGISTRY_DELETEVALUE).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nDeleteValue = 2;
			}

			if(m_nNoError != (pItem->GetParameterFlag("Flags","noerror") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGISTRY_NOERROR).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nNoError = 2;
			}

			if(m_nNoError != (pItem->GetParameterFlag("Flags","noerror") ? TRUE : FALSE)) {
				GetDlgItem(IDC_REGISTRY_NOERROR).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nNoError = 2;
			}
		}
		DoDataExchange(DDX_LOAD);

		m_pDoc->AddRegistrySubkeys(m_wndSubkey);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_list.GetSize()==1;

		if(bForce) {
			if(m_nRoot==-1) {
				AtlMessageBox(m_hWnd,_L("Invalid root."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}

			if(m_strSubkey.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Subkey must be entered."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
		}

		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pLine = m_list[nPos];

			CInnoScriptEx::SetString(pLine,bForce,"Root",CInnoScriptEx::GetRegistryRoot(m_nRoot));
			CInnoScriptEx::SetString(pLine,bForce,"Subkey",m_strSubkey);
			CInnoScriptEx::SetString(pLine,bForce,"ValueType",CInnoScriptEx::GetRegistryValueType(m_nValueType));
			CInnoScriptEx::SetString(pLine,bForce,"ValueName",m_strValueName);
			CInnoScriptEx::SetString(pLine,bForce,"ValueData",m_strValueData);
			CInnoScriptEx::SetString(pLine,bForce,"Permissions",m_strPermissions);

			// Flags
			CInnoScriptEx::SetFlag(pLine,"Flags","createvalueifdoesntexist",m_nCreateValueIfDoesntExist);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsclearvalue",m_nUninsClearValue);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsdeletekey",m_nUninsDeleteKey);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsdeletekeyifempty",m_nUninsDeleteKeyIfEmpty);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsdeletevalue",m_nUninsDeleteValue);
			CInnoScriptEx::SetFlag(pLine,"Flags","preservestringtype",m_bPreserveStringType);
			CInnoScriptEx::SetFlag(pLine,"Flags","deletekey",m_nDeleteKey);
			CInnoScriptEx::SetFlag(pLine,"Flags","deletevalue",m_nDeleteValue);
			CInnoScriptEx::SetFlag(pLine,"Flags","noerror",m_nNoError);
			CInnoScriptEx::SetFlag(pLine,"Flags","dontcreatekey",m_nDontCreateKey);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnSelChangeValueType(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		BOOL bEnable = _stricmp("none",SAFESTR(CInnoScriptEx::GetRegistryValueType(m_nValueType)));
		m_wndValueData.EnableWindow(bEnable);
		return 0;
	}

	LRESULT OnUninsdeletevalue(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_nUninsDeleteValue==1 && m_nUninsDeleteKey==1) {
			m_nUninsDeleteKey = 0;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	LRESULT OnUninsdeletekey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		if(m_nUninsDeleteValue==1 && m_nUninsDeleteKey==1) {
			m_nUninsDeleteValue = 0;
			DoDataExchange(DDX_LOAD);
		}
		return 0;
	}

	CString m_strTitle;
	CPageRegistry(CScriptList& list,CMyDoc* pDoc,const bool bNew) : 
		m_list(list), m_pDoc(pDoc), m_bNew(bNew),
		m_wndValueData(pDoc), m_wndValueName(pDoc), m_wndSubkey(pDoc)
	{
		m_strTitle = _L("DialogTitles|Registry","Registry");
		SetTitle((LPCTSTR)m_strTitle);

		m_nRoot = -1;
		m_nCreateValueIfDoesntExist = FALSE;
		m_nValueType = -1;
		m_nUninsClearValue = FALSE;
		m_nUninsDeleteKey = FALSE;
		m_nUninsDeleteKeyIfEmpty = FALSE;
		m_nUninsDeleteValue = FALSE;
		m_bPreserveStringType = FALSE;
		m_nDeleteKey = FALSE;
		m_nDeleteValue = FALSE;
		m_nNoError = FALSE;
		m_nDontCreateKey = FALSE;
		for(int nPos=0;nPos<m_list.GetSize();nPos++) {
			CScriptLine* pItem = m_list[nPos];
			m_nRoot = CInnoScriptEx::GetRegistryRoot(pItem->GetParameter("Root"));
			m_strSubkey = pItem->GetParameter("Subkey");
			m_nValueType = CInnoScriptEx::GetRegistryValueType(pItem->GetParameter("ValueType"));
			m_strValueName = pItem->GetParameter("ValueName");
			m_strValueData = pItem->GetParameter("ValueData");
			m_strPermissions = pItem->GetParameter("Permissions");

			if(pItem->GetParameterFlag("Flags","createvalueifdoesntexist")) m_nCreateValueIfDoesntExist = 1;
			if(pItem->GetParameterFlag("Flags","uninsclearvalue")) m_nUninsClearValue = 1;
			if(pItem->GetParameterFlag("Flags","uninsdeletekey")) m_nUninsDeleteKey = 1;
			if(pItem->GetParameterFlag("Flags","uninsdeletekeyifempty")) m_nUninsDeleteKeyIfEmpty = 1;
			if(pItem->GetParameterFlag("Flags","uninsdeletevalue")) m_nUninsDeleteValue = 1;
			if(pItem->GetParameterFlag("Flags","preservestringtype")) m_bPreserveStringType = 1;
			if(pItem->GetParameterFlag("Flags","deletekey")) m_nDeleteKey = 1;
			if(pItem->GetParameterFlag("Flags","deletevalue")) m_nDeleteValue = 1;
			if(pItem->GetParameterFlag("Flags","noerror")) m_nNoError = 1;
			if(pItem->GetParameterFlag("Flags","dontcreatekey")) m_nDontCreateKey = 1;
			break;
		}
	}

	CScriptList&	m_list;
	CMyDoc*			m_pDoc;
	const bool		m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_ROOT,_L("Help|Registry|Root","The root key."))
		TOOLTIP_HANDLER(IDC_SUBKEY,_L("Help|Registry|Subkey","The subkey name, which can include constants."))
		TOOLTIP_HANDLER(IDC_CREATEVALUEIFDOESNTEXIST,_L("Help|Registry|CreateValueIfDoesntExist","When this flag is specified, Setup will create the value only if a value of the same name doesn't already exist. This flag has no effect if the data type is none, or if you specify the deletevalue flag."))
		TOOLTIP_HANDLER(IDC_VALUETYPE,_L("Help|Registry|ValueType","The data type of the value."))
		TOOLTIP_HANDLER(IDC_UNINSCLEARVALUE,_L("Help|Registry|UninsClearValue","When the program is uninstalled, set the value's data to a null string (type REG_SZ). This flag cannot be combined with the uninsdeletekey flag."))
		TOOLTIP_HANDLER(IDC_UNINSDELETEKEY,_L("Help|Registry|UninsDeleteKey","When the program is uninstalled, delete the entire key, including all values and subkeys in it. It obviously wouldn't be a good idea to use this on a key that is used by Windows itself. You should only use this on keys private to your application.\r\n\r\nTo prevent disasters, this flag is ignored during installation if Subkey is blank or contains only backslashes."))
		TOOLTIP_HANDLER(IDC_UNINSDELETEKEYIFEMPTY,_L("Help|Registry|UninsDeleteKeyIfEmpty","When the program is uninstalled, delete the key if it has no values or subkeys left in it. This flag can be combined with uninsdeletevalue.\r\n\r\nTo prevent disasters, this flag is ignored during installation if Subkey is blank or contains only backslashes."))
		TOOLTIP_HANDLER(IDC_UNINSDELETEVALUE,_L("Help|Registry|UninsDeleteValue","Delete the value when the program is uninstalled. This flag can be combined with uninsdeletekeyifempty."))
		TOOLTIP_HANDLER(IDC_VALUEDATA,_L("Help|Registry|ValueData","The data for the value. If the ValueType parameter is string, expandsz, or multisz, this is a string that can include constants. If the data type is dword, this can be a decimal integer (e.g. \"123\"), a hexadecimal integer (e.g. \"$7B\"), or a constant which resolves to an integer. If the data type is binary, this is a sequence of hexadecimal bytes in the form: \"00 ff 12 34\". If the data type is none, this is ignored."))
		TOOLTIP_HANDLER(IDC_VALUENAME,_L("Help|Registry|ValueName","The name of the value to create, which can include constants. If this is blank, it will write to the \"Default\" value. If the ValueType parameter is set to none, this parameter is ignored."))
		TOOLTIP_HANDLER(IDC_PRESERVESTRINGTYPE,_L("Help|Registry|PreserveStringType","This is only applicable when the ValueType parameter is string or expandsz. When this flag is specified and the value did not already exist or the existing value isn't a string type (REG_SZ or REG_EXPAND_SZ), it will be created with the type specified by ValueType. If the value did exist and is a string type, it will be replaced with the same value type as the pre-existing value."))
		TOOLTIP_HANDLER(IDC_REGISTRY_DELETEKEY,_L("Help|Registry|DeleteKey","When this flag is specified, Setup will first try deleting the entire key if it exists, including all values and subkeys in it. If ValueType is not none, it will then create a new key and value.\r\n\r\nTo prevent disasters, this flag is ignored during installation if Subkey is blank or contains only backslashes."))
		TOOLTIP_HANDLER(IDC_REGISTRY_DELETEVALUE,_L("Help|Registry|DeleteValue","When this flag is specified, Setup will first try deleting the value if it exists. If ValueType is not none, it will then create the key if it didn't already exist, and the new value."))
		TOOLTIP_HANDLER(IDC_REGISTRY_NOERROR,_L("Help|Registry|NoError","Don't display an error message if Setup fails to create the key or value for any reason."))
		TOOLTIP_HANDLER(IDC_REGISTRY_DONT_CREATE_KEY,_L("Help|Registry|DontCreateKey","When this flag is specified, Setup will not attempt to create the key or any value if the key did not already exist on the user's system. No error message is displayed if the key does not exist.\r\n\r\nTypically this flag is used in combination with the uninsdeletekey flag, for deleting keys during uninstallation but not creating them during installation."))
		TOOLTIP_HANDLER(IDC_PERMISSIONS,_L("Help|Registry|Permissions","Specifies additional permissions to grant in the registry key's ACL (access control list). It is not recommended that you use this parameter if you aren't familiar with ACLs or why you would need to change them, because misusing it could negatively impact system security."))
	END_TOOLTIP_MAP()
};
