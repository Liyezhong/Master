# include pri file from Master/Test

!include("../../../../../../Platform/Master/Test/Platform.pri") {
    error("../../../../../../Platform/Master/Test/Platform.pri not found")
}

INCLUDEPATH += ../../../../../Shared/Master/Components \
               ../../../../Common/Components \
               ../../../../../../Platform/Master/Components \

###################################
### stuff for adding libraries ####
###################################
EXTRACOMPONENTDIR = ../../../../../../Platform/Master/Components/

# this function adds all libraries passed as arguments to the linker and dependency list
defineTest(UseDepLibs){
    Args=$$ARGS
    LIBS += -Wl,--start-group
    for(TheLib, Args) {
        THELIBPATH       = $$EXTRACOMPONENTDIR/$$TheLib/Build/lib_$$CONFIG_SUFFIX
        PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheLib}.a
        LIBS            += $$THELIBPATH/lib$${TheLib}.a
        PRE_TARGETDEPS  += ../../Scheduler/Build/lib_$$CONFIG_SUFFIX/libScheduler.a
        LIBS            += ../../Scheduler/Build/lib_$$CONFIG_SUFFIX/libScheduler.a
    }
    LIBS += -Wl,--end-group
    export(LIBS)
    export(PRE_TARGETDEPS)
}
