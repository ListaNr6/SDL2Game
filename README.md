REQUIREMENTS
* MinGW
* SDL2
* SDL_image 2.0

INSTALLING SDL2 UNDER MINGW
* Download SDL2, SDL_image 2.0 (devel-mingw)
* Copy the contents of SDL2\i686-w64-mingw32\, SDL2_image-2.0.0\i686-w64-mingw32\
-> MinGW\
* https://hg.libsdl.org/SDL/raw-file/e217ed463f25/include/SDL_platform.h
-> \MinGW\include\SDL2\

BUILDING WITH CLION/CMAKE
* https://github.com/tcbrindle/sdl2-cmake-scripts
* FindSDL2.cmake
* FINDSDL2_image.cmake
-> {CLion}\bin\cmake\share\cmake-3.3\Modules\
