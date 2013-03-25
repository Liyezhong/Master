################# include pri file #################
!include("Main.pri") {
    error("Main.pri not found")
}
####################################################

################# override some pri settings #################
TEMPLATE = app
TARGET = himalaya
CONFIG -= static
QT +=   xml \
        network \
        xmlpatterns
#        gui
##############################################################


################# list used platform libraries #################
PLATFORM_COMPONENTS_DIR = ../../../../../../Platform/Master/Components
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
                      ImportExport \
                      Threads \
                      PasswordManager \
                      SoftSwitchManager
################################################################


################# list himalaya shared libraries #################
HIMALAYA_SHAREDCOMPONENTS_DIR = ../../../../../Shared/Master/Components
HIMALAYA_SHAREDCOMPONENTS = HimalayaDataContainer
###########################################################


################# list himalaya libraries #################
HIMALAYA_COMPONENTS_DIR = ../..
HIMALAYA_COMPONENTS = HimalayaGuiController \
                      HimalayaMasterThread \
                      ExportController \
                      Scheduler \
                      HimalayaErrorHandler
###########################################################

################## list simulation libraries #################
#SIMULATION_COMPONENTS_DIR = ../../Scheduler/Demo
#SIMULATION_COMPONENTS = SchedulerSimulation
############################################################

################# own sources #################
INCLUDEPATH += ../..
HEADERS +=  ../Include/Main.h \
            ../../../Include/HimalayaEventCodes.h \
            ../../../Include/DataLoggingSources.h \
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
               ../../../../../../Platform/Common/Components

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

# for yocto 6.0 SDK build
LIBS += -lrt
