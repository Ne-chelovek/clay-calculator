cmake_minimum_required(VERSION 3.16)

project(ClayCalculator)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Принудительный выбор MinGW на Windows
if(WIN32 AND NOT DEFINED CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "MinGW Makefiles" CACHE STRING "CMake generator" FORCE)
    set(CMAKE_CXX_COMPILER "g++" CACHE STRING "C++ compiler" FORCE)
    set(CMAKE_C_COMPILER "gcc" CACHE STRING "C compiler" FORCE)
    message(STATUS "🔧 Forcing MinGW generator")
endif()

# --- XLNT Общие настройки (выносим в переменные)
set(XLNT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/include)
set(XLNT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib)
set(XLNT_LIBRARIES
        ${XLNT_LIB_DIR}/libxlnt.a
        ${XLNT_LIB_DIR}/libfmt.a
)


# Находим все исходные файлы в папке src
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

# --- XLNT для основной программы
target_include_directories(${PROJECT_NAME} PRIVATE ${XLNT_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${XLNT_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${XLNT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE XLNT_STATIC)
# --- XLNT end

# Подключаем необходимые библиотеки Windows
target_link_libraries(${PROJECT_NAME} PRIVATE
        comctl32
        user32
        gdi32
        shell32
)

# --- GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# 1. Собираем все .cpp файлы из папки src для тестов
file(GLOB SRC_FILES_TESTS "src/*.cpp")

# 2. Исключаем main.cpp, так как он содержит свою функцию main()
list(REMOVE_ITEM SRC_FILES_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# 3. Создаем исполняемый файл тестов
add_executable(
        clay_calculator_tests
        tests/test_main.cpp
        ${SRC_FILES_TESTS}
)

# Настройки для тестов (все в одном target_link_libraries)
target_include_directories(clay_calculator_tests PRIVATE
        src
        ${XLNT_INCLUDE_DIR}
)

target_link_directories(clay_calculator_tests PRIVATE
        ${XLNT_LIB_DIR}
)

# Вызов target_link_libraries для тестов
target_link_libraries(clay_calculator_tests PRIVATE
        GTest::gtest_main
        ${XLNT_LIBRARIES}
        comctl32
        user32
        gdi32
        shell32
)

target_compile_definitions(clay_calculator_tests PRIVATE
        XLNT_STATIC
)

include(GoogleTest)
gtest_discover_tests(clay_calculator_tests)