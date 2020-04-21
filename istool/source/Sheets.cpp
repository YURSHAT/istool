#include "stdafx.h"
#include "ISTool.h"
#include "Sheets.h"
#include "Translate.h"
#include "mywtl.h"
#include "PageCommon.h"
#include "PageCommonComponents.h"
#include "PageCommonTasks.h"
#include "PageCommonLanguages.h"
#include "PageFiles.h"
#include "PageDirs.h"
#include "PageType.h"
#include "PageTask.h"
#include "PageRun.h"
#include "PageRegistry.h"
#include "PageMessage.h"
#include "PageCustomMessage.h"
#include "PageLanguages.h"
#include "PageIni.h"
#include "PageIcons.h"
#include "PageDownload.h"
#include "PageDelete.h"
#include "PageComponent.h"
#include "PageCompileStep.h"

#define USE_TABS

bool CSheets::ShowSheet(HWND hWnd,CScriptList& list,bool bNewItem) {
	DWORD dwSheets = 0;
	for(long i=0;i<list.GetSize();i++) {
		switch(list[i]->GetSection()) {
			case CInnoScript::SEC_DIRS:
				dwSheets |= SHEET_DIRS|SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_FILES:
				dwSheets |= SHEET_FILES|SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_ICONS:
				dwSheets |= SHEET_ICONS | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_INI:
				dwSheets |= SHEET_INI | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_INSTALLDELETE:
				dwSheets |= SHEET_DELETE | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_MESSAGES:
				dwSheets |= SHEET_MESSAGES;
				break;
			case CInnoScript::SEC_REGISTRY:
				dwSheets |= SHEET_REGISTRY | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_RUN:
				dwSheets |= SHEET_RUN | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_UNINSTALLDELETE:
				dwSheets |= SHEET_DELETE | SHEET_COMMON | PAGE_HASCHECK | PAGE_ISUNINSTALL | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_UNINSTALLRUN:
				dwSheets |= SHEET_RUN | SHEET_COMMON | PAGE_HASCHECK | PAGE_ISUNINSTALL | PAGE_HASCOMPONENTS | PAGE_HASTASKS | PAGE_HASLANGUAGES | PAGE_HASBEFOREINSTALL | PAGE_HASAFTERINSTALL;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_CUSTOMMESSAGES:
				dwSheets |= SHEET_CUSTOMMESSAGES;
				break;
			case CInnoScript::SEC_TYPES:
				dwSheets |= SHEET_TYPES | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASLANGUAGES;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_COMPONENTS:
				dwSheets |= SHEET_COMPONENT | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASLANGUAGES;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_TASKS:
				dwSheets |= SHEET_TASKS | SHEET_COMMON | PAGE_HASCHECK | PAGE_HASCOMPONENTS | PAGE_HASLANGUAGES;
#ifdef USE_TABS
				dwSheets |= SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
#endif
				break;
			case CInnoScript::SEC_LANGUAGES:
				dwSheets |= SHEET_LANGUAGES;
				break;
			case CInnoScript::PRJ_PRECOMPILESTEPS:
			case CInnoScript::PRJ_POSTCOMPILESTEPS:
				dwSheets |= SHEET_COMPILESTEP;
				break;
			case CInnoScript::PRJ_DOWNLOAD:
				dwSheets |= SHEET_DOWNLOAD | SHEET_COMMON_TASKS | SHEET_COMMON_COMPONENTS | SHEET_COMMON_LANGUAGES;
				break;
			default:
				ATLASSERT(false);
		}
	}

	return ShowSheet(hWnd,list,dwSheets,bNewItem);
}

bool CSheets::ShowSheet(HWND hWnd,CScriptList& list,DWORD dwSheets,bool bNewItem) {
	CString					strTitle(bNewItem ? _L("New Item") : _L("Properties"));
	CMyPropertySheet		sheet((LPCTSTR)strTitle);

	CPageFiles				dlgFiles(list,AfxGetDocument(),bNewItem);
	CPageDirs				dlgDirs(list,AfxGetDocument(),bNewItem);
	CPageType				dlgTypes(list,AfxGetDocument(),bNewItem);
	CPageTask				dlgTasks(AfxGetDocument(),list,bNewItem,AfxGetDocument()->GetScript());
	CPageRun				dlgRun(list,AfxGetDocument(),bNewItem,dwSheets & PAGE_ISUNINSTALL ? true : false);
	CPageRegistry			dlgRegistry(list,AfxGetDocument(),bNewItem);
	CPageMessage			dlgMessage(list,AfxGetDocument(),bNewItem);
	CPageLanguages			dlgLanguages(list,AfxGetDocument(),bNewItem);
	CPageIni				dlgIni(list,AfxGetDocument(),bNewItem);
	CPageIcons				dlgIcons(list,AfxGetDocument(),bNewItem);
	CPageDownload			dlgDownload(list,AfxGetDocument(),bNewItem);
	CPageDelete				dlgDelete(list,AfxGetDocument(),bNewItem);
	CPageComponent			dlgComponent(AfxGetDocument(),list,bNewItem,AfxGetDocument()->GetScript());
	CPageCompileStep		dlgCompileStep(list,AfxGetDocument(),bNewItem);
	CPageCustomMessage		dlgCustomMessage(list,AfxGetDocument(),bNewItem);

	DWORD dwFlags = 0;
	if(dwSheets & PAGE_HASCHECK) dwFlags |= CPageCommon::HASCHECK;
	if(dwSheets & PAGE_HASCOMPONENTS) dwFlags |= CPageCommon::HASCOMPONENTS;
	if(dwSheets & PAGE_HASTASKS) dwFlags |= CPageCommon::HASTASKS;
	if(dwSheets & PAGE_HASLANGUAGES) dwFlags |= CPageCommon::HASLANGUAGES;
	if(dwSheets & PAGE_HASBEFOREINSTALL) dwFlags |= CPageCommon::HASBEFOREINSTALL;
	if(dwSheets & PAGE_HASAFTERINSTALL) dwFlags |= CPageCommon::HASAFTERINSTALL;

	CPageCommon				dlgCommon(dwFlags,list,AfxGetDocument(),bNewItem);
	CPageCommonComponents	dlgCommonComponents(list,AfxGetDocument(),bNewItem);
	CPageCommonTasks		dlgCommonTasks(list,AfxGetDocument(),bNewItem);
	CPageCommonLanguages	dlgCommonLanguages(list,AfxGetDocument(),bNewItem);

	if(dwSheets & SHEET_FILES)				sheet.AddPage(dlgFiles);
	if(dwSheets & SHEET_DIRS)				sheet.AddPage(dlgDirs);
	if(dwSheets & SHEET_TYPES)				sheet.AddPage(dlgTypes);
	if(dwSheets & SHEET_TASKS)				sheet.AddPage(dlgTasks);
	if(dwSheets & SHEET_RUN)				sheet.AddPage(dlgRun);
	if(dwSheets & SHEET_REGISTRY)			sheet.AddPage(dlgRegistry);
	if(dwSheets & SHEET_MESSAGES)			sheet.AddPage(dlgMessage);
	if(dwSheets & SHEET_LANGUAGES)			sheet.AddPage(dlgLanguages);
	if(dwSheets & SHEET_INI)				sheet.AddPage(dlgIni);
	if(dwSheets & SHEET_ICONS)				sheet.AddPage(dlgIcons);
	if(dwSheets & SHEET_DOWNLOAD)			sheet.AddPage(dlgDownload);
	if(dwSheets & SHEET_DELETE)				sheet.AddPage(dlgDelete);
	if(dwSheets & SHEET_COMPONENT)			sheet.AddPage(dlgComponent);
	if(dwSheets & SHEET_COMPILESTEP)		sheet.AddPage(dlgCompileStep);
	if(dwSheets & SHEET_CUSTOMMESSAGES)		sheet.AddPage(dlgCustomMessage);

	if(dwSheets & SHEET_COMMON)				sheet.AddPage(dlgCommon);
	if(dwSheets & SHEET_COMMON_COMPONENTS)	sheet.AddPage(dlgCommonComponents);
	if(dwSheets & SHEET_COMMON_TASKS)		sheet.AddPage(dlgCommonTasks);
	if(dwSheets & SHEET_COMMON_LANGUAGES)	sheet.AddPage(dlgCommonLanguages);

	return sheet.DoModal(hWnd)==IDOK;
}
