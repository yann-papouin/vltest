SET CSP=C:\Program Files (x86)\Codejock Software\MFC\Xtreme ToolkitPro v15.2.1
SET VL_SRC=d:\spa_instdir\VizLib_svn\src
SET VL_DATA_PATH=d:\spa_instdir\VizLib_svn\data
SET VL=d:\spa_instdir\VizLib_vc9_x86\src
SET A3DT=d:\SPA_INSTDIR\acisR22
SET X3DT=d:\SPA_INSTDIR\iopR22
set XERCES=d:\SPA_INSTDIR\xerces-c-built_2_7_0
SET HOOPS_INSTALL_DIR=d:\SPA_INSTDIR\HOOPS-1918
SET HOOPS_ARCH=nt_i386_vc90
SET HOOPS_ARCH_DEBUG=nt_i386_vc90d
SET ARCH=NT_VC9_DLLD
SET ARCH_RELEASE=NT_VC9_DLL
SET ARCH_DEBUG=NT_VC9_DLLD

SET PATH=%VL%\vlMolecule\bin\Debug;%VL%\vlVG\bin\Debug;%VL%\vlVolume\bin\Debug;%VL%\Applets\bin\Debug;%VL%\vlMFC\bin\Debug;%VL%\vlGraphics\bin\Debug;%VL%\vlCore\bin\Debug;%VL%\vlWin32\bin\Debug;%VL%\Applets\bin\Release;%VL%\vlMFC\bin\Release;%VL%\vlGraphics\bin\Release;%VL%\vlCore\bin\Release;%VL%\vlWin32\bin\Release;%XERCES%\BIN;%CSP%\BIN\VC90;%A3DT%\BIN\%ARCH_RELEASE%;%A3DT%\BIN\%ARCH_DEBUG%;%X3DT%\BIN\%ARCH_RELEASE%;%X3DT%\BIN\%ARCH_DEBUG%;%XERCES%\bin;%HOOPS_INSTALL_DIR%\bin\%HOOPS_ARCH%;%HOOPS_INSTALL_DIR%\bin\%HOOPS_ARCH_DEBUG%;%PATH%

"c:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" viewer\GenViewer_vl_sdi_vc9.sln
