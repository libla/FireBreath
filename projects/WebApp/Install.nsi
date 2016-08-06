; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

!include "LogicLib.nsh"

; 安装程序初始定义常量
!define PRODUCT_NAME "$(LANG_APP_NAME)"
!define PRODUCT_VERSION "1.0.1.0"
!define PRODUCT_PUBLISHER "$(LANG_COMPANY_NAME)"
!define PRODUCT_WEB_SITE "http://1a.3k.topgame.kr"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor /SOLID lzma
SetCompressorDictSize 32

Unicode true

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"

; MUI 预定义常量
!define MUI_ABORTWARNING
!define MUI_ICON "idr_main.ico"
!define MUI_UNICON "idr_main.ico"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 许可协议页面
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "agreement.txt"
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
; 开始菜单设置页面
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "Korean"

LangString LANG_APP_NAME ${LANG_ENGLISH} "Era Zero"
LangString LANG_COMPANY_NAME ${LANG_ENGLISH} "SHENZHEN TIANWANG KONGJIAN TECHNOLOGY CO. LTD."
LangString LANG_INSTALL_1 ${LANG_ENGLISH} "Will install Era Zero, continue?"
LangString LANG_INSTALL_2 ${LANG_ENGLISH} "Install Era Zero success!"
LangString LANG_INSTALL_3 ${LANG_ENGLISH} "Install Era Zero failed, please contact customer service!"
LangString LANG_UNINSTALL_1 ${LANG_ENGLISH} "Are you sure you want to completely remove "
LangString LANG_UNINSTALL_2 ${LANG_ENGLISH} " and all the components?"
LangString LANG_UNINSTALL_3 ${LANG_ENGLISH} " has been successfully removed from your computer."

LangString LANG_APP_NAME ${LANG_SIMPCHINESE} "零纪元"
LangString LANG_COMPANY_NAME ${LANG_SIMPCHINESE} "深圳市天网空间科技有限公司"
LangString LANG_INSTALL_1 ${LANG_SIMPCHINESE} "即将安装零纪元，继续吗？"
LangString LANG_INSTALL_2 ${LANG_SIMPCHINESE} "安装零纪元成功！"
LangString LANG_INSTALL_3 ${LANG_SIMPCHINESE} "安装零纪元失败，请联系客服！"
LangString LANG_UNINSTALL_1 ${LANG_SIMPCHINESE} "您确实要完全移除 "
LangString LANG_UNINSTALL_2 ${LANG_SIMPCHINESE} " ，及其所有的组件？"
LangString LANG_UNINSTALL_3 ${LANG_SIMPCHINESE} " 已成功地从您的计算机移除。"

LangString LANG_APP_NAME ${LANG_KOREAN} "퍼스트 에이지"
LangString LANG_COMPANY_NAME ${LANG_KOREAN} "SHENZHEN TIANWANG KONGJIAN TECHNOLOGY CO. LTD."
LangString LANG_INSTALL_1 ${LANG_KOREAN} "퍼스트 에이지를 설치하시겠습니까?"
LangString LANG_INSTALL_2 ${LANG_KOREAN} "퍼스트 에이지 설치성공!"
LangString LANG_INSTALL_3 ${LANG_KOREAN} "퍼스트 에이지 설치에 실패했습니다. 고객센터로 문의주시기 바랍니다!"
LangString LANG_UNINSTALL_1 ${LANG_KOREAN} " "
LangString LANG_UNINSTALL_2 ${LANG_KOREAN} " 의 모든 파일을 제거하시겠습니까?"
LangString LANG_UNINSTALL_3 ${LANG_KOREAN} " 를 컴퓨터에서 제거 했습니다."

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "setup-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
;ShowInstDetails hide
;ShowUnInstDetails show
SilentInstall silent
BrandingText "${PRODUCT_NAME}"

; 激活安装日志记录，该日志文件将会作为卸载文件的依据(注意，本区段必须放置在所有区段之前)
Section "-LogSetOn"
  ;LogSet on
SectionEnd

Section "MainSection" SEC01
  SetOutPath $INSTDIR
  SetOverwrite on
  File /r ".\WebPluginInst\*.*"

; 创建开始菜单快捷方式
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  ;CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\九天传_网页版.lnk" "$INSTDIR\九天传.url"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  RegDLL $INSTDIR\NPWebApp.dll
SectionEnd


/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/

; 根据安装日志卸载文件的调用宏
!macro DelFileByLog LogFile
  ifFileExists `${LogFile}` 0 +4
    Push `${LogFile}`
    Call un.DelFileByLog
    Delete `${LogFile}`
!macroend

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  UnRegDLL $INSTDIR\NPWebApp.dll
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
	;卸载 data
 	Delete "$INSTDIR\data\*.*"
	
  ; 调用宏只根据安装日志卸载安装程序自己安装过的文件
  !insertmacro DelFileByLog "$INSTDIR\install.log"

  ; 清除安装程序创建的且在卸载时可能为空的子目录，对于递归添加的文件目录，请由最内层的子目录开始清除(注意，不要带 /r 参数，否则会失去 DelFileByLog 的意义)
  RMDir "$SMPROGRAMS\$ICONS_GROUP"

  RMDir /r "$INSTDIR\Log"
  RMDir /r "$INSTDIR\data"
  RMDir /r "$INSTDIR\res"
	
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#
Function .onInit
	MessageBox MB_YESNO "$(LANG_INSTALL_1)" IDYES NoAbort
	 Abort ; causes installer to quit.
	NoAbort:
	StrCpy $INSTDIR "$APPDATA\WebApp"
FunctionEnd

Function .onInstSuccess
    MessageBox MB_OK "$(LANG_INSTALL_2)"
FunctionEnd

Function .onInstFailed
    MessageBox MB_OK "$(LANG_INSTALL_3)"
 FunctionEnd



Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(LANG_UNINSTALL_1)$(^Name)$(LANG_UNINSTALL_2)" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name)$(LANG_UNINSTALL_3)"
FunctionEnd

; 以下是卸载程序通过安装日志卸载文件的专用函数，请不要随意修改
Function un.DelFileByLog
  Exch $R0
  Push $R1
  Push $R2
  Push $R3
  FileOpen $R0 $R0 r
  ${Do}
    FileRead $R0 $R1
    ${IfThen} $R1 == `` ${|} ${ExitDo} ${|}
    StrCpy $R1 $R1 -2
    StrCpy $R2 $R1 11
    StrCpy $R3 $R1 20
    ${If} $R2 == "File: wrote"
    ${OrIf} $R2 == "File: skipp"
    ${OrIf} $R3 == "CreateShortCut: out:"
    ${OrIf} $R3 == "created uninstaller:"
      Push $R1
      Push `"`
      Call un.DelFileByLog.StrLoc
      Pop $R2
      ${If} $R2 != ""
        IntOp $R2 $R2 + 1
        StrCpy $R3 $R1 "" $R2
        Push $R3
        Push `"`
        Call un.DelFileByLog.StrLoc
        Pop $R2
        ${If} $R2 != ""
          StrCpy $R3 $R3 $R2
          Delete /REBOOTOK $R3
        ${EndIf}
      ${EndIf}
    ${EndIf}
    StrCpy $R2 $R1 7
    ${If} $R2 == "Rename:"
      Push $R1
      Push "->"
      Call un.DelFileByLog.StrLoc
      Pop $R2
      ${If} $R2 != ""
        IntOp $R2 $R2 + 2
        StrCpy $R3 $R1 "" $R2
        Delete /REBOOTOK $R3
      ${EndIf}
    ${EndIf}
  ${Loop}
  FileClose $R0
  Pop $R3
  Pop $R2
  Pop $R1
  Pop $R0
FunctionEnd

Function un.DelFileByLog.StrLoc
  Exch $R0
  Exch
  Exch $R1
  Push $R2
  Push $R3
  Push $R4
  Push $R5
  StrLen $R2 $R0
  StrLen $R3 $R1
  StrCpy $R4 0
  ${Do}
    StrCpy $R5 $R1 $R2 $R4
    ${If} $R5 == $R0
    ${OrIf} $R4 = $R3
      ${ExitDo}
    ${EndIf}
    IntOp $R4 $R4 + 1
  ${Loop}
  ${If} $R4 = $R3
    StrCpy $R0 ""
  ${Else}
    StrCpy $R0 $R4
  ${EndIf}
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Pop $R1
  Exch $R0
FunctionEnd
