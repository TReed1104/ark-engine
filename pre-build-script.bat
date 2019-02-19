@ECHO OFF
ECHO Removing Old Content
RMDIR /s /q .\content
RMDIR /s /q .\bin\content

ECHO Copying Content
XCOPY /q /i /e /y ..\content .\content
XCOPY /q /i /e /y ..\content .\bin\content

ECHO Creating Logging Directories
RMDIR .\logs
RMDIR .\bin\logs
MKDIR .\logs
MKDIR .\bin\logs