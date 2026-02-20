cmake_minimum_required(VERSION 4.0)
project(QTAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick Qml QuickControls2 QuickWidgets)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)
qt_standard_project_setup()

qt_add_executable(QTAI
    src/main.cpp
    src/qtai.cpp
    src/etl.cpp
    src/knn.cpp
    include/main.hpp
    include/qtai.hpp
    include/etl.hpp
    include/algorithm.hpp
    include/knn.hpp
)

if(MSVC)
    target_compile_options(QTAI PRIVATE /O2 /arch:AVX2)
else()
    target_compile_options(QTAI PRIVATE -O2 -march=native)
endif()

qt_add_qml_module(QTAI
    URI QTAI
    VERSION 1.0
    QML_FILES
        qml/qtai.qml
        qml/knn.qml
        qml/kmc.qml
)

qt_add_resources(QTAI "mnist_data"
    PREFIX "/data"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES
        mnist/train-images.idx3-ubyte
        mnist/train-labels.idx1-ubyte
        mnist/t10k-images.idx3-ubyte
        mnist/t10k-labels.idx1-ubyte
)

target_include_directories(QTAI PRIVATE include)

target_link_libraries(QTAI PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2
    Qt6::QuickWidgets
)

if(WIN32)
    find_program(WINDEPLOYQT windeployqt HINTS "${Qt6_DIR}/../../../bin")
    if(WINDEPLOYQT)
        add_custom_command(TARGET QTAI POST_BUILD
            COMMAND "${WINDEPLOYQT}"
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/qml"
                "$<TARGET_FILE:QTAI>"
            COMMENT "Deploying Qt dependencies..."
        )
    endif()
endif()