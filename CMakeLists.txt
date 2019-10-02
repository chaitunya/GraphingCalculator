cmake_minimum_required(VERSION 3.10)
project(GraphingCalculator)
set(QT_VERSION_REQ "5.9")


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core ${QT_VERSION_REQ} REQUIRED)
find_package(Qt5Widgets ${QT_VERSION_REQ} REQUIRED)
find_package(Qt5Gui ${QT_VERSION_REQ} REQUIRED)

include_directories(include /usr/local/include)

add_library(mathpresso SHARED IMPORTED)
set_property(TARGET mathpresso PROPERTY
    IMPORTED_LOCATION "/usr/local/lib/libmathpresso.so")

set(SOURCES
    src/main.cpp
    src/window.cpp
    src/grapher.cpp
    src/function.cpp
    src/equation_widget.cpp
    include/window.h
    include/grapher.h
    include/function.h
    include/equation_widget.h
)

add_executable(GraphingCalculator ${SOURCES} ${MOC_HEADERS})

target_link_libraries(GraphingCalculator
  Qt5::Core
  Qt5::Widgets  
  Qt5::Gui  
  mathpresso
)
