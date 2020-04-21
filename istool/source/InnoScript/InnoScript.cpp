// InnoScript.cpp: implementation of the CInnoScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InnoScript.h"
#include "Lines.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInnoScript::~CInnoScript() {
	Clear();
}

static const struct {
	CInnoScript::SECTION	m_sec;
	LPCTSTR					m_pszName;
} m_sectionnames[] = {
	CInnoScript::SEC_SETUP,				"Setup",
	CInnoScript::SEC_DIRS,				"Dirs",
	CInnoScript::SEC_FILES,				"Files",
	CInnoScript::SEC_ICONS,				"Icons",
	CInnoScript::SEC_INI,				"INI",
	CInnoScript::SEC_INSTALLDELETE,		"InstallDelete",
	CInnoScript::SEC_MESSAGES,			"Messages",
	CInnoScript::SEC_REGISTRY,			"Registry",
	CInnoScript::SEC_RUN,				"Run",
	CInnoScript::SEC_UNINSTALLDELETE,	"UninstallDelete",
	CInnoScript::SEC_UNINSTALLRUN,		"UninstallRun",
	CInnoScript::SEC_TYPES,				"Types",
	CInnoScript::SEC_COMPONENTS,		"Components",
	CInnoScript::SEC_TASKS,				"Tasks",
	CInnoScript::SEC_LANGOPTIONS,		"LangOptions",
	CInnoScript::SEC_CODE,				"Code",
	CInnoScript::SEC_LANGUAGES,			"Languages",
	CInnoScript::SEC_CUSTOMMESSAGES,	"CustomMessages",
	CInnoScript::PRJ_PRECOMPILESTEPS,	"_ISToolPreCompile",
	CInnoScript::PRJ_POSTCOMPILESTEPS,	"_ISToolPostCompile",
	CInnoScript::PRJ_DOWNLOAD,			"_ISToolDownload",
	CInnoScript::PRJ_ISTOOL,			"_ISTool",
	CInnoScript::SEC_NONE,				NULL,
};

LPCTSTR CInnoScript::GetSectionName(SECTION sec) {
	for(UINT n=0;m_sectionnames[n].m_pszName;n++) {
		if(m_sectionnames[n].m_sec == sec)
			return m_sectionnames[n].m_pszName;
	}
	return NULL;
}

CInnoScript::SECTION CInnoScript::GetSectionCode(LPCTSTR pszSection) {
	CString strSection(pszSection);
	int nLength = strSection.Trim().GetLength();
	if(nLength>2 && strSection[0]=='[' && strSection[nLength-1]==']')
		strSection = strSection.Mid(1,nLength-2);
	for(UINT n=0;m_sectionnames[n].m_pszName;n++) {
		if(!strSection.CompareNoCase(m_sectionnames[n].m_pszName))
			return m_sectionnames[n].m_sec;
	}
	return SEC_NONE;
}

void CInnoScript::Clear() {
	for(long i=0;i<(long)m_lines.GetCount();i++)
		delete m_lines[i];
	m_lines.RemoveAll();
}

bool CInnoScript::LoadScript(LPCTSTR pszFileName) {
	FILE* pFile = fopen(pszFileName,"r");
	if(!pFile)
		return false;

	return LoadScript(pFile);
}

bool CInnoScript::AddLine(SECTION& sec,CString& strLine) {
	CLine* pLine = NULL;
	DWORD dwUserFlags = 0;

	strLine.TrimRight();

	// Concatenation
#if 1
	if(!strLine.Right(2).Compare(" \\")) {
		dwUserFlags |= CLine::FLG_CONCAT;
		strLine = strLine.Left(strLine.GetLength()-2);
		strLine.ReleaseBuffer();
	}
#endif
	//

	int nFirstNonWhite = 0;
	while(strLine[nFirstNonWhite] && iswspace(strLine[nFirstNonWhite])) nFirstNonWhite++;

	try {
		if(strLine[0]=='[') {
			if(strLine.GetLength()>1 && strLine[1]=='/') {
				// End of section
				sec = SEC_NONE;
			} else {
				UINT nRow = 0;
				while(m_sectionnames[nRow].m_sec!=SEC_NONE) {
					UINT nLength = _tcslen(m_sectionnames[nRow].m_pszName);
					if(!strLine.Mid(1,nLength).CompareNoCase(m_sectionnames[nRow].m_pszName) && strLine.GetAt(nLength+1)==']') {
						sec = m_sectionnames[nRow].m_sec;
						break;
					}
					nRow++;
				}
				if(m_sectionnames[nRow].m_sec==SEC_NONE) {
					sec = SEC_NONE;
					pLine = new CLineComment(sec,strLine);
				}
			}
		} else if(strLine[0]=='#' || strLine[nFirstNonWhite]=='#') {
			pLine = new CLineHash(sec,strLine);
		} else if(strLine[0]==';' || !strLine[0] || sec==SEC_NONE || sec==SEC_CODE) {
			pLine = new CLineComment(sec,strLine);
		} else {
			if(sec==SEC_SETUP || sec==SEC_MESSAGES || sec==PRJ_ISTOOL || sec==SEC_LANGOPTIONS || sec==SEC_CUSTOMMESSAGES)
				pLine = new CLineSetup(sec,strLine);
			else
				pLine = new CLineParam(sec,strLine);
		}
	}
	catch(...) {
		if(pLine) delete pLine;
		pLine = new CLineComment(sec,strLine);
		nFirstNonWhite = 0;
	}
	// Insert the newly added line
	if(pLine) {
		if(nFirstNonWhite>0)
			pLine->SetIndent(strLine.Left(nFirstNonWhite));
		pLine->m_dwUserFlags |= dwUserFlags;
		bool bFound = false;
		for(long i=0;i<(long)m_lines.GetCount();i++) {
			if(bFound && m_lines[i]->GetSection() != pLine->GetSection()) {
#if 0
				// Rewind
				while(i>0 && m_lines[i-1]->GetSection()==pLine->GetSection()) {
					CString strLine;
					m_lines[i-1]->Write(strLine.GetBufferSetLength(100),100);
					strLine.ReleaseBuffer();
					if(strLine.Trim().IsEmpty())
						i--;
					else
						break;
				}
#endif
				m_lines.InsertAt(i,pLine);
				return true;
			}
			if(m_lines[i]->GetSection() == pLine->GetSection())
				bFound = true;
		}
		m_lines.Add(pLine);
	}
	return true;
}

bool CInnoScript::LoadScriptBuffer(LPSTR pszBuffer) {
	SECTION sec = SEC_NONE;
	LPCTSTR pszLineStart = pszBuffer;
	while(*pszBuffer) {
		if(*pszBuffer=='\n') {
			*pszBuffer++ = 0;
			AddLine(sec,CString(pszLineStart));
			//while(*pszBuffer && iswspace(*pszBuffer)) pszBuffer++;
			pszLineStart = pszBuffer;
		} else
			pszBuffer++;
	}
	if(pszBuffer!=pszLineStart)
		AddLine(sec,CString(pszLineStart));

	return true;
}

bool CInnoScript::LoadScript(FILE* pFile) {
	CString strLine;
	char szLine[8192];
	SECTION sec = SEC_NONE;
	while(fgets(szLine,sizeof szLine,pFile)) {
		strLine = szLine;
		strLine.TrimRight();
		if(strLine.GetLength()>2 && strLine.Left(2)=="[/")
			continue;
		AddLine(sec,strLine);
	}
	fclose(pFile);
	return true;
}

bool CInnoScript::WriteScript(FILE* fp) {
	SECTION sec = SEC_NONE;
	for(long i=0;i<GetCount();i++) {
		CLine* pLine = m_lines[i];
		// New section?
		if(pLine->GetSection()!=sec) {
#if 0
			if(sec!=SEC_NONE)
				fprintf(fp,"[/%s]\r\n",m_sectionnames[sec].m_pszName);
#endif
			sec = pLine->GetSection();
			if(sec!=SEC_NONE) {
				fprintf(fp,"[%s]\r\n",m_sectionnames[sec].m_pszName);
			}
		}

		// Write line
		char szLine[5000];
		pLine->Write(szLine,5000);
		fprintf(fp,"%s\r\n",szLine);
	}
#if 0
	if(sec!=SEC_NONE)
		fprintf(fp,"[/%s]\r\n",m_sectionnames[sec].m_pszName);
#endif
	return true;
}

bool CInnoScript::WriteScript(LPCTSTR pszName) {
	FILE* fp = fopen(pszName,"wb");
	bool bRet = false;
	if(fp) {
		bRet = WriteScript(fp);
		fclose(fp);
	}
	return bRet;
}

void CInnoScript::MoveUp(CLine* pLine) {
	for(long i=1;i<GetCount();i++) {
		if(m_lines[i]==pLine) {
			m_lines.RemoveAt(i);
			while(i>1 && m_lines[i-1]->GetComment()) i--;
			m_lines.InsertAt(i-1,pLine);
			break;
		}
	}
}

void CInnoScript::MoveDown(CLine* pLine) {
	for(long i=0;i<GetCount()-1;i++) {
		if(m_lines[i]==pLine) {
			m_lines.RemoveAt(i);
			while(i<GetCount() && m_lines[i]->GetComment()) i++;
			if(i+2>=GetCount())
				m_lines.Add(pLine);
			else
				m_lines.InsertAt(i+1,pLine);
			break;
		}
	}
}

long CInnoScript::GetLineNumber(CLine* pLineFind) {
	SECTION sec = SEC_NONE;
	long nLine = -1;
	for(long i=0;i<GetCount();i++) {
		CLine* pLine = m_lines[i];
		nLine++;
		// New section?
		if(pLine->GetSection()!=sec) {
			sec = pLine->GetSection();
			if(sec!=SEC_NONE) nLine++;
		}
		if(pLine==pLineFind)
			return nLine;
	}
	return -1;
}

long CInnoScript::GetLineNumber(SECTION secFind) {
	SECTION sec = SEC_NONE;
	long nLine = -1;
	for(long i=0;i<GetCount();i++) {
		CLine* pLine = m_lines[i];
		nLine++;
		// New section?
		if(pLine->GetSection()!=sec) {
			sec = pLine->GetSection();
			if(sec!=SEC_NONE) nLine++;
		}
		if(sec==secFind)
			return nLine;
	}
	return -1;
}

bool CInnoScript::AddHead(CLine* pLine) {
	m_lines.InsertAt(0,pLine);
	return true;
}

bool CInnoScript::AddTail(CLine* pLine) {
	m_lines.Add(pLine);
	return true;
}

long CInnoScript::GetSize() {
	return m_lines.GetCount();
}

long CInnoScript::GetCount() {
	return GetSize();
}

CInnoScript::CLine* CInnoScript::operator[](long iItem) {
	ATLASSERT(iItem < (long)m_lines.GetCount());
	return m_lines[iItem];
}

const CInnoScript& CInnoScript::operator=(CInnoScript& in) {
	Clear();
	for(long i=0;i<in.GetSize();i++)
		m_lines.Add(in.m_lines[i]->Copy());
	return *this;
}
