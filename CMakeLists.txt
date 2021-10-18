cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

project(Chip8CPP)

set(sources
    "src/CPU.cpp"
	"src/main.cpp"
)

add_executable(${PROJECT_NAME} ${sources})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
