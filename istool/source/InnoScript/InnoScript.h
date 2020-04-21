// InnoScript.h: interface for the CInnoScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_INNOSCRIPT_H)
#define _INNOSCRIPT_H

#include <windows.h>
#include <stdio.h>

class CInnoScript {
public:
	class CLine;
	class CLineComment;
	class CLineHash;
	class CLineSetup;
	class CLineParam;

	virtual ~CInnoScript();

	enum SECTION {
		SEC_SETUP = 0,
		SEC_DIRS,
		SEC_FILES,
		SEC_ICONS,
		SEC_INI,
		SEC_INSTALLDELETE,
		SEC_MESSAGES,
		SEC_REGISTRY,
		SEC_RUN,
		SEC_UNINSTALLDELETE,
		SEC_UNINSTALLRUN,
		SEC_TYPES,
		SEC_COMPONENTS,
		SEC_TASKS,
		SEC_LANGOPTIONS,
		SEC_CODE,
		SEC_LANGUAGES,
		SEC_CUSTOMMESSAGES,
		PRJ_PRECOMPILESTEPS,
		PRJ_POSTCOMPILESTEPS,
		PRJ_DOWNLOAD,
		PRJ_ISTOOL,
		SEC_NONE,
	};

	bool LoadScript(LPCTSTR pszFileName);
	bool LoadScript(FILE* fp);
	bool LoadScriptBuffer(LPSTR pszBuffer);
	bool AddLine(SECTION& sec,CString& strLine);
	bool AddHead(CLine* pLine);
	bool AddTail(CLine* pLine);
	bool WriteScript(LPCTSTR pszName);
	bool WriteScript(FILE* fp);
	long GetLineNumber(CLine* pLine);
	long GetLineNumber(SECTION sec);

	void Clear();
	void MoveUp(CLine* pLine);
	void MoveDown(CLine* pLine);
	
	static LPCTSTR	GetSectionName(SECTION sec);
	static SECTION	GetSectionCode(LPCTSTR pszSection);

	long GetSize();
	long GetCount();
	CLine* operator[](long iItem);
	const CInnoScript& operator=(CInnoScript& in);

protected:
	class CToken;
	class CParam;

	CAtlArray<CLine*>	m_lines;
};

#endif // !defined(_INNOSCRIPT_H)
