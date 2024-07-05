cmake_minimum_required(VERSION 3.28)
project(mwp)

set(CMAKE_CXX_STANDARD 23)

add_executable(mwp game/main.cpp
        game/Game.cpp
        game/Game.h
        game/scenes/Scene.cpp
        game/scenes/Scene.h
        game/scenes/MainMenu.cpp
        game/scenes/MainMenu.h
        game/scenes/LoadGame.cpp
        game/scenes/LoadGame.h
        game/scenes/MapWithPeople.cpp
        game/scenes/MapWithPeople.h
        game/scenes/people/Person.cpp
        game/scenes/people/Person.h
)
add_executable(mapbuilder
        mapbuilder/main.cpp
        mapbuilder/Osm.cpp
        mapbuilder/Osm.h
        mapbuilder/Map.cpp
        mapbuilder/Map.h
        mapbuilder/HighwayGraph.cpp
        mapbuilder/HighwayGraph.h
        mapbuilder/People.cpp
        mapbuilder/People.h)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

find_package(Osmium REQUIRED COMPONENTS io pbf xml)
include_directories(${OSMIUM_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

target_link_libraries(mapbuilder expat z bz2 cairo)
target_link_libraries(mwp PRIVATE sfml-graphics)
