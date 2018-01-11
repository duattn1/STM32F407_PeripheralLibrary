@ECHO off

SET BUILD_LOG_FILE=E:\C\GIT\STM32F4_Smart_Home\Application\UnitTesting\build_log.txt

REM Usage: Test.bat [PATH TO UV4.exe] 

IF "%1"=="" (
SET UV4_EXE=C:\Keil_v5\UV4\UV4.exe
) ELSE (
SET UV4_EXE=%1
)
ECHO Using %UV4_EXE%
REM *******************************************************************

REM Build the application (-b) in headless mode (-j0) (-j0:	Hides the µVision GUI)
ECHO. 
ECHO Building application...
IF EXIST ..\Objects\DIY_Smart_Home.axf DEL ..\Objects\DIY_Smart_Home.axf

%UV4_EXE% -b ..\DIY_Smart_Home.uvprojx -j0 -o "%BUILD_LOG_FILE%" -j0

REM Check if build was successful
IF EXIST ..\Objects\DIY_Smart_Home.axf (
ECHO Build succeeded
) ELSE (
ECHO Build failed
GOTO :done
)
REM *******************************************************************

REM Flash the application (-b) in headless mode (-j0)
ECHO. 
ECHO Loading application to target...
%UV4_EXE% -f ..\DIY_Smart_Home.uvprojx -j0
REM *******************************************************************

REM Debug the application (-b) in headless mode (-j0)
ECHO. 
ECHO Debugging on target...
IF EXIST .\test_results.txt DEL .\test_results.txt

%UV4_EXE% -d ..\DIY_Smart_Home.uvprojx -j0
REM *******************************************************************

REM Check if test results have been written and display them
IF EXIST .\test_results.txt (
ECHO -----------------------
ECHO TEST RESULTS
ECHO -----------------------
TYPE .\test_results.txt 
) ELSE (
ECHO. Test ended abnormally - file Test_results.txt is not produced
GOTO :done
)

:done
PAUSE