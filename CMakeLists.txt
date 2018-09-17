cmake_minimum_required(VERSION 3.8)
project(qt)

set(CMAKE_CXX_STANDARD 17)

# включаем поиск библиотек с использованием find_package()
# через REQUIRED указываем, что библиотеки обязательны
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

# Указывает CMake запускать MOC когда это необходимо
set(CMAKE_AUTOMOC ON)

# По скольку MOC файлы генерируются в дирекотрии bin, необходимо
# указать CMake включать эти файлы (из этой директории)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt5Widgets_INCLUDES включает QtCore и QtGui include пути
include_directories(${Qt5Widgets_INCLUDES})

# Добавляет -DQT_WIDGETS_LIB при использовании QtWidgets в Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Добавление флагов компиляции для Qt проекта
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#OMP

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp GridPainter.cpp GridPainter.hpp)
add_executable(HelloWorld ${SOURCE_FILES})

# Указывает какие модули использовать в проекте
qt5_use_modules(HelloWorld Widgets)

# указываем, какие библиотеки необходимо подключить
target_link_libraries(HelloWorld Qt5::Core)
target_link_libraries(HelloWorld Qt5::Gui)
target_link_libraries(HelloWorld Qt5::Widgets)
