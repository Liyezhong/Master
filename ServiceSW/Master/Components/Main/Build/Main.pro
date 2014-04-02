################# include pri file #################
!include("Main.pri") {
    error("Main.pri not found")
}
####################################################

################# override some pri settings #################
TEMPLATE = app
TARGET = himalaya_service
CONFIG -= static
QT +=   xml \
        network \
        xmlpatterns
##############################################################


################# list used platform libraries #################
PLATFORM_COMPONENTS_DIR = ../../../../../../Platform/Master/Components
PLATFORM_COMPONENTS = NetworkComponents \
                      Threads \
                      DataLogging \
                      DataManager \
                      EventHandler \
                      NetCommands \
                      DeviceControl \
                      DeviceCommandProcessor \
                      StateMachines \
                      ExternalProcessController \
                      GPIOManager \
                      ExportController \
                      ImportExport \
                      Global

################# list used platform gui libraries #################
PLATFORM_GUI_COMPONENTS_DIR = ../../../../../../Platform/Gui/Components
PLATFORM_GUI_COMPONENTS = Application \
                          MainMenu \
                          KeyBoard

################# list used platform service SW libraries #################
PLATFORM_SERVICESW_COMPONENTS_DIR = ../../../../../../Platform/ServiceSW/Components
PLATFORM_SERVICESW_COMPONENTS = LogViewer \
                                Settings

################# list used shared libraries #################
SHARED_COMPONENTS_DIR = ../../../../../Shared/Gui/Components
SHARED_COMPONENTS = Images

##############################################################

################# list used himalaya specific libraries #################
HIMALAYA_MAIN_COMPONENTS_DIR = ../../../../../HimalayaMain/Master/Components
HIMALAYA_MAIN_COMPONENTS = \



##############################################################


################# list himalaya service main libraries #############
HIMALAYA_COMPONENTS_DIR = ../..
HIMALAYA_COMPONENTS = TestCases  \
                      ServiceWidget \
                      Calibration \
                      SystemTracking \
                      LogViewer \
                      Diagnostics \
                      ServiceUpdates \
                      ServiceMasterThreadController \
                      ServiceDeviceController \
                      Core  \
                      ServiceDataManager
###########################################################

################# list himalaya shared libraries #################
HIMALAYA_SHAREDCOMPONENTS_DIR = ../../../../../Shared/Master/Components
HIMALAYA_SHAREDCOMPONENTS = HimalayaDataContainer


################# own sources #################
INCLUDEPATH += ../..
HEADERS *= ../Include/*.h
SOURCES += ../Source/ServiceMain.cpp
###############################################


###########################################################################################
################# DO NOT TOUCH THIS UNLESS YOU KNOW WHAT YOU ARE DOING!!! #################
###########################################################################################

################# set destination path
DESTDIR = bin_$$CONFIG_SUFFIX

################# edit include path
INCLUDEPATH += $$PLATFORM_COMPONENTS_DIR \
               $$PLATFORM_GUI_COMPONENTS_DIR \
               $$PLATFORM_SERVICESW_COMPONENTS_DIR \
               $$SHARED_COMPONENTS_DIR \
               $$HIMALAYA_MAIN_COMPONENTS_DIR \
               $$HIMALAYA_COMPONENTS_DIR


################# start group
LIBS += -Wl,--start-group \

################# include kineticscroller library and set dependencies
THELIBPATH       = $$PLATFORM_GUI_COMPONENTS_DIR/kineticscroller/lib
PRE_TARGETDEPS  += $$THELIBPATH/libQtScroller.a
LIBS            += $$THELIBPATH/libQtScroller.a

################# include platform libraries and set dependencies
for(TheComponent, PLATFORM_COMPONENTS) {
    THELIBPATH       = $$PLATFORM_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################# include platform gui libraries and set dependencies
for(TheComponent, PLATFORM_GUI_COMPONENTS) {
    THELIBPATH       = $$PLATFORM_GUI_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################# include platform service sw libraries and set dependencies
for(TheComponent, PLATFORM_SERVICESW_COMPONENTS) {
    THELIBPATH       = $$PLATFORM_SERVICESW_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################# include shared libraries and set dependencies
for(TheComponent, SHARED_COMPONENTS) {
    THELIBPATH       = $$SHARED_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
    PRE_TARGETDEPS  += $$THELIBPATH/lib$${TheComponent}.a
    LIBS            += $$THELIBPATH/lib$${TheComponent}.a
}

################# include himalaya main libraries and set dependencies
for(TheComponent, HIMALAYA_MAIN_COMPONENTS) {
    THELIBPATH       = $$HIMALAYA_MAIN_COMPONENTS_DIR/$${TheComponent}/Build/lib_$$CONFIG_SUFFIX
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

################# end group
LIBS += -Wl,--end-group
