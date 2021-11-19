cmake_minimum_required(VERSION 2.6)
project(pvdss)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(PVDSS_SRCS PVDSS.cpp main.cpp)

qt4_automoc(${PVDSS_SRCS})
add_executable(pvdss ${PVDSS_SRCS})
target_link_libraries(pvdss ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS pvdss RUNTIME DESTINATION bin)
