// CompInt.h : Compiler Interface

#ifndef __CompInt_h
#define __CompInt_h

#pragma once

/*
  Inno Setup
  Copyright (C) 1998-2000 Jordan Russell
  For conditions of distribution and use, see LICENSE.TXT.

  Compiler interface
*/

  /* Constants passed in Code parameter of callback function */
enum {
  iscbReadScript = 1,      /* Sent when compiler needs the next script line */
  iscbNotifyStatus = 2,    /* Sent to notify the application of compiler status */
  iscbNotifyIdle = 3,      /* Sent at various intervals during the compilation */
  iscbNotifySuccess = 4,   /* Sent when compilation succeeds */
  iscbNotifyError = 5,      /* Sent when compilation fails or is aborted by the
                             application */
  iscbReceiveTranslation	= 65535	// ISPP
};

  /* Return values for callback function */
enum {
  iscrSuccess = 0,         /* Return this for compiler to continue */
  iscrRequestAbort = 1     /* Return this to abort compilation */
};

  /* Return values for ISDllCompileScript */
enum {
  isceNoError = 0,         /* Successful */
  isceInvalidParam = 1,    /* Bad parameters passed to function */
  isceCompileFailure = 2   /* There was an error compiling or it was aborted
                             by the application */
};

  /* TCompilerCallbackData is a record passed to the callback function. The
    fields which you may access vary depending on what Code was passed to the
    callback function. */
typedef struct {
	union {
		struct {
			bool Reset;           /* [in] True if it needs the application to return
									to the beginning of the script. In other words,
									LineRead must be the first line of the script. */
			LPCSTR LineRead;      /* [out] Application returns pointer to line it
									reads, or a NULL pointer if the end of file is
									reached. Application is responsible for
									allocating a buffer to hold the line; LineRead
									is initially NULL when the callback function
									is called. */
		} ReadScript;

		struct {
			LPCSTR StatusMsg;     /* [in] Contents of status message. */
		} NotifyStatus;

		struct {
			LPCSTR OutputExeFilename;  /* [in] The name of the resulting setup.exe */
		} NotifySuccess;

		struct {
			LPCSTR ErrorMsg;      /* [in] The error message, or NULL if compilation
									was aborted by the application. */
			LPCSTR ErrorFilename; /* [in] Filename in which the error occured. This
									is NULL if the file is the main script. */
			LONG ErrorLine;       /* [in] The line number the error occured on.
									Zero if the error doesn't apply to any
									particular line. */
		} NotifyError;

		struct {
			DWORD	CompressProgress;		// Amount compressed so far
			DWORD	CompressProgressMax;	// Maximum value of CompressProgress
		} NotifyIdle;
	};
} TCompilerCallbackDataA;

typedef struct {
	union {
		struct {
			bool Reset;           /* [in] True if it needs the application to return
									to the beginning of the script. In other words,
									LineRead must be the first line of the script. */
			LPWSTR LineRead;      /* [out] Application returns pointer to line it
									reads, or a NULL pointer if the end of file is
									reached. Application is responsible for
									allocating a buffer to hold the line; LineRead
									is initially NULL when the callback function
									is called. */
		} ReadScript;

		struct {
			LPWSTR StatusMsg;     /* [in] Contents of status message. */
		} NotifyStatus;

		struct {
			LPWSTR OutputExeFilename;  /* [in] The name of the resulting setup.exe */
		} NotifySuccess;

		struct {
			LPWSTR ErrorMsg;      /* [in] The error message, or NULL if compilation
									was aborted by the application. */
			LPWSTR ErrorFilename; /* [in] Filename in which the error occured. This
									is NULL if the file is the main script. */
			LONG ErrorLine;       /* [in] The line number the error occured on.
									Zero if the error doesn't apply to any
									particular line. */
		} NotifyError;

		struct {
			DWORD	CompressProgress;		// Amount compressed so far
			DWORD	CompressProgressMax;	// Maximum value of CompressProgress
		} NotifyIdle;
	};
} TCompilerCallbackDataW;

typedef LONG(__stdcall* TCompilerCallbackProcA)(LONG Code,TCompilerCallbackDataA* Data, DWORD AppData);
typedef LONG(__stdcall* TCompilerCallbackProcW)(LONG Code,TCompilerCallbackDataW* Data, DWORD AppData);

typedef struct {
    DWORD Size;           /* [in] Set to SizeOf(TCompileScriptParams). */
    LPCSTR CompilerPath;  /* [in] The "compiler:" directory. This is the
                            directory which contains the *.e32 files. */
    LPCSTR ScriptPath;    /* [in] Set to the directory containing the script file.
                            This path is used as the default source directory. */
    TCompilerCallbackProcA CallbackProc;
                          /* [in] The callback procedure which the compiler calls
                            to read the script and for status notification. */
    DWORD AppData;        /* [in] Application-defined. AppData is passed to the
                            callback function. */
} TCompileScriptParamsA;

typedef struct {
    DWORD Size;           /* [in] Set to SizeOf(TCompileScriptParams). */
    LPWSTR CompilerPath;  /* [in] The "compiler:" directory. This is the
                            directory which contains the *.e32 files. */
    LPWSTR ScriptPath;    /* [in] Set to the directory containing the script file.
                            This path is used as the default source directory. */
    TCompilerCallbackProcW CallbackProc;
                          /* [in] The callback procedure which the compiler calls
                            to read the script and for status notification. */
    DWORD AppData;        /* [in] Application-defined. AppData is passed to the
                            callback function. */
} TCompileScriptParamsW;

typedef struct {
    LPCSTR Title;          /* Name of compiler engine - 'Inno Setup' */
    LPCSTR Version;        /* Version number text */
    DWORD BinVersion;      /* Version number as an integer */
} TCompilerVersionInfo, *PCompilerVersionInfo;


const LPCSTR ISCmplrDLL = "ISCmplr.dll";

/* The ISDllCompileScript function begins compilation of a script. See the above
  description of the TCompileScriptParams record. Return value is one of the
  isce* constants.
  Note: The compiler does change the current directory during compilation,
  but restores it once the function returns. */
typedef LONG(__stdcall* ISDllCompileScriptProcA)(TCompileScriptParamsA* Params);
typedef LONG(__stdcall* ISDllCompileScriptProcW)(TCompileScriptParamsW* Params);

/* The ISDllGetVersion returns a pointer to a TCompilerVersionInfo record which
  contains information about the compiler version. */
typedef PCompilerVersionInfo(__stdcall* ISDllGetVersionProc)();

// ISPP
#pragma pack(push,1)
typedef struct {
	DWORD	ParserOptions;			// offset=00
	DWORD	Options;				// 04
	BYTE	VerboseLevel;			// 08
	CHAR	InlineStart[8];			// 09
	CHAR	InlineEnd[8];			// 17
	CHAR	SpanSymbol;				// 25
} TIsppOptionsA;

#if 1
typedef TIsppOptionsA TIsppOptionsW;
#else
typedef struct {
	DWORD	ParserOptions;			// offset=00
	DWORD	Options;				// 04
	BYTE	VerboseLevel;			// 08
	WCHAR	InlineStart[8];			// 09
	WCHAR	InlineEnd[8];			// 17
	WCHAR	SpanSymbol;				// 25
} TIsppOptionsW;
#endif
#pragma pack(pop)

#define OPTION_A	(1<<0)
#define OPTION_B	(1<<1)
#define OPTION_C	(1<<2)
#define OPTION_D	(1<<3)
#define OPTION_E	(1<<4)
#define OPTION_F	(1<<5)
#define OPTION_G	(1<<6)
#define OPTION_H	(1<<7)
#define OPTION_I	(1<<8)
#define OPTION_J	(1<<9)
#define OPTION_K	(1<<10)
#define OPTION_L	(1<<11)
#define OPTION_M	(1<<12)
#define OPTION_N	(1<<13)
#define OPTION_O	(1<<14)
#define OPTION_P	(1<<15)
#define OPTION_Q	(1<<16)
#define OPTION_R	(1<<17)
#define OPTION_S	(1<<18)
#define OPTION_T	(1<<19)
#define OPTION_U	(1<<20)
#define OPTION_V	(1<<21)
#define OPTION_W	(1<<22)
#define OPTION_X	(1<<23)
#define OPTION_Y	(1<<24)
#define OPTION_Z	(1<<25)

typedef LONG(__stdcall* ISDllCompileScriptISPPProcA)(TCompileScriptParamsA* Params,TIsppOptionsA* ISPPOptions,LPCTSTR IncludePath,LPCTSTR Definitions);
typedef LONG(__stdcall* ISDllCompileScriptISPPProcW)(TCompileScriptParamsW* Params,TIsppOptionsW* ISPPOptions,LPCTSTR IncludePath,LPCTSTR Definitions);

#endif
