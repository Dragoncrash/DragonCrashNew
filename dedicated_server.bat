@echo off
set editorPath="C:\Program Files (x86)\Epic Games\4.14\Engine\Binaries\Win64\UE4Editor.exe"
echo Starting server...
%editorPath% %~dp0\DragonCrashNew.uproject -server -game -log
