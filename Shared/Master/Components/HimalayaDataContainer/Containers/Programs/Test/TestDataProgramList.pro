!include("../../../Templates/Test/TestDataManager.pri") {
    error("TestDataManager.pri not found")
}


TARGET = utTestDataProgramList

SOURCES += TestDataProgramList.cpp

INCLUDEPATH += ../../../../ \
               ../../../../../../../../Platform/Master/Components \
               ../../../../../../../HimalayaMain/Master/Include/


DEPENDPATH +=  ../../../../

RESOURCES += ../Resources/*.qrc

UseLib(../../../../HimalayaDataManager)
UseLib(../../../../../../../../Platform/Master/Components/DataManager ../../../../../../../../Platform/Master/Components/Global)
#CONFIG  += qtestlib
