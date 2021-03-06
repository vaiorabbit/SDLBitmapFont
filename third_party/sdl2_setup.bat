@echo off
setlocal EnableDelayedExpansion

set SDL_VERSION=2.0.14
set SDL_ARCHIVE=SDL2-devel-!SDL_VERSION!-VC

if exist SDL2 (
    rmdir /s /q SDL2
)
curl -O https://www.libsdl.org/release/!SDL_ARCHIVE!.zip
powershell -command Expand-Archive -Path .\!SDL_ARCHIVE!.zip
ren !SDL_ARCHIVE!\SDL2-!SDL_VERSION! SDL2
move !SDL_ARCHIVE!\SDL2 .
rmdir /s /q !SDL_ARCHIVE!
