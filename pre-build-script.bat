@ECHO OFF
ECHO Removing Old Content
RMDIR /s /q .\content
RMDIR /s /q .\bin\content
RMDIR /s /q .\content_json
RMDIR /s /q .\bin\content_json
ECHO Copying Content
XCOPY /q /i /e /y ..\content .\content
XCOPY /q /i /e /y ..\content .\bin\content
XCOPY /q /i /e /y ..\content_json .\content_json
XCOPY /q /i /e /y ..\content_json .\bin\content_json