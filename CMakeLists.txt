cmake_minimum_required(VERSION 3.16)

project(ClayCalculator)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем компилятор и выбираем соответствующие библиотеки
if(MSVC)
    message(STATUS "🔧 Building with MSVC")
    set(XLNT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/include)
    set(XLNT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib/msvc)

    # Ищем .lib файлы
    file(GLOB XLNT_LIB_FILES "${XLNT_LIB_DIR}/*.lib")
    set(XLNT_LIBRARIES ${XLNT_LIB_FILES})

    if(NOT XLNT_LIBRARIES)
        message(WARNING "MSVC libraries not found at ${XLNT_LIB_DIR}")
        message(WARNING "Please build xlnt for MSVC or install via vcpkg")
    endif()

elseif(MINGW)
    message(STATUS "🔧 Building with MinGW")
    set(XLNT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/include)
    set(XLNT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib/mingw)
    set(XLNT_LIBRARIES
            ${XLNT_LIB_DIR}/libxlnt.a
            ${XLNT_LIB_DIR}/libfmt.a
    )

    if(NOT EXISTS ${XLNT_LIB_DIR}/libxlnt.a)
        message(WARNING "MinGW libraries not found at ${XLNT_LIB_DIR}")
    endif()

else()
    message(STATUS "🔧 Building with ${CMAKE_CXX_COMPILER_ID}")
    # Fallback to default paths
    set(XLNT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/include)
    set(XLNT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xlnt/lib)
    set(XLNT_LIBRARIES
            ${XLNT_LIB_DIR}/libxlnt.a
            ${XLNT_LIB_DIR}/libfmt.a
    )
endif()

# Проверка наличия заголовков
if(NOT EXISTS ${XLNT_INCLUDE_DIR}/xlnt/xlnt.hpp)
    message(FATAL_ERROR "xlnt headers not found at ${XLNT_INCLUDE_DIR}")
endif()

# Находим все исходные файлы в папке src
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

# --- XLNT для основной программы
target_include_directories(${PROJECT_NAME} PRIVATE ${XLNT_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${XLNT_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${XLNT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE XLNT_STATIC)

# Подключаем необходимые библиотеки Windows
target_link_libraries(${PROJECT_NAME} PRIVATE
        comctl32
        user32
        gdi32
        shell32
)
