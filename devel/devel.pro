TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast
QMAKE_CXXFLAGS += -Wconversion -Winit-self

QMAKE_CXXFLAGS += -std=c++17


SOURCES += main.cpp

HEADERS += \
    conecpt.h \
    full_example.h \
    variant_helper.h




