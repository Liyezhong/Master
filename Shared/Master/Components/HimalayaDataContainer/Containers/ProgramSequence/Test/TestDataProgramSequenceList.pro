!include("../../../Test/TestDataManager.pri"):error("TestDataManager.pri not found")
TARGET = utTestDataProgramSequenceList
SOURCES += TestDataProgramSequenceList.cpp

INCLUDEPATH += ../../../../ \
               ../../../../../../../../Platform/Master/Components \
               ../../../../../../../ColoradoMain/Master/Components/
DEPENDPATH += ../../../../
RESOURCES += ../Resources/*.qrc




UseLib(../../../../ColoradoDataContainer)
UseLib(../../../../../../../../Platform/Master/Components/DataManager ../../../../../../../../Platform/Master/Components/Global)
