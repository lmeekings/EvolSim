cmake_minimum_required(VERSION 3.16)
project(EvolSim)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

#The project files
set(SOURCE_FILES
	main.cpp
	creature.cpp
	environment.cpp
	trait.cpp
	battle.cpp)

add_executable(EvolSim ${SOURCE_FILES})
target_link_libraries(EvolSim PRIVATE sfml-graphics)
target_compile_features(EvolSim PRIVATE cxx_std_17)