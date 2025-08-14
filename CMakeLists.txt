cmake_minimum_required(VERSION 3.16)

project(ss_sim VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the Qt6 package
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Include source files from the subdirectories
file(GLOB_RECURSE SRC_FILES
    "src/physics/*.cpp"
    "src/visualization/*.cpp"
)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Link the Qt modules to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)