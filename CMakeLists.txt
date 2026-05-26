cmake_minimum_required(VERSION 3.16)
project(ClayCalculator)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим все исходные файлы в папке src
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Путь к заголовочным файлам xlnt
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/include
)

# Путь к библиотеке xlnt (.lib файлу)
target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib
)

# Подключаем саму библиотеку
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib/libxlnt.a
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib/libfmt.a
)

# Определяем макрос для статической линковки (ВАЖНО!)
target_compile_definitions(${PROJECT_NAME} PRIVATE XLNT_STATIC)
