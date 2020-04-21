// WTL complete
#pragma once

#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectFonts dialog

// Macros for TrueType portability
#define FS_2BYTE(p)  ( ((unsigned short)((p)[0]) << 8) |  (p)[1])
#define FS_4BYTE(p)  ( FS_2BYTE((p)+2) | ( (FS_2BYTE(p)+0L) << 16) )
#define SWAPW(a)        ((short) FS_2BYTE( (unsigned char FAR*)(&a) ))
#define SWAPL(a)        ((long) FS_4BYTE( (unsigned char FAR*)(&a) ))

#include <fcntl.h>
#include <io.h>

#define tag_NamingTable         0x656d616e        /* 'name' */

class CDlgSelectFonts : 
	public CDialogImpl<CDlgSelectFonts>, 
	public CMyDialogBase<CDlgSelectFonts>
{
public:
	enum { IDD = IDD_SELECTFONTS };

	BEGIN_MSG_MAP(CDlgSelectFonts)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		NOTIFY_HANDLER(IDC_FONTLIST,LVN_DELETEITEM,OnDeleteItem)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgSelectFonts>)
	END_MSG_MAP()

	CListViewCtrl	m_wndFontList;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			if(m_wndFontList.GetSelectedCount()==0) {
				EndDialog(IDCANCEL);
				return 0;
			}

			UINT nCount = m_wndFontList.GetItemCount();
			for(UINT nItem=0;nItem<nCount;nItem++) {
				if(m_wndFontList.GetItemState(nItem,LVIS_SELECTED)==LVIS_SELECTED) {
					CData* pData = (CData*)m_wndFontList.GetItemData(nItem);

					CString strLine;
					strLine.Format("Source: %s; DestDir: {fonts}; "
						"Flags: onlyifdoesntexist uninsneveruninstall%s; FontInstall: %s",
						pData->m_szFilePath,
						pData->m_bTrueType ? "" : " fontisnttruetype",
						pData->m_szFaceName);

					m_pDoc->GetScript().AddLine(new CInnoScript::CLineParam(CInnoScript::SEC_FILES,strLine));
				}
			}

			m_pDoc->SetModifiedFlag(TRUE);
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"SelectFonts");
		CenterWindow(GetParent());

		CWaitCursor wait;
		m_wndFontList.Attach(GetDlgItem(IDC_FONTLIST));
		m_wndFontList.InsertColumn(0,"Font Name",LVCFMT_LEFT,10,0);
		m_wndFontList.InsertColumn(1,"File Name",LVCFMT_LEFT,120,0);

		EnumTTF();
		//EnumFON();

		CRect rc;
		m_wndFontList.GetClientRect(rc);
		m_wndFontList.SetColumnWidth(0,rc.Width() - 120);
		m_wndFontList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
		return TRUE;
	}

	LRESULT OnDeleteItem(int,NMHDR* pNMHDR,BOOL&) {
		NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
		
		CData* pData = (CData*)m_wndFontList.GetItemData(pNMListView->iItem);
		delete pData;
		return 0;
	}

	CDlgSelectFonts(CMyDoc* pDoc) : m_pDoc(pDoc) {}

	typedef short int16;
	typedef unsigned short uint16;
	typedef long int32;
	typedef unsigned long uint32;
	typedef long sfnt_TableTag;

	typedef struct {
		uint16 platformID;
		uint16 specificID;
		uint16 languageID;
		uint16 nameID;
		uint16 length;
		uint16 offset;
	} sfnt_NameRecord;

	typedef struct {
		uint16 format;
		uint16 count;
		uint16 stringOffset;
	} sfnt_NamingTable;

	typedef struct {
		sfnt_TableTag   tag;
		uint32          checkSum;
		uint32          offset;
		uint32          length;
	} sfnt_DirectoryEntry;

	typedef struct {
		int32 version;                  /* 0x10000 (1.0) */
		uint16 numOffsets;              /* number of tables */
		uint16 searchRange;             /* (max2 <= numOffsets)*16 */
		uint16 entrySelector;           /* log2 (max2 <= numOffsets) */
		uint16 rangeShift;              /* numOffsets*16-searchRange*/
		sfnt_DirectoryEntry table[1];   /* table[numOffsets] */
	} sfnt_OffsetTable;


	LPSTR ReadLongName(LPCTSTR pszFile) {   
		static char			lpszLongName[256];
		unsigned			i;
		char				namebuf[255];
		int                 fp;
		unsigned short      numNames;
		long				curseek;
		unsigned            cTables;
		sfnt_OffsetTable    OffsetTable;
		sfnt_DirectoryEntry Table;
		sfnt_NamingTable    NamingTable;
		sfnt_NameRecord     NameRecord;

		lpszLongName[0] = '\0';
		if ((fp = _open (pszFile, O_RDONLY | O_BINARY)) == -1)
			return NULL;

		/* First off, read the initial directory header on the TTF.  We're only
		* interested in the "numOffsets" variable to tell us how many tables
		* are present in this file.  
		*
		* Remember to always convert from Motorola format (Big Endian to 
		* Little Endian).
		*/
		_read (fp, &OffsetTable, sizeof (OffsetTable) - sizeof (sfnt_DirectoryEntry));
		cTables = (int) SWAPW (OffsetTable.numOffsets);

		for ( i = 0; i < cTables && i < 40; i++) {
			if ((_read (fp, &Table, sizeof (Table))) != sizeof(Table)) 
				return NULL;
			if(Table.tag == tag_NamingTable) {	/* defined in sfnt_en.h */
				/* Now that we've found the entry for the name table, seek to that
				* position in the file and read in the initial header for this
				* particular table.  See "True Type Font Files" for information
				* on this record layout.
				*/
				_lseek (fp, SWAPL (Table.offset), SEEK_SET);
				_read (fp, &NamingTable, sizeof (NamingTable));
				numNames = SWAPW(NamingTable.count);
				while (numNames--) {
					_read (fp, &NameRecord, sizeof (NameRecord));
					curseek = _tell(fp);
					if (SWAPW(NameRecord.platformID) == 1 && SWAPW(NameRecord.nameID) == 4) {
						_lseek (fp, SWAPW (NameRecord.offset) + 
							SWAPW(NamingTable.stringOffset) + 
							SWAPL(Table.offset), SEEK_SET);
						_read (fp, &namebuf, SWAPW(NameRecord.length));
						namebuf[SWAPW(NameRecord.length)] = '\0';
						lstrcpy(lpszLongName,namebuf);
						_lseek (fp, curseek, SEEK_SET);
					}
				}
				_close (fp);
				return lpszLongName;
			}
		}
		_close (fp);
		return FALSE;
	}

#ifdef TODO
	// Executable file header information
	typedef struct
	{
		WORD    wFileSignature;         // 0x5A4D
		WORD    wLengthMod512;          // bytes on last page
		WORD    wLength;                // 512 byte pages
		WORD    wRelocationTableItems;  
		WORD    wHeaderSize;            // Paragraphs
		WORD    wMinAbove;              // Paragraphs
		WORD    wDesiredAbove;          // Paragraphs
		WORD    wStackDisplacement;     // Paragraphs
		WORD    wSP;                    // On entry
		WORD    wCheckSum;
		WORD    wIP;                    // On entry
		WORD    wCodeDisplacement;      // Paragraphs
		WORD    wFirstRelocationItem;   // Offset from beginning
		WORD    wOverlayNumber;
		WORD    wReserved[ 16 ];
		LONG    lNewExeOffset;          
	} OLDEXE;

	// FontInfo structure located at beginning of a .fnt file
	typedef struct 
	{
			WORD  fontOrdinal;
			WORD  dfVersion;
			DWORD dfSize;
			char  dfCopyright[60];
			WORD  dfType;
			WORD  dfPoints;
			WORD  dfVertRes;
			WORD  dfHorizRes;
			WORD  dfAscent;
			WORD  dfInternalLeading;
			WORD  dfExternalLeading;
			BYTE  dfItalic;
			BYTE  dfUnderline;
			BYTE  dfStrikeOut;
			WORD  dfWeight;
			BYTE  dfCharSet;
			WORD  dfPixWidth;
			WORD  dfPixHeight;
			BYTE  dfPitchAndFamily;
			WORD  dfAvgWidth;
			WORD  dfMaxWidth;
			BYTE  dfFirstChar;
			BYTE  dfLastChar;
			BYTE  dfDefaultChar;
			BYTE  dfBreakChar;
			WORD  dfWidthBytes;
			DWORD dfDevice;
			DWORD dfFace;
			DWORD dfReserved;
			char  dfCharTable[100];        
	} FONTDIRENTRY;

	// New executable file header
	typedef struct
	{
		WORD  wNewSignature;    // 0x454e
		char  cLinkerVer;       // Version number 
		char  cLinkerRev;       // Revision number 
		WORD  wEntryOffset;     // Offset to Entry Table
		WORD  wEntrySize;       // Number of bytes in Entry Table
		long  lChecksum;        // 32 bit check sum for the file
		WORD  wFlags;           // Flag word 
		WORD  wAutoDataSegment; // Seg number for automatic data seg
		WORD  wHeapInit;        // Initial heap allocation; 0 for no heap
		WORD  wStackInit;       // Initial stack allocation; 0 for libraries
		WORD  wIPInit;          // Initial IP setting 
		WORD  wCSInit;          // Initial CS segment number
		WORD  wSPInit;          // Initial SP setting 
		WORD  wSSInit;          // Initial SS segment number
		WORD  wSegEntries;      // Count of segment table entries
		WORD  wModEntries;      // Entries in Module Reference Table 
		WORD  wNonResSize;      // Size of non-resident name table (bytes)
		WORD  wSegOffset;       // Offset of Segment Table 
		WORD  wResourceOffset;  // Offset of Resource Table 
		WORD  wResOffset;       // Offset of resident name table 
		WORD  wModOffset;       // Offset of Module Reference Table 
		WORD  wImportOffset;    // Offset of Imported Names Table 
		long  lNonResOffset;    // Offset of Non-resident Names Table
								// THIS FIELD IS FROM THE BEGINNING OF THE FILE
								// NOT THE BEGINNING OF THE NEW EXE HEADER
		WORD  wMoveableEntry;   // Count of movable entries in entry table
		WORD  wAlign;           // Segment alignment shift count
		WORD  wResourceSegs;    // Count of resource segments
		BYTE  bExeType;         // Operating System flags  
		BYTE  bAdditionalFlags; // Additional exe flags 
		WORD  wFastOffset;      // offset to FastLoad area 
		WORD  wFastSize;        // length of FastLoad area 
		WORD  wReserved;
		WORD  wExpVersion;      // Expected Windows version number 
	} NEWEXE, *PNEWEXE;

	void FormatMessage(CString& strOut,DWORD dwError) {
		LPSTR lpMsgBuf = NULL;
		::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL 
		);
		if(lpMsgBuf) {
			strOut.Format("Error no. %d:\n\n%s",dwError,lpMsgBuf);
			LocalFree(lpMsgBuf);
		} else
			strOut.Format("Error no. %d",dwError);
	}

	BOOL ReadExeDesc(LPCTSTR lpszFile, LPSTR lpszDesc) {   
		OFSTRUCT			of;
		int					fFile;
		OLDEXE				OldExe; 
		NEWEXE				NewExe;
		long				lNonResTable;
		WORD				wNonResSize;
		FONTDIRENTRY FAR*	fdEntry;
		HRSRC				hResource;
		HINSTANCE			hModule;

		if(!(fFile = OpenFile(lpszFile, &of, OF_READ )))
			return FALSE;

		_lread(fFile, (LPSTR)&(OldExe), sizeof(OLDEXE));    
		_llseek(fFile, OldExe.lNewExeOffset, 0 );
		_lread(fFile, (LPSTR)&(NewExe), sizeof(NEWEXE));    
		lNonResTable = NewExe.lNonResOffset;
		_llseek( fFile, lNonResTable, 0 );
		wNonResSize = 0;
		// Read string size
		_lread(fFile, (LPSTR)&wNonResSize, 1);
		_lread(fFile, (LPSTR)lpszDesc, wNonResSize);
		_lclose( fFile );        
		lpszDesc[wNonResSize] = '\0';
		if(lpszDesc[0] == '\0')
			return FALSE; 
		if(!(hModule = LoadLibrary(lpszFile)))
			return FALSE;

		/* Read Font Directory */
		hResource = FindResource(hModule, "fontdir", RT_FONTDIR);
		if(!hResource)
			return FALSE;

		hResource = (HRSRC)LoadResource(hModule, hResource);
		if(fdEntry = (FONTDIRENTRY FAR *)LockResource(hResource)) {  
			LPSTR lpstr = (LPSTR)fdEntry->dfCharTable;
			LPSTR FaceName;
			while (*lpstr == '\0')
			lpstr++;
			FaceName = lpstr;
			//     l_tcscpy(lf->lfFaceName,FaceName);

			UnlockResource(hResource);
			FreeResource(hResource);     
			FreeLibrary(hModule);
			return TRUE;
		} else
			return FALSE;  
	}
#endif

	bool GetShellFolderPath(int nFolder,CString& strOut) {
		LPMALLOC		pMalloc;
		LPITEMIDLIST	pidl;
		bool			bResult = false;

		if(FAILED(SHGetMalloc(&pMalloc))) pMalloc = NULL;
		if(SUCCEEDED(SHGetSpecialFolderLocation(0,nFolder,&pidl))) {
			strOut.Empty();
			if(SHGetPathFromIDList(pidl,strOut.GetBuffer(MAX_PATH+1)))
				bResult = true;
			if(pMalloc)
				pMalloc->Free(pidl);
		}
		strOut.ReleaseBuffer();
		return bResult;
	}

	void CDlgSelectFonts::EnumTTF() {
		WIN32_FIND_DATA wfd;
		CString strFonts, strWild;

		if(!GetShellFolderPath(CSIDL_FONTS,strFonts)) return;
		CMyUtils::EndWith(strFonts,'\\');
		strWild.Format("%s*.ttf",strFonts);

		HANDLE hFind = FindFirstFile(strWild,&wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			do {
				CString strFilePath(strFonts + wfd.cFileName);

				CString strFaceName = ReadLongName(strFilePath);
				if(strFaceName.IsEmpty()) continue;

				CData* pData = new CData;
				_tcscpy(pData->m_szFaceName,strFaceName);
				_tcscpy(pData->m_szFilePath,strFilePath);
				pData->m_bTrueType = true;
				
				strFaceName += " (TrueType)";
				int nItem = m_wndFontList.InsertItem(0,strFaceName);
				m_wndFontList.SetItemText(nItem,1,wfd.cFileName);
				m_wndFontList.SetItemData(nItem,(DWORD)pData);
			} while(FindNextFile(hFind,&wfd));
			FindClose(hFind);
		}
	}

#ifdef TODO
	void EnumFON() {
		CFileFind find;
		CString strFonts, strWild;

		if(!GetShellFolderPath(CSIDL_FONTS,strFonts)) return;
		if(strFonts[strFonts.GetLength()-1]!='\\') strFonts += '\\';
		strWild.Format("%s*.fon",strFonts);

		BOOL bOk = find.FindFile(strWild);
		while(bOk) {
			bOk = find.FindNextFile();

			CString strFaceName;
			if(!ReadExeDesc(find.GetFilePath(),strFaceName.GetBuffer(256))) continue;
			if(strFaceName.IsEmpty()) continue;

			CData* pData = new CData;
			_tcscpy(pData->m_szFaceName,strFaceName);
			_tcscpy(pData->m_szFilePath,find.GetFilePath());
			pData->m_bTrueType = false;

			int nItem = m_wndFontList.InsertItem(0,strFaceName);
			m_wndFontList.SetItemText(nItem,1,find.GetFileName());
			m_wndFontList.SetItemData(nItem,(DWORD)pData);
		}
	}
#endif

protected:
	typedef struct {
		CHAR	m_szFaceName[MAX_PATH];
		CHAR	m_szFilePath[MAX_PATH];
		bool	m_bTrueType;
	} CData;
	CMyDoc*		m_pDoc;
	void EnumFON();
};
