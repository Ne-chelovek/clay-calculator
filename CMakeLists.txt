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

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Для Windows: предотвращаем переопределение настроек компилятора/линкера родительского проекта
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 1. Собираем все .cpp файлы из папки src
file(GLOB SRC_FILES "src/*.cpp")

# 2. Исключаем main.cpp, так как он содержит свою функцию main()
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# 3. Создаем исполняемый файл тестов
add_executable(
        clay_calculator_tests
        tests/test_main.cpp
        ${SRC_FILES}
)

target_link_libraries(
        clay_calculator_tests
        GTest::gtest_main
)

target_include_directories(clay_calculator_tests PRIVATE src)

include(GoogleTest)
gtest_discover_tests(clay_calculator_tests)
