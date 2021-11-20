cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)

project(Chip8CPP)

find_package(SFML COMPONENTS system graphics window audio CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

set(sources
    "src/GUI/imgui_file_browser.cpp"
    "src/GUI/HUD.cpp"
    "src/Emulator.cpp"
    "src/CPU.cpp"
	"src/main.cpp"
)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(${PROJECT_NAME} ${sources})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui::imgui
    ImGui-SFML::ImGui-SFML
    sfml-system
    sfml-graphics
    sfml-window
    sfml-audio
)
