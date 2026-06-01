cmake_minimum_required(VERSION 3.16)

project(ClayCalculator)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим все исходные файлы в папке src
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

# Подключаем необходимые библиотеки Windows
target_link_libraries(${PROJECT_NAME} PRIVATE
    comctl32
    user32
    gdi32
    shell32
)
