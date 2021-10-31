cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

project(Chip8CPP)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

set(sources
    "src/Emulator.cpp"
    "src/CPU.cpp"
	"src/main.cpp"
)

add_executable(${PROJECT_NAME} ${sources})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)
