@echo off
rmdir /q /s Release
del JoeBlade3_remake.zip
md Release

call PrepareTools.bat

:: Prepare levels
copy Resources\LevOpt.exe Resources\Levels\
copy Resources\MakeResFile.exe Resources\Levels\
cd Resources\Levels
LevOpt
MakeResFile Rooms
move joeblade3.rooms ..\..\Release\
move joeblade3.tiles ..\..\Release\
move Rooms.hdr ..\..\Release\
move Rooms.res ..\..\Release\
rmdir /q /s Rooms
del LevOpt.exe
del MakeResFile.exe
cd ..

:: Prepare other resources
MakeResFile Common
MakeResFile Music
MakeResFile Sounds
MakeResFile Sprites
MakeResFile Tiles
move Common.* ..\Release\
move Music.* ..\Release\
move Sounds.* ..\Release\
move Music.* ..\Release\
move Sprites.* ..\Release\
move Tiles.* ..\Release\
cd ..

:: Copy libraries and executables
copy Lib\Bass\bass.dll Release\
:: copy Lib\SDL_bcb\SDL.dll Release\
copy Lib\SDL_Compat\*.dll Release\
copy Lib\BcbRuntime\* Release\
copy JoeBlade\Win32\Release\*.exe Release\

:: Copy additional files
copy JoeBlade\joeblade3.hitbl Release\
copy JoeBlade\joeblade3.music Release\
copy JoeBlade\JoeBlade3.cfg Release\
copy Readme.md Release\
type nul > Release\slot1.sav
type nul > Release\slot2.sav
type nul > Release\slot3.sav
type nul > Release\slot4.sav
type nul > Release\slot5.sav
type nul > Release\slot6.sav
type nul > Release\slot7.sav

powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('Release', 'JoeBlade3_remake.zip'); }"
rmdir /q /s Release