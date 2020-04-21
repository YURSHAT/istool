#include "isxbb.iss"

[Setup]
AppName=Demo
AppVerName=Demo
DisableProgramGroupPage=true
Uninstallable=false
DirExistsWarning=no
CreateAppDir=false
SourceDir=.
WindowVisible=true
OutputDir=.

[Files]
Source: *.jpg; DestDir: {tmp}; CopyMode: dontcopy
Source: *.gif; DestDir: {tmp}; CopyMode: dontcopy
; A bunch of stupid dummy files that should take some time
Source: {sys}\*.dll; DestDir: {tmp}; Flags: external

[_ISTool]
EnableISX=true

[Code]
procedure CurStepChanged(CurStep: Integer);
begin
  if CurStep=csStart then begin
    ExtractTemporaryFile('ksu.jpg');
    ExtractTemporaryFile('is.jpg');
    ExtractTemporaryFile('inno_banner_468x60.gif');
    ExtractTemporaryFile('isx_banner_468x60.gif');

    isxbb_AddImage(ExpandConstant('{tmp}')+'\ksu.jpg',TOPRIGHT or TIMER);
    isxbb_AddImage(ExpandConstant('{tmp}')+'\is.jpg',TOPRIGHT or TIMER);

	isxbb_AddImage(ExpandConstant('{tmp}')+'\inno_banner_468x60.gif',BOTTOM or TIMER);
	isxbb_AddImage(ExpandConstant('{tmp}')+'\isx_banner_468x60.gif',BOTTOM or TIMER);

	isxbb_AddImage(ExpandConstant('{tmp}')+'\isx_banner_468x60.gif',TOP or TIMER);
	isxbb_AddImage(ExpandConstant('{tmp}')+'\inno_banner_468x60.gif',TOP or TIMER);

    isxbb_AddImage(ExpandConstant('{tmp}')+'\ksu.jpg',BOTTOMLEFT);
    isxbb_AddImage(ExpandConstant('{tmp}')+'\is.jpg',BOTTOMRIGHT);

    isxbb_Init(StrToInt(ExpandConstant('{hwnd}')));

    isxbb_StartTimer(2,BOTTOM)
    isxbb_StartTimer(2,TOP)
  end
  else if CurStep=csCopy then
    // TOPRIGHT is only shown during file copying
    isxbb_StartTimer(10,TOPRIGHT)
  else if CurStep<>csWizard then
    isxbb_KillTimer(TOPRIGHT);
end;

