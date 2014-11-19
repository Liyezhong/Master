!include("TestSVCScreenLock.pri"):error("TestSVCScreenLock.pri not found")

TARGET 	 = utTestSVCScreenLock
CONFIG   += console
SOURCES  += TestSVCScreenLock.cpp
TEMPLATE  = app


INCLUDEPATH += ../../ \
               ../../../../../../Shared/Master/Components \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \

CONFIG += qtestlib
CONFIG += qt
DEPENDPATH += ../../../../

UseLibs(Core ServiceDataManager SVCScreenLock Diagnostics)
UsePlatformGUILibs(MainMenu KeyBoard Application)
UsePlatformLibs(DataManager Global PasswordManager)
# UseSharedLibs(HimalayaDataContainer)
UsePlatformScroller(QtScroller)

