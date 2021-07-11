@echo. 
@echo. 
@echo. 
@echo. 
echo "int version = %GITHUB_RUN_NUMBER%;" > app_version.h
@call %~dp0.\build_exe.cmd

%~dp0.\build_out\%app_name%.exe now utc compat

