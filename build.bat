@echo off

if not exist build\ mkdir build

set warnings=/W4 /WX
set includes=/I../third_party/glfw/include /I../third_party/glad/include
set link=../third_party/glfw/build/src/Release/glfw3.lib opengl32.lib gdi32.lib user32.lib shell32.lib

pushd build

cl /nologo /FC /Zi /MD %warnings% ..\src\application.c %includes% %link%

popd