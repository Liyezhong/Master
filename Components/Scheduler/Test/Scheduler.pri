# include pri file from Master/Test

!include("../../../../Shared/Master/Test/Platform.pri") {
    error("../../../../Shared/Master/Test/Platform.pri not found")
}

DEFINES += GOOGLE_MOCK
INCLUDEPATH += ../../../../Master/Include \
               ../../../../Shared/Master/Components \
               ../../../../Shared/Common/Components \
               ../../../../Shared/Master/Components \
#Add google-mock related headers into our project
INCLUDEPATH += /usr/local/include

###################################
### stuff for adding libraries ####
###################################
#Add google-mock related libraries into our project
PRE_TARGETDEPS  += ../../../../Shared/ExternalPackages/gmock/lib/gmock/libgmock.a
LIBS            += ../../../../Shared/ExternalPackages/gmock/lib/gmock/libgmock.a
PRE_TARGETDEPS  += ../../../../Shared/ExternalPackages/gmock/lib/gtest/libgtest.a
LIBS            += ../../../../Shared/ExternalPackages/gmock/lib/gtest/libgtest.a

EXTRACOMPONENTDIR = ../../../../Shared/Master/Components

# this function adds all libraries passed as arguments to the linker and dependency list
defineTest(UseDepLibs){
    Args=$$ARGS
    LIBS += -Wl,--start-group
    for(TheLib, Args) {
        THELIBPATH       = $$EXTRACOMPONENTDIR/$$TheLib/Build/lib_$$CONFIG_SUFFIX
        PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheLib}.a
        LIBS            += $$THELIBPATH/lib$${TheLib}.a
    }
    PRE_TARGETDEPS  += ../../../../Shared/Common/Components/HimalayaDataContainer/Build/lib_$$CONFIG_SUFFIX/libHimalayaDataContainer.a
    LIBS            += ../../../../Shared/Common/Components/HimalayaDataContainer/Build/lib_$$CONFIG_SUFFIX/libHimalayaDataContainer.a
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
