SET CSP=E:\Program Files (x86)\Codejock Software\MFC\Xtreme ToolkitPro v15.2.1
SET VL_SRC=G:\Visualization_Library_SDK\src
SET VL=J:\VL_Build\src
SET A3DT=J:\Spatial\acisR22
SET X3DT=J:\Spatial\iopR22
set XERCES=I:\xerces-c-3.1.1-x86-windows-vc-9.0
SET HOOPS_INSTALL_DIR=J:\HOOPS-1800
SET HOOPS_ARCH=nt_i386_vc90
SET HOOPS_ARCH_DEBUG=nt_i386_vc90d
SET ARCH=NT_VC9_DLLD
SET ARCH_RELEASE=NT_VC9_DLL
SET ARCH_DEBUG=NT_VC9_DLLD
SET QTDIR=G:\Qt\4.8.3

SET PATH=%QTDIR%\bin;%VL%\Applets\bin\Debug;%VL%\vlMFC\bin\Debug;%VL%\vlMolecule\bin\Debug;%VL%\vlVG\bin\Debug;%VL%\vlVolume\bin\Debug;%VL%\vlGraphics\bin\Debug;%VL%\vlCore\bin\Debug;%VL%\vlWin32\bin\Debug;%XERCES%\BIN;%CSP%\BIN\VC90;%A3DT%\BIN\%ARCH_RELEASE%;%A3DT%\BIN\%ARCH_DEBUG%;%X3DT%\BIN\%ARCH_RELEASE%;%X3DT%\BIN\%ARCH_DEBUG%;%XERCES%\bin;%HOOPS_INSTALL_DIR%\bin\%HOOPS_ARCH%;%HOOPS_INSTALL_DIR%\bin\%HOOPS_ARCH_DEBUG%;%VL%\Applets\bin\Release;%VL%\vlMFC\bin\Release;%VL%\vlMolecule\bin\Release;%VL%\vlVG\bin\Release;%VL%\vlVolume\bin\Release;%VL%\vlGraphics\bin\Release;%VL%\vlCore\bin\Release;%VL%\vlWin32\bin\Release;%PATH%

start "E:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" .\GenViewer_vl_sdi_vc9.sln
