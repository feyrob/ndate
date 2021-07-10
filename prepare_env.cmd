

@echo off
md build_out > nul 2>&1
call:get_tcc
call:get_app_name
@echo on

@goto:eof


:get_app_name
	rem get parent_dir_name
	for %%I in ("%~dp0.\x") do for %%J in ("%%~dpI.") do set parent_dir_name=%%~nxJ
	set app_name=%parent_dir_name%
@goto:eof

:get_tcc
	set tcc_exe_abspath=%~dp0.\temp_tools\tcc\tcc.exe
	
	if exist %tcc_exe_abspath% (
		@goto:eof
	)
	
	@echo on
	md %~dp0.\temp_tools\tcc
	md %~dp0.\temp_download
	set tcc_zip_uri=http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win64-bin.zip
	set tcc_zip_abspath=%~dp0.\temp_download\tcc.zip

	rem download zip file
	powershell -NoLogo -Command "add-type 'using System.Net;using System.Security.Cryptography.X509Certificates;public class TrustAllCertsPolicy : ICertificatePolicy {public bool CheckValidationResult(ServicePoint srvPoint, X509Certificate certificate,WebRequest request, int certificateProblem) {return true;}}';[System.Net.ServicePointManager]::CertificatePolicy = New-Object TrustAllCertsPolicy;(new-object System.Net.WebClient).DownloadFile($env:tcc_zip_uri, $env:tcc_zip_abspath)"
	
	rem unpack zip file
	powershell Expand-Archive -Path %tcc_zip_abspath% -DestinationPath %~dp0.\temp_tools
	@echo off
@goto:eof
