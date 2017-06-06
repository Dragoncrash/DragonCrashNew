@echo off
set editorPath="../Engine/Binaries/Win64/UE4Editor.exe"
set serverIP=127.0.0.1
set /p serverIP=Enter Server IP Address (Default = 127.0.0.1): 
echo Connecting to server at %serverIP%...
%editorPath% %~dp0\DragonCrashNew.uproject %serverIP% -game
