// isslexer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <SciLexer.h>
#include <Platform.h>
#include <PropSet.h>
#include <Accessor.h>
#include <WindowAccessor.h>
#include <StyleContext.h>

#define mdebug(x)	::MessageBox(GetActiveWindow(),x,"DEBUG",MB_OK|MB_ICONINFORMATION)

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

int __stdcall GetLexerCount() {
	return 1;
}

void __stdcall GetLexerName(unsigned int Index, char *name, int buflength) {
	_tcsncpy(name,_T("iss"),buflength);
}

enum {
	SCE_ISS_DEFAULT = 0,
	SCE_ISS_COMMENT,
	SCE_ISS_DIRECTIVE,
	SCE_ISS_SECTION,
	SCE_ISS_CONSTANT,
	SCE_ISS_FLAG,
	SCE_ISS_ERROR,
	SCE_ISS_STRING,
	SCE_ISS_ARGUMENT,
	SCE_ISS_PREPROC
};

enum {
	SEC_MASK			= 0x01F,
	SEC_SETUP			= 0x001,
	SEC_TYPES			= 0x002,
	SEC_COMPONENTS		= 0x003,
	SEC_TASKS			= 0x004,
	SEC_DIRS			= 0x005,
	SEC_FILES			= 0x006,
	SEC_ICONS			= 0x007,
	SEC_INI				= 0x008,
	SEC_INSTALLDELETE	= 0x009,
	SEC_MESSAGES		= 0x00A,
	SEC_CUSTOMMESSAGES	= 0x00B,
	SEC_LANGOPTIONS		= 0x00C,
	SEC_REGISTRY		= 0x00D,
	SEC_RUN				= 0x00E,
	SEC_UNINSTALLDELETE	= 0x00F,
	SEC_UNINSTALLRUN	= 0x010,
	SEC_CODE			= 0x011,
	SEC_LANGUAGES		= 0x012,
	SEC_ISTOOL			= 0x013,
	SEC_ISTOOLPRE		= 0x014,
	SEC_ISTOOLPOST		= 0x015,
	SEC_ISTOOLDL		= 0x016,
	ISS_CONSTANT		= 0x020,
	ISS_ARGUMENT		= 0x040,
	ISS_SECTION			= 0x080,
	ISS_STRING			= 0x100,
	ISS_COMMENT			= 0x200
};

static long IsValidSection(LPCTSTR psz) {
	static const LPCTSTR sections[] = {
		"setup", "types", "components", "tasks", "dirs", "files", "icons", "ini",
		"installdelete", "messages", "custommessages", "langoptions", "registry", "run", "uninstalldelete",
		"uninstallrun", "code", "languages",
		"_istool", "_istoolprecompile", "_istoolpostcompile", "_istooldownload"
	};
	for(long i=0;i<21;i++) {
		if(!stricmp(psz,sections[i]) || (psz[0]=='/'))
			return i+1;
	}
	return 0;
}

static bool FindDirective(const LPCTSTR a[],LPCTSTR psz) {
	for(long i=0;a[i];i++) {
		if(!_tcsicmp(a[i],psz))
			return true;
	}
	return false;
}

static bool IsValidDirective(LPCTSTR psz,long sec) {
	static const LPCTSTR _setup[] = {
		"Compression", "DiskClusterSize", "DiskSliceSize", "DiskSpanning", "MergeDuplicateFiles", 
		"InternalCompressLevel", "OutputBaseFilename", "OutputDir", "ReserveBytes", "SlicesPerDisk", 
		"SourceDir", "UseSetupLdr", "AdminPrivilegesRequired", "AllowNoIcons", "AllowRootDirectory", 
		"AllowUNCPath", "AlwaysRestart", "AlwaysShowComponentsList", "AlwaysShowDirOnReadyPage", 
		"AlwaysShowGroupOnReadyPage", "AlwaysUsePersonalGroup", "AppName", "AppId", "AppMutex", 
		"AppPublisher", "AppPublisherURL", "AppSupportURL", "AppUpdatesURL", "AppVersion", "AppVerName", 
		"ChangesAssociations", "CreateAppDir", "CreateUninstallRegKey", "DefaultDirName", 
		"DefaultGroupName", "DefaultUserInfoName", "DefaultUserInfoOrg", "DefaultUserInfoSerial", 
		"DirExistsWarning", "DisableDirPage", "DisableFinishedPage", 
		"DisableProgramGroupPage", "DisableReadyMemo", "DisableReadyPage", "DisableStartupPrompt", 
		"EnableDirDoesntExistWarning", "ExtraDiskSpaceRequired", "InfoAfterFile", "InfoBeforeFile", 
		"LicenseFile", "MinVersion", "OnlyBelowVersion", "Password", "PrivilegesRequired", 
		"RestartIfNeededByRun", "ShowLanguageDialog", "Uninstallable", "UninstallDisplayIcon", 
		"UninstallDisplayName", "UninstallFilesDir", "UninstallLogMode", "UninstallRestartComputer", 
		"UpdateUninstallLogAppName", "UsePreviousAppDir", "UsePreviousGroup", "UsePreviousSetupType", 
		"UsePreviousTasks", "UsePreviousUserInfo", "UserInfoPage", "AppCopyright", "BackColor", 
		"BackColor2", "BackColorDirection", "BackSolid", "FlatComponentsList", "ShowComponentSizes", 
		"ShowTasksTreeLines", "UninstallStyle", "WindowShowCaption", "WindowStartMaximized", 
		"WindowResizable", "WindowVisible", "WizardImageBackColor", "WizardImageFile", 
		"WizardSmallImageBackColor", "WizardSmallImageFile", "SolidCompression", 
		"LanguageDetectionMethod", "AllowCancelDuringInstall", "TimeStampsInUTC", 
		"VersionInfoVersion", "VersionInfoCompany", "VersionInfoDescription", "VersionInfoTextVersion",
		"SetupIconFile", "UninstallIconFile", "TimeStampRounding", "WizardImageStretch", 
		"AppendDefaultDirName", "AppendDefaultGroupName", "TouchFileDate", "TouchFileTime",
		"Encryption", "AppModifyPath", "AppComments", "AppContact", "AppReadmeFile", 
		"OutputManifestFile", "ChangesEnvironment", "VersionInfoCopyright", "TouchDate", "TouchTime",
		"TerminalServicesAware", "ShowUndisplayableLanguages", "AppSupportPhone", "SetupLogging", 
		"CompressionThreads", "SignedUninstaller", "SignedUninstallerDir",
		"VersionInfoProductName", "VersionInfoProductVersion", "SignTool",
		NULL
	};
	static const LPCTSTR _files[] = { "source", "destdir", "destname", "attribs", "fontinstall", "flags", "permissions", "excludes", "externalsize", "strongassemblyname", NULL };
	static const LPCTSTR _run[] = { "filename", "description", "parameters", "workingdir", "statusmsg", "runonceid", "flags", "verb", NULL };
	static const LPCTSTR _dirs[] = { "name", "attribs", "flags", "permissions", NULL };
	static const LPCTSTR _messages[] = { "beveledlabel", NULL };
	static const LPCTSTR _languages[] = { "name", "messagesfile", "licensefile","infobeforefile","infoafterfile",NULL };
	static const LPCTSTR _delete[] = { "type", "name", NULL };
	static const LPCTSTR _istool[] = { "useabsolutepaths", "logfile", "logfileappend", "outputexefilename", NULL };
	static const LPCTSTR _istoolpre[] = { "name", "parameters", NULL };
	static const LPCTSTR _istooldl[] = { "source","destdir","destname",NULL };
	static const LPCTSTR _types[] = { "name", "description", "flags", NULL };
	static const LPCTSTR _components[] = { "name","description","types","extradiskspacerequired","flags",NULL };
	static const LPCTSTR _tasks[] = { "name","description","groupdescription","components","flags",NULL};
	static const LPCTSTR _icons[] = { "name","filename","parameters","workingdir","hotkey","comment","iconfilename","iconindex","flags",NULL};
	static const LPCTSTR _ini[] = { "filename","section","key","string","flags",NULL };
	static const LPCTSTR _registry[] = { "root","subkey","valuetype","valuename","valuedata","flags","permissions", NULL };
	static const LPCTSTR _common[] = { "languages", "tasks", "components", "minversion", "onlybelowversion", "check", "beforeinstall", "afterinstall", NULL };

	if(!sec)
		return false;
	
	CString tmp(psz);
	tmp.Trim();

	if(FindDirective(_common,tmp))
		return true;

	switch(sec) {
		case SEC_SETUP:
			return FindDirective(_setup,tmp);
		case SEC_FILES:
			return FindDirective(_files,tmp);
		case SEC_RUN:
		case SEC_UNINSTALLRUN:
			return FindDirective(_run,tmp);
		case SEC_DIRS:
			return FindDirective(_dirs,tmp);
		case SEC_MESSAGES:
			return FindDirective(_messages,tmp);
		case SEC_LANGUAGES:
			return FindDirective(_languages,tmp);
		case SEC_INSTALLDELETE:
		case SEC_UNINSTALLDELETE:
			return FindDirective(_delete,tmp);
		case SEC_ISTOOL:
			return FindDirective(_istool,tmp);
		case SEC_ISTOOLPRE:
		case SEC_ISTOOLPOST:
			return FindDirective(_istoolpre,tmp);
		case SEC_ISTOOLDL:
			return FindDirective(_istooldl,tmp);
		case SEC_TYPES:
			return FindDirective(_types,tmp);
		case SEC_COMPONENTS:
			return FindDirective(_components,tmp);
		case SEC_TASKS:
			return FindDirective(_tasks,tmp);
		case SEC_ICONS:
			return FindDirective(_icons,tmp);
		case SEC_INI:
			return FindDirective(_ini,tmp);
		case SEC_REGISTRY:
			return FindDirective(_registry,tmp);
		default:
			return false;
	}
}

static bool IsFlag(LPCTSTR pszFlag,long nSection) {
	static const LPCTSTR _types[] = { "iscustom", NULL };
	static const LPCTSTR _components[] = { "exclusive", "fixed", "restart", "disablenouninstallwarning", "dontinheritcheck", "checkablealone", NULL };
	static const LPCTSTR _tasks[] = { "checkedonce", "exclusive", "restart", "unchecked", "dontinheritcheck", "checkablealone", NULL };
	static const LPCTSTR _dirs[] = { "deleteafterinstall", "uninsalwaysuninstall", "uninsneveruninstall", "system", "hidden", "readonly", "setntfscompression", "unsetntfscompression", NULL };
	static const LPCTSTR _files[] = { "allowunsafefiles", "comparetimestamp", "confirmoverwrite", "deleteafterinstall", "dontcopy", "external", "fontisnttruetype", "ignoreversion", "isreadme", "noregerror", "onlyifdestfileexists", "onlyifdoesntexist", "overwritereadonly", "promptifolder", "recursesubdirs", "regserver", "regtypelib", "restartreplace", "sharedfile", "skipifsourcedoesntexist", "uninsrestartdelete", "uninsneveruninstall", "hidden", "system", "readonly", "uninsremovereadonly", "sortfilesbyextension", "replacesameversion", "touch", "noencryption", "nocompression", "dontverifychecksum", "uninsnosharedfileprompt", "createallsubdirs", "32bit", "64bit", "solidbreak", "setntfscompression", "unsetntfscompression", "sortfilesbyname", "gacinstall", NULL };
	static const LPCTSTR _icons[] = { "closeonexit", "createonlyiffileexists", "dontcloseonexit", "runmaximized", "runminimized", "uninsneveruninstall", "useapppaths", "foldershortcut", NULL };
	static const LPCTSTR _ini[] = { "createkeyifdoesntexist", "uninsdeleteentry", "uninsdeletesection", "uninsdeletesectionifempty", NULL };
	static const LPCTSTR _registry[] = { "HKCR", "HKCU", "HKLM", "HKU", "HKCC", "none", "string", "expandsz", "multisz", "dword", "binary", "createvalueifdoesntexist", "deletekey", "deletevalue", "dontcreatekey", "noerror", "preservestringtype", "uninsclearvalue", "uninsdeletekey", "uninsdeletekeyifempty", "uninsdeletevalue", NULL };
	static const LPCTSTR _run[] = { "hidewizard", "nowait", "postinstall", "runmaximized", "runminimized", "shellexec", "skipifdoesntexist", "skipifnotsilent", "skipifsilent", "unchecked", "waituntilidle", "32bit", "64bit", "runascurrentuser", "runasoriginaluser", NULL };
	static const LPCTSTR _delete[] = { "files", "filesandordirs", "dirifempty", NULL };

	CString tmp(pszFlag);
	tmp.Trim();

	switch(nSection) {
		case SEC_TYPES:
			return FindDirective(_types,tmp);
		case SEC_COMPONENTS:
			return FindDirective(_components,tmp);
		case SEC_TASKS:
			return FindDirective(_tasks,tmp);
		case SEC_DIRS:
			return FindDirective(_dirs,tmp);
		case SEC_FILES:
			return FindDirective(_files,tmp);
		case SEC_ICONS:
			return FindDirective(_icons,tmp);
		case SEC_INI:
			return FindDirective(_ini,tmp);
		case SEC_REGISTRY:
			return FindDirective(_registry,tmp);
		case SEC_RUN:
		case SEC_UNINSTALLRUN:
			return FindDirective(_run,tmp);
		case SEC_UNINSTALLDELETE:
		case SEC_INSTALLDELETE:
			return FindDirective(_delete,tmp);
		default:
			return false;
	}
}

static void DoLex(unsigned int startPos, int length, int initStyle, WordList *keywordLists[], Accessor &styler) {
	StyleContext sc(startPos, length, initStyle, styler);

	long nState = 0;
	for (; sc.More(); sc.Forward()) {
		long nLine = styler.GetLine(sc.currentPos);
		if(sc.atLineStart && nLine>0)
			nState = styler.GetLineState(nLine-1);

		// Handle line continuation generically.
		if (sc.ch == '\\') {
			if (sc.Match("\\\n")) {
				sc.Forward();
				continue;
			}
			if (sc.Match("\\\r\n")) {
				sc.Forward();
				sc.Forward();
				continue;
			}
		}

		if(sc.ch=='#' && sc.state!=SCE_ISS_PREPROC && sc.state!=SCE_ISS_COMMENT) {
			sc.ChangeState(SCE_ISS_DEFAULT);
			sc.SetState(SCE_ISS_PREPROC);
		} else if(sc.state==SCE_ISS_PREPROC && !isalpha(sc.ch)) {
			sc.SetState(SCE_ISS_DEFAULT);
		}
		if(sc.state == SCE_ISS_ARGUMENT) {
			if(sc.ch==';') {
				char s[100];
				sc.GetCurrentLowered(s, sizeof(s));
				if(IsFlag(s,nState)) {
					sc.ChangeState(SCE_ISS_FLAG);
					sc.SetState(SCE_ISS_DEFAULT);
				} else
					sc.ChangeState(SCE_ISS_DEFAULT);

				sc.ForwardSetState(SCE_ISS_DIRECTIVE);
			} else if(sc.ch==' ' || sc.ch=='\n') {
				char s[100];
				sc.GetCurrentLowered(s, sizeof(s));
				if(IsFlag(s,nState))
					sc.ChangeState(SCE_ISS_FLAG);
				else
					sc.ChangeState(SCE_ISS_DEFAULT);
				sc.SetState(sc.ch=='\n' ? SCE_ISS_DEFAULT : SCE_ISS_ARGUMENT);
			} else if(sc.ch=='{') {
				sc.ChangeState(SCE_ISS_DEFAULT);
				sc.SetState(SCE_ISS_CONSTANT);
			}
		} else if(sc.state == SCE_ISS_CONSTANT) {
			if(sc.ch=='}') {
				if(sc.chNext==';') {
					sc.ForwardSetState(SCE_ISS_DEFAULT);
					sc.ForwardSetState(SCE_ISS_DIRECTIVE);
				} else
					sc.ForwardSetState(SCE_ISS_ARGUMENT);
				//sc.ForwardSetState(SCE_ISS_ARGUMENT);
			} else if(sc.ch=='\n') {
				sc.ChangeState(SCE_ISS_DEFAULT);
				sc.SetState(SCE_ISS_DEFAULT);
			}
		} else if(sc.state == SCE_ISS_DIRECTIVE) {
			if(sc.ch == '=' || sc.ch == ':') {
				char s[100];
				sc.GetCurrentLowered(s, sizeof(s));
				if(!IsValidDirective(s,nState))
					sc.ChangeState(SCE_ISS_DEFAULT);
				
				sc.SetState(SCE_ISS_ARGUMENT);
			} else if(sc.ch == '\n') {
				sc.ChangeState(SCE_ISS_DEFAULT);
			}
		} else if(sc.ch=='\n') {
			sc.SetState(SCE_ISS_DEFAULT);
		} else if(sc.state == SCE_ISS_SECTION) {
			if(sc.ch==']') {
				char s[100];
				sc.GetCurrentLowered(s, sizeof(s));
				long nSec = IsValidSection(&s[1]);
				if(!nSec)
					sc.ChangeState(SCE_ISS_DEFAULT);
				else {
					sc.ForwardSetState(SCE_ISS_DEFAULT);
					nState &= ~SEC_MASK;
					nState |= nSec;
				}
			}
		} else if(sc.state == SCE_ISS_COMMENT) {
			if(sc.ch=='\n')
				sc.SetState(SCE_ISS_DEFAULT);
		} else if(sc.state == SCE_ISS_DEFAULT) {
			if(sc.atLineStart) {
				if(sc.ch=='[') {
					sc.SetState(SCE_ISS_SECTION);
				} else if(sc.ch==';' && (nState & SEC_MASK)!=SEC_CODE) {
					sc.SetState(SCE_ISS_COMMENT);
				} else if((nState & SEC_MASK)!=SEC_CODE) {
					sc.SetState(SCE_ISS_DIRECTIVE);
				}
			}
		}
		styler.SetLineState(nLine,nState);
	}
	if(sc.state!=SCE_ISS_PREPROC) {
		if(sc.state == SCE_ISS_ARGUMENT) {
			char s[100];
			sc.GetCurrentLowered(s, sizeof(s));
			if(IsFlag(s,nState)) sc.ChangeState(SCE_ISS_FLAG);
		} else {
			sc.ChangeState(SCE_ISS_DEFAULT);
		}
	}
	sc.Complete();
}

static WordList **StringToWordLists(char *val[]) {
    int dim = 0;

    while (val[dim])
        dim++;

	WordList **wla = new WordList*[dim+1];
    
	for (int i = 0; i < dim; i++) 
	{
        wla[i] = new WordList;
        wla[i]->Set(val[i]);
    }
    
	wla[dim] = 0;
    
	return wla;
}

static void FreeWordLists(WordList *wla[])
{
	int dim = 0;
	while (wla[dim])
	{
		delete wla[dim];
		dim++;
	}

	delete [] wla;
	wla = NULL;
}

void __stdcall Lex(unsigned int lexer, unsigned int startPos, int length, int initStyle,
				   char *words[], WindowID window, char *props) 
{
	WordList **wordlists = StringToWordLists(words);

	PropSet ps;
	ps.SetMultiple(props);
	
	WindowAccessor wa(window, ps);
	
//	theLexers[lexer]->DoLex(startPos, length, initStyle, wordlists, wa);
	DoLex(startPos,length,initStyle,wordlists,wa);

	wa.Flush();

	FreeWordLists(wordlists);	
}

void __stdcall Fold(unsigned int lexer, unsigned int startPos, int length, int initStyle,
                  char *words[], WindowID window, char *props)
{
	mdebug("Fold called");
	WordList **wordlists = StringToWordLists(words);

	PropSet ps;
	ps.SetMultiple(props);
	
	WindowAccessor wa(window, ps);
	
//	theLexers[lexer]->DoFold(startPos, length, initStyle, wordlists, wa);

	wa.Flush();

	FreeWordLists(wordlists);
}

void* __stdcall GetLexerFunction(unsigned int Index) {
	mdebug("GetLexerFunction called");
	return NULL;
}
