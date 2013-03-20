!include("../../../Templates/Test/TestDataManager.pri") {
    error("TestDataManagement.pri not found")
}

TARGET = utTestDataReagentList

SOURCES += TestDataReagentList.cpp

INCLUDEPATH += ../../../../ \
                ../../../../../../../../Platform/Master/Components \
                ../../../../../../../HimalayaMain/Master/Include/


DEPENDPATH +=  ../../../../

RESOURCES += ../Resources/*.qrc
UseLib(../../../../HimalayaDataManager)
UseLib(../../../../../../../../Platform/Master/Components/DataManager ../../../../../../../../Platform/Master/Components/Global)
