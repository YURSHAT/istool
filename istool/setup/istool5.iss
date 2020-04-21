; Script created by ISTool
; http://www.bhenden.org/istool/

#pragma option -v+
#pragma verboselevel 9

#define APPVER	GetFileVersion("..\source\release\ISTool.exe")

[Setup]
AppName=ISTool
AppVerName=ISTool {#APPVER}
DefaultDirName={pf}\ISTool
ChangesAssociations=true
AppCopyright=Copyright © 1999 - 2009 Bjørnar Henden
DefaultGroupName=ISTool
LicenseFile=License.rtf
;MinVersion=4.0,4.0.1381sp6
OutputBaseFilename=istool-5.3.0.1
OutputDir=.
AppID={{A9E12684-DD23-4D11-ACAF-6041954BCA00}
AppPublisher=Bjørnar Henden
AppPublisherURL=http://www.istool.org/
AppSupportURL=http://www.istool.org/
AppUpdatesURL=http://www.istool.org/
AppVersion={#APPVER}
InfoAfterFile=History.rtf
AlwaysShowComponentsList=false
AppMutex=ISTool
Compression=lzma/ultra
AllowUNCPath=true
ShowTasksTreeLines=false
WizardSmallImageFile=small.bmp
WizardImageFile=large.bmp
ShowLanguageDialog=yes
TimeStampsInUTC=true
SolidCompression=true
UninstallIconFile=
SetupIconFile=
VersionInfoVersion={#APPVER}
VersionInfoCompany=Bjørnar Henden
VersionInfoDescription=http://www.istool.org/
VersionInfoTextVersion={#APPVER}
InternalCompressLevel=ultra

[Files]
Source: ..\source\release\ISTool.exe; DestDir: {app}; Flags: overwritereadonly ignoreversion comparetimestamp uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\help\ISTool.chm; DestDir: {app}; Flags: overwritereadonly ignoreversion comparetimestamp uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\setup\iwz2issl.dll; DestDir: {app}; Flags: promptifolder comparetimestamp uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\..\..\isxdl\trunk\source\release\isxdl.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion comparetimestamp uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\..\..\isxdl\trunk\isxdl.iss; DestDir: {app}; Flags: overwritereadonly ignoreversion comparetimestamp uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\..\..\isxdl\trunk\isxdl.htm; DestDir: {app}; Flags: overwritereadonly ignoreversion comparetimestamp uninsremovereadonly sortfilesbyextension replacesameversion
Source: C:\Program Files (x86)\ISTool\SciLexer.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\source\release\ISSLexer.dll; DestDir: {app}; Flags: overwritereadonly ignoreversion uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-de.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-en.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-fr.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-hu.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-pt_BR.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-sv.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\Languages\istool-sw.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: ..\languages\istool-it.lng; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: calltips.txt; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: License.rtf; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: History.rtf; DestDir: {app}; Flags: overwritereadonly uninsremovereadonly sortfilesbyextension replacesameversion
Source: {localappdata}\ISTool\ISTool.ini; DestDir: {userappdata}\ISTool; Flags: external onlyifdoesntexist skipifsourcedoesntexist

[Icons]
Name: {group}\ISTool; Filename: {app}\ISTool.exe; WorkingDir: {app}; Comment: Start ISTool; IconIndex: 0
Name: {group}\ISTool Help; Filename: {app}\ISTool.chm; WorkingDir: {app}; Comment: View ISTool Help; Flags: createonlyiffileexists
Name: {group}\ISTool License; Filename: {app}\License.rtf; WorkingDir: {app}; Comment: View the ISTool License; Flags: createonlyiffileexists
Name: {group}\ISTool History; Filename: {app}\History.rtf; WorkingDir: {app}; Comment: View the history of ISTool; Flags: createonlyiffileexists
Name: {group}\Download DLL Documentation; Filename: {app}\isxdl.htm; WorkingDir: {app}; Comment: Download DLL Documentation; Flags: createonlyiffileexists
Name: {group}\Uninstall ISTool; Filename: {uninstallexe}; Comment: This will remove ISTool from your computer.
Name: {userdesktop}\ISTool; Filename: {app}\ISTool.exe; WorkingDir: {app}; IconIndex: 0; Comment: Start ISTool; Check: DesktopIconCheck

[Registry]
; BEGIN .ISS ASSOCIATION
Root: HKCR; Subkey: .iss; ValueType: string; ValueData: InnoSetupScriptFile
Root: HKCR; Subkey: InnoSetupScriptFile\DefaultIcon; ValueType: string; ValueData: """{app}\ISTool.exe"",0"; Flags: createvalueifdoesntexist
Root: HKCR; Subkey: InnoSetupScriptFile\shell\OpenWithISTool; ValueType: string; ValueData: Open with &ISTool; Flags: uninsdeletekey
Root: HKCR; Subkey: InnoSetupScriptFile\shell\OpenWithISTool; ValueType: none; Flags: uninsdeletekey
Root: HKCR; Subkey: InnoSetupScriptFile\shell\OpenwithISTool\command; ValueType: string; ValueData: """{app}\ISTool.exe"" ""%1"""; Flags: uninsdeletekey
; END .ISS ASSOCIATION

[Run]
Filename: {app}\ISTool.exe; WorkingDir: {app}; Description: Launch ISTool; Flags: nowait postinstall skipifsilent

[Messages]
BeveledLabel=ISTool
SelectLanguageLabel=Please select the language you want this installation and ISTool to use:

[_ISTool]
UseAbsolutePaths=true
;LogFile=compile.log
LogFileAppend=false

[Languages]
Name: english; MessagesFile: compiler:default.isl
Name: german; MessagesFile: compiler:Languages\German.isl
Name: french; MessagesFile: compiler:languages\French.isl
Name: hungarian; MessagesFile: compiler:Languages\Hungarian.isl
Name: portuguese_brazil; MessagesFile: compiler:languages\BrazilianPortuguese.isl
Name: italian; MessagesFile: compiler:languages\Italian.isl
;Name: swedish; MessagesFile: compiler:languages\swedish.isl

[Code]
function IsInstalled( AppID: String ): Boolean;
var
	sPrevPath: String;
begin
	sPrevPath := '';
	if not RegQueryStringValue( HKLM,
		'Software\Microsoft\Windows\CurrentVersion\Uninstall\'+AppID+'_is1',
		'Inno Setup: App Path', sPrevpath) then
		RegQueryStringValue( HKCU, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\'+AppID+'_is1' ,
			'Inno Setup: App Path', sPrevpath);

  Result := sPrevPath<>'';
end;

// If this is an update then we use the same directories again
function ShouldSkipPage(PageID: Integer): Boolean;
begin
	Result := False;
	if (PageID = wpSelectDir) or (PageID = wpSelectProgramGroup) then begin
		Result := IsInstalled('{A9E12684-DD23-4D11-ACAF-6041954BCA00}');
	end;
end;

// Should a desktop icon be installed?
function DesktopIconCheck: Boolean;
var
    i: Integer;
begin
    Result := False;
    for i:=1 to ParamCount do begin
        if Lowercase(ParamStr(i))='desktopicon' then begin
            Result := true;
            Exit;
        end;
    end;
end;

[INI]
Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-en.lng; Languages: english
Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-de.lng; Languages: german
Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-fr.lng; Languages: french
Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-hu.lng; Languages: hungarian
Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-pt_BR.lng; Languages: portuguese_brazil
Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-it.lng; Languages: italian
;Filename: {userappdata}\ISTool\ISTool.ini; Section: prefs; Key: LanguageFile; String: {app}\istool-sw.lng; Languages: swedish

[Dirs]
Name: {userappdata}\ISTool; Flags: uninsneveruninstall

