cmake_minimum_required(VERSION 3.3)
project(SDL2Game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp)

add_executable(SDL2Game ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

include_directories(${PROJECT_NAME}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES})

configure_file(Mapy/mapa_obiektow2.txt Mapy/mapa_obiektow2.txt COPYONLY)
configure_file(Mapy/mapa_powierzchni1.txt Mapy/mapa_powierzchni1.txt COPYONLY)
configure_file(Mapy/mapa_powierzchni2.txt Mapy/mapa_powierzchni2.txt COPYONLY)
configure_file(Mapy/Objects1.txt Mapy/Objects1.txt COPYONLY)

configure_file(tekstury/character.png tekstury/character.png COPYONLY)
configure_file(tekstury/drzewo.png tekstury/drzewo.png COPYONLY)
configure_file(tekstury/kappa.png tekstury/kappa.png COPYONLY)
configure_file(tekstury/sciezka.png tekstury/sciezka.png COPYONLY)
configure_file(tekstury/trawa.png tekstury/trawa.png COPYONLY)
