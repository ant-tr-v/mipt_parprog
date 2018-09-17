    cmake_minimum_required(VERSION 3.8)
    project(qt)

    set(CMAKE_CXX_STANDARD 17)

    # включаем поиск библиотек с использованием find_package()
    # через REQUIRED указываем, что библиотеки обязательны
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Gui REQUIRED)
    find_package(Qt5Widgets REQUIRED)

    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    QT5_WRAP_CPP(MOC_Files
        iGridPainter.hpp
        )

    set(SOURCE_FILES main.cpp GridPainter.cpp GridPainter.hpp)
    add_executable(HelloWorld ${SOURCE_FILES})

    # указываем, какие библиотеки необходимо подключить
    target_link_libraries(HelloWorld Qt5::Core)
    target_link_libraries(HelloWorld Qt5::Gui)
    target_link_libraries(HelloWorld Qt5::Widgets)
