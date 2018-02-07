# include pri file from Master/Test

!include("../../../../../../Platform/Master/Test/Platform.pri") {
    error("../../../../../../Platform/Master/Test/Platform.pri not found")
}

DEFINES += GOOGLE_MOCK
INCLUDEPATH += ../../../../../HimalayaMain/Master/Include \
               ../../../../../Shared/Master/Components \
               ../../../../Common/Components \
               ../../../../../../Platform/Master/Components \
#Add google-mock related headers into our project
INCLUDEPATH += /usr/local/include

###################################
### stuff for adding libraries ####
###################################
#Add google-mock related libraries into our project
PRE_TARGETDEPS  += /usr/local/lib/gmock/libgmock.a
LIBS            += /usr/local/lib/gmock/libgmock.a
PRE_TARGETDEPS  += /usr/local/lib/gtest/libgtest.a
LIBS            += /usr/local/lib/gtest/libgtest.a

EXTRACOMPONENTDIR = ../../../../../../Platform/Master/Components

# this function adds all libraries passed as arguments to the linker and dependency list
defineTest(UseDepLibs){
    Args=$$ARGS
    LIBS += -Wl,--start-group
    for(TheLib, Args) {
        THELIBPATH       = $$EXTRACOMPONENTDIR/$$TheLib/Build/lib_$$CONFIG_SUFFIX
        PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheLib}.a
        LIBS            += $$THELIBPATH/lib$${TheLib}.a
    }
    PRE_TARGETDEPS  += ../../../../../Shared/Master/Components/HimalayaDataContainer/Build/lib_$$CONFIG_SUFFIX/libHimalayaDataContainer.a
    LIBS            += ../../../../../Shared/Master/Components/HimalayaDataContainer/Build/lib_$$CONFIG_SUFFIX/libHimalayaDataContainer.a
    PRE_TARGETDEPS  += ../../HimalayaDataManager/Build/lib_$$CONFIG_SUFFIX/libHimalayaDataManager.a
    LIBS            += ../../HimalayaDataManager/Build/lib_$$CONFIG_SUFFIX/libHimalayaDataManager.a
    PRE_TARGETDEPS  += ../../HimalayaMasterThread/Build/lib_$$CONFIG_SUFFIX/libHimalayaMasterThread.a
    LIBS            += ../../HimalayaMasterThread/Build/lib_$$CONFIG_SUFFIX/libHimalayaMasterThread.a
    PRE_TARGETDEPS  += ../../HimalayaGuiController/Build/lib_$$CONFIG_SUFFIX/libHimalayaGuiController.a
    LIBS            += ../../HimalayaGuiController/Build/lib_$$CONFIG_SUFFIX/libHimalayaGuiController.a
    PRE_TARGETDEPS  += ../../Scheduler/Build/lib_$$CONFIG_SUFFIX/libScheduler.a
    LIBS            += ../../Scheduler/Build/lib_$$CONFIG_SUFFIX/libScheduler.a

    LIBS += -Wl,--end-group
    export(LIBS)
    export(PRE_TARGETDEPS)
}
