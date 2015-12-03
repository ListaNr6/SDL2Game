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

configure_file(tekstury/arrow_down1.png tekstury/arrow_down1.png COPYONLY)
configure_file(tekstury/arrow_down2.png tekstury/arrow_down2.png COPYONLY)
configure_file(tekstury/arrow_left1.png tekstury/arrow_left1.png COPYONLY)
configure_file(tekstury/arrow_left2.png tekstury/arrow_left2.png COPYONLY)
configure_file(tekstury/arrow_right1.png tekstury/arrow_right1.png COPYONLY)
configure_file(tekstury/arrow_right2.png tekstury/arrow_right2.png COPYONLY)
configure_file(tekstury/arrow_up1.png tekstury/arrow_up1.png COPYONLY)
configure_file(tekstury/arrow_up2.png tekstury/arrow_up2.png COPYONLY)
configure_file(tekstury/bird.png tekstury/bird.png COPYONLY)
configure_file(tekstury/bird2.png tekstury/bird2.png COPYONLY)
configure_file(tekstury/character.png tekstury/character.png COPYONLY)
configure_file(tekstury/character_down.png tekstury/character_down.png COPYONLY)
configure_file(tekstury/character_left.png tekstury/character_left.png COPYONLY)
configure_file(tekstury/character_right.png tekstury/character_right.png COPYONLY)
configure_file(tekstury/character_up.png tekstury/character_up.png COPYONLY)
configure_file(tekstury/closed_door.png tekstury/closed_door.png COPYONLY)
configure_file(tekstury/drzewo.png tekstury/drzewo.png COPYONLY)
configure_file(tekstury/gate.png tekstury/gate.png COPYONLY)
configure_file(tekstury/kappa.png tekstury/kappa.png COPYONLY)
configure_file(tekstury/key.png tekstury/key.png COPYONLY)
configure_file(tekstury/key_door.png tekstury/key_door.png COPYONLY)
configure_file(tekstury/lever_left.png tekstury/lever_left.png COPYONLY)
configure_file(tekstury/lever_right.png tekstury/lever_right.png COPYONLY)
configure_file(tekstury/open_door.png tekstury/open_door.png COPYONLY)
configure_file(tekstury/sciezka.png tekstury/sciezka.png COPYONLY)
configure_file(tekstury/trawa.png tekstury/trawa.png COPYONLY)
configure_file(tekstury/wall.png tekstury/wall.png COPYONLY)
configure_file(tekstury/wooden_floor.png tekstury/wooden_floor.png COPYONLY)
