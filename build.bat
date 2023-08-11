@echo off

if not exist build\ mkdir build

pushd build

cl /nologo /FC /Zi /W4 /WX ..\src\application.c

popd