@echo off
set editorPath="../Engine/Binaries/Win64/UE4Editor.exe"
echo Starting server...
%editorPath% %~dp0\DragonCrashNew.uproject -server -game -log
