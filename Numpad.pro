
TEMPLATE = app
TARGET = NumpadEnhanced
DEPENDPATH += . Buttons
INCLUDEPATH += . Buttons

QT += svg xml

# Input
HEADERS += AboutWindow.h \
           Numpad.h \
           NumpadManager.h \
           SettingsDialog.h \
           Buttons/AbstractBigButton.h \
           Buttons/BigHorizontalButton.h \
           Buttons/BigVerticalButton.h \
           Buttons/Button.h \
           KeyboardHookMsg.h \
           Key.h \
           AddBtnsSettingsDialog.h \
           FirstRunDialog.h \
    Buttons/SvgButton.h
           
SOURCES += AboutWindow.cpp \
           main.cpp \
           Numpad.cpp \
           NumpadManager.cpp \
           SettingsDialog.cpp \
           Buttons/AbstractBigButton.cpp \
           Buttons/BigHorizontalButton.cpp \
           Buttons/BigVerticalButton.cpp \
           Buttons/Button.cpp \
           AddBtnsSettingsDialog.cpp \
           FirstRunDialog.cpp \
    Buttons/SvgButton.cpp
RESOURCES += resourse.qrc
RC_FILE = numpad.rc

DISTFILES +=


