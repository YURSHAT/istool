#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

static LRESULT SendCopyDataMessage(HWND DestWnd,HWND SourceWnd,DWORD CopyDataMsg,LPVOID Data,DWORD Size) {
  COPYDATASTRUCT CopyDataStruct;

  CopyDataStruct.dwData = CopyDataMsg;
  CopyDataStruct.cbData = Size;
  CopyDataStruct.lpData = Data;
  return SendMessage(DestWnd, WM_COPYDATA, WPARAM(SourceWnd), LPARAM(&CopyDataStruct));
}

static LRESULT SendCopyDataMessageStr(HWND DestWnd,HWND SourceWnd,DWORD CopyDataMsg,LPCTSTR Data) {
  /* Windows 95/98/Me bug workaround: Call UniqueString to ensure the string is
    in writable memory. Amazingly enough, sending a WM_COPYDATA message with a
    read-only buffer causes a fatal page fault error. */
#if 0
  if (Win32Platform = VER_PLATFORM_WIN32_WINDOWS) and
     IsBadWritePtr(Pointer(Data), Length(Data)) then
    UniqueString(Data);
#endif
	return SendCopyDataMessage(DestWnd, SourceWnd, CopyDataMsg,LPVOID(Data),strlen(Data));
}

LRESULT CMainFrame::OnDebuggerCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	CString tmp;
	COPYDATASTRUCT* pcds = reinterpret_cast<COPYDATASTRUCT*>(lParam);

	switch(pcds->dwData) {
		case CD_Debugger_LogMessage:
			_tcsncpy(tmp.GetBufferSetLength(pcds->cbData),(char*)pcds->lpData,pcds->cbData);
			tmp = LPCTSTR(pcds->lpData);
			//SetString(S, PChar(Message.CopyDataStruct.lpData),
			//Message.CopyDataStruct.cbData);
			//DebugLogMessage(S);
			return 1;
		default:
			_tcsncpy(tmp.GetBufferSetLength(pcds->cbData),(char*)pcds->lpData,pcds->cbData);
			return 1;
	}


	return 0;
}

LRESULT CMainFrame::OnDebuggerHello(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	HWND hDebugClientWnd = HWND(wParam);
	BOOL bWantCodeText = BOOL(lParam);
	HANDLE hProcess = 0;

	DWORD dwPID = 0;
	::GetWindowThreadProcessId(hDebugClientWnd,&dwPID);
	if(dwPID!=0)
		hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,dwPID);

	if(bWantCodeText)
		SendCopyDataMessageStr(hDebugClientWnd, m_hWnd, CD_DebugClient_CompiledCodeText, "FCompiledCodeText");
	SendCopyDataMessageStr(hDebugClientWnd, m_hWnd, CD_DebugClient_CompiledCodeDebugInfo, "FCompiledCodeDebugInfo");
	return 0;
}
