
#define MyAppVersion "1.1.6"
#define MyAppName    "CoOS"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher=coocox.org
AppPublisherURL=http://www.coocox.org/
AppSupportURL=http://www.coocox.org/CoOS.htm

AppId={#MyAppName}
CreateUninstallRegKey=yes
Uninstallable=yes
UninstallDisplayName={#MyAppName}
UninstallDisplayIcon={app}\CoOS.exe

DefaultDirName={pf}\CooCox\{#MyAppName}
DefaultGroupName=CooCox Software

Compression=lzma
SolidCompression=yes
OutputDir=dist
OutputBaseFilename={#MyAppName}-{#MyAppVersion}
ChangesAssociations=yes
ChangesEnvironment=yes


[Files]
Source: "CoOS\*"; DestDir: "{app}"; Flags: recursesubdirs



[Code]
var
ErrorCode: Integer;
IsRunning: Integer;
ResultStr: String;
ResultCode: Integer;
VersionStr: String;
// 安装时判断程序是否正在运行
function InitializeSetup(): Boolean;
begin
Result :=true; //安装程序继续
end;


// 卸载时判断程序是否正在运行
function InitializeUninstall(): Boolean;
begin
   Result :=true; //安装程序继续

end;





