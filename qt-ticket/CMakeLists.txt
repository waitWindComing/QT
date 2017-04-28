PROJECT(ticket)
cmake_minimum_required(VERSION 2.8)
set(QT_QMAKE_EXECUTABLE /usr/bin/qmake-4.8)
FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtNetwork)

include_directories(${ticket_SOURCE_DIR}/include)

set(CMAKE_AUTOMOC ON)
set(QT_USE_QTSQL ON)
set(QT_USE_QTWEBKIT ON)
file(GLOB ticket_SOURCES src/*.cpp muparser/src/*.cpp)
file(GLOB ticket_HEADERS include/*.h muparser/include/*.h)
set(ticket_FORMS 
    resource/mainwindow.ui
)
set(ticket_RESOURCES resource/ticket.qrc)

QT4_WRAP_CPP(ticket_HEADERS_MOC ${ticket_HEADERS})
QT4_WRAP_UI(ticket_FORMS_HEADERS ${ticket_FORMS})
QT4_ADD_RESOURCES(ticket_RESOURCES_RCC ${ticket_RESOURCES})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DLIBARCHIVE_STATIC)

ADD_EXECUTABLE(ticket
    ${ticket_SOURCES} 
    ${ticket_HEADERS_MOC}
    ${ticket_FORMS_HEADERS}
    ${ticket_RESOURCES_RCC})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F /Library/Frameworks")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(WIN32)
    LINK_LIBRARIES(${QT_QTMAIN_LIBRARY})
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
    IF(MINGW)
        set(CMAKE_CXX_FLAGS "-mwindows")
    ENDIF(MINGW)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(ticket qjson ${QT_LIBRARIES})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/qjson/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/muparser/include)

ADD_SUBDIRECTORY(qjson/src)

