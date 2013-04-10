!include("../../../Test/DataManager.pri"):error("DataManager.pri not found")

TARGET = utTestProcessSettings
SOURCES += TestProcessSettings.cpp

INCLUDEPATH += ../../../../ \
 ../../../../../../../../Platform/Master/Components/

DEPENDPATH += ../../../../ \

UseLib(../../../../DataManager ../../../../Global)

RESOURCES += \
    ../Resources/ProcessSettings.qrc

OTHER_FILES += \
    ../Resources/ProcessSettings.xml
