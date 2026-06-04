; Inno Setup script cho Ezsnagit (C++/Qt6).
; Biên dịch: package.bat đã chạy windeployqt -> dist\Ezsnagit\, rồi ISCC installer.iss.

#define MyAppName "Ezsnagit"
#ifndef MyAppVersion
  #define MyAppVersion "0.4.0"
#endif
#define MyAppPublisher "Ez"
#define MyAppExeName "Ezsnagit.exe"

[Setup]
; AppId riêng cho Ezsnagit — giữ cố định để cập nhật tại chỗ (cùng AppId = cài đè).
AppId={{A1F3C2E7-9B4D-4A61-8E2C-7D5F0B3A6C19}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\Ezsnagit
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
; Cài per-user (không cần admin) — khớp định vị "nhẹ, không phiền".
PrivilegesRequired=lowest
OutputDir=installer
OutputBaseFilename=Ezsnagit-Setup-{#MyAppVersion}
SetupIconFile=assets\icon.ico
UninstallDisplayIcon={app}\{#MyAppExeName}
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Tạo lối tắt ngoài Desktop"; GroupDescription: "Lối tắt:"

[Files]
; Toàn bộ thư mục portable do windeployqt dựng (exe + Qt DLL + plugins).
Source: "dist\Ezsnagit\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\Gỡ {#MyAppName}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Mở {#MyAppName} ngay"; Flags: nowait postinstall skipifsilent
