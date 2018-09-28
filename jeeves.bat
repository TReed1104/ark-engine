@ECHO OFF
ECHO --------------------------------
ECHO Jeeves, the cache cleaner
ECHO --------------------------------

ECHO - Cleaning Build Folder
RD /s /q build

ECHO - Cleaning Conan Cache
conan remove * -f