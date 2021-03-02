cmake_minimum_required(VERSION 3.14)

project(filtermusic-headless LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core WebSockets Multimedia LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core WebSockets Multimedia LinguistTools REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	find_package(QT NAMES Qt6 Qt5 COMPONENTS Gui REQUIRED)
	find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
endif()

set(TS_FILES filtermusic-headless_en_150.ts)

add_executable(filtermusic-headless
	src/main.cpp
	src/server.cpp
	src/audio/audioplayer.cpp
	${TS_FILES}
)
target_link_libraries(filtermusic-headless Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Multimedia)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(filtermusic-headless Qt${QT_VERSION_MAJOR}::Gui)
endif()

if(COMMAND qt_create_translation)
	qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
	qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
