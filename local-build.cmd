REM run as Administrator
@echo off
cd /d %~dp0
set DOWNLOADS_DIR=%USERPROFILE%\Downloads
set DOWNLOADS_DIR_LINUX=%DOWNLOADS_DIR:\=/%

@REM SET PATH=^
@REM %DOWNLOADS_DIR%\PortableGit\bin;^
@REM %DOWNLOADS_DIR%\x86_64-8.1.0-release-posix-seh-rt_v6-rev0;^
@REM %DOWNLOADS_DIR%\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\bin;^
@REM %DOWNLOADS_DIR%\cmake-3.22.2-windows-x86_64\bin;

set PATH=^
D:\Softwares\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\mingw64;^
D:\Softwares\x86_64-8.1.0-release-posix-seh-rt_v6-rev0\mingw64\bin;^
D:\Softwares\cmake-3.23.0-rc1-windows-x86_64\bin;

@REM -DCURL_DIR="%DOWNLOADS_DIR_LINUX%/curl-v7.86.0-x86_64-posix-seh-rev0-8.1.0/lib/cmake/CURL" ^

cmake.exe -G"MinGW Makefiles" ^
-DCMAKE_BUILD_TYPE=Debug ^
-DPortAudio_DIR="D:/Code Projects/cpp-libraries/portaudio-v19.7.0-mingw64-x86_64-posix-seh-rev0-8.1.0/lib/cmake/portaudio" ^
-DPortAudioCpp_DIR="D:/Code Projects/cpp-libraries/portaudiocpp-v19.7.0-mingw64-x86_64-posix-seh-rev0-8.1.0" ^
-DCURL_DIR="D:/Code Projects/cpp-libraries/curl-v7.86.0-x86_64-posix-seh-rev0-8.1.0/lib/cmake/CURL" ^
-B./cmake-build &&^
cd cmake-build &&^
cmake --build . &&^
echo "Successful build"
pause