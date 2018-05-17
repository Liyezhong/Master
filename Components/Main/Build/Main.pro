################# include pri file #################
!include("Main.pri") {
    error("Main.pri not found")
}
####################################################

################# override some pri settings #################
TEMPLATE = app
TARGET = skyline
CONFIG -= static
QT +=   xml \
        network \
        xmlpatterns
#        gui
##############################################################


################# list used platform libraries #################
PLATFORM_COMPONENTS_DIR = ../../../../Shared/Master/Components
PLATFORM_COMPONENTS = DataLogging \
                      DataManager\
                      DeviceControl \
                      EventHandler \
                      NetworkComponents \
                      NetCommands \
                      Global \
                      #DeviceCommandProcessor \
                      StateMachines \
                      ExternalProcessController \
                      ExportController \
                      EncryptionDecryption \
                      Threads \
                      PasswordManager \
#                      GPIOManager \
                      RemoteCareController \
                      HeartBeatManager \
                      RemoteCareManager \
                      SWUpdateManager
################################################################


################# list himalaya shared libraries #################
HIMALAYA_SHAREDCOMPONENTS_DIR = ../../../../Shared/Common/Components
HIMALAYA_SHAREDCOMPONENTS = HimalayaDataContainer
###########################################################


################# list himalaya libraries #################
HIMALAYA_COMPONENTS_DIR = ../..
HIMALAYA_COMPONENTS = HimalayaGuiController \
                      HimalayaMasterThread \
                      Scheduler \
                      HimalayaDataManager
###########################################################

################## list simulation libraries #################
#SIMULATION_COMPONENTS_DIR = ../../Scheduler/Demo
#SIMULATION_COMPONENTS = SchedulerSimulation
############################################################

################# own sources #################
INCLUDEPATH += ../..
HEADERS +=  ../Include/Main.h \
            ../../../Include/HimalayaEventCodes.h \
            ../../../Include/HimalayaProcessExitCodes.h
SOURCES +=  ../Source/Main.cpp
###############################################





###########################################################################################
################# DO NOT TOUCH THIS UNLESS YOU KNOW WHAT YOU ARE DOING!!! #################
###########################################################################################

################# set destination path
DESTDIR = bin_$$CONFIG_SUFFIX

################# edit include path
INCLUDEPATH += $$PLATFORM_COMPONENTS_DIR \
               $$HIMALAYA_SHAREDCOMPONENTS_DIR \
               $$HIMALAYA_COMPONENTS_DIR \
#               $$SIMULATION_COMPONENTS_DIR \
               ../../../../../Shared/Common/Components

################# start group
LIBS += -Wl,--start-group \

################# include platform libraries and set dependencies
for(TheComponent, PLATFORM_COMPONENTS) {
    THELIBPATH       = $$PLATFORM_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################# include himalaya shared libraries and set dependencies
for(TheComponent, HIMALAYA_SHAREDCOMPONENTS) {
    THELIBPATH       = $$HIMALAYA_SHAREDCOMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################# include himalaya libraries and set dependencies
for(TheComponent, HIMALAYA_COMPONENTS) {
    THELIBPATH       = $$HIMALAYA_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################## include simulation libraries and set dependencies
#for(TheComponent, SIMULATION_COMPONENTS) {
#    THELIBPATH       = ../../Scheduler/Demo/$$SIMULATION_COMPONENTS
#    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
#    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
#}

################# end group
LIBS += -Wl,--end-group

# for yocto 8.0.2 SDK build
LIBS += -lrt
LIBS += -pthread
LIBS += -ldl

################ external libraries and headers
XSDEDir =

contains(QMAKE_HOST.arch, x86):{
    CONFIG += 32bit
}

contains(QMAKE_HOST.arch, x86_64):{
    CONFIG += 64bit
}


CONFIG(32bit) {
    XSDEDir = ../../../../Shared/ExternalPackages/xsde/xsde-3.2.0-i686-linux-gnu/libxsde
}

CONFIG(64bit) {
    XSDEDir = ../../../../Shared/ExternalPackages/xsde/xsde-3.2.0-x86_64-linux-gnu/libxsde
}

INCLUDEPATH += $$XSDEDir
DEPENDPATH +=  $$XSDEDir

LIBS += $$XSDEDir/xsde/libxsde.a
QMAKE_PRE_LINK = make -C $$XSDEDir

