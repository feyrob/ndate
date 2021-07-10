@call %~dp0.\prepare_env.cmd

@%tcc_exe_abspath% -o build_out\%app_name%.exe main_win64.c


