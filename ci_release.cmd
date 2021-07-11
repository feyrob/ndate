@echo. 
@echo. 
@echo. 
@echo. 
type app_version.h
echo "int version = %GITHUB_RUN_NUMBER%;" > app_version.h
type app_version.h
@call %~dp0.\build_exe.cmd

%~dp0.\build_out\%app_name%.exe now utc compat

