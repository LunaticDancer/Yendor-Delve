xcopy ".\assets" ".\bin\Debug\assets" /c /d /i /y /s /e
xcopy ".\assets" ".\bin\x64\Debug\assets" /c /d /i /y /s /e
cd build
premake5.exe gmake
cd ..
mingw32-make clean
mingw32-make
pause
