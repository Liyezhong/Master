######################
# common stuff for all components
######################

TEMPLATE = lib

CONFIG += static \
          warn_on \
          qt \
          thread \
          rtti

QT += gui \
      core \
      xml \
      xmlpatterns \
      network

INCLUDEPATH += ../.. \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../../Platform/ServiceSW/Components \
               ../../../../../../Himalaya/HimalayaMain/Master/Components \
               ../../../../../Shared/Gui/Components  \
               ../../../../../../Platform/Master/Components/DeviceControl/Include/Devices \
               ../../../../../../Platform/Master/Components/DeviceControl/Include/Interface \
               ../../../../../../Platform/ProtoTest/Include \
               ../../../../../../Himalaya/ServiceSW/Master/Components/ServiceDeviceController/Include/DeviceProcessor/Helper/
DEPENDPATH +=  ../.. \
               ../../../../../../Platform/Master/Components \
               ../../../../../../Platform/ServiceSW/Components \
               ../../../../../../Platform/Gui/Components \
               ../../../../../Himalaya/HimalayaMain/Master/Components \
               ../../../../../Shared/Gui/Components

CONFIG += debug_and_release
#CONFIG += build_all

QMAKE_CXXFLAGS += -Wall -std=c++0x

CONFIG(debug, debug|release) {
    CONFIG_SUFFIX = dbg
} else {
    CONFIG_SUFFIX = rel
    DEFINES += QT_NO_DEBUG \
               QT_NO_DEBUG_OUTPUT \
               NDEBUG
    CONFIG(gcov) {
        QMAKE_CXXFLAGS_RELEASE += -fprofile-arcs -ftest-coverage
        QMAKE_LFLAGS_RELEASE += -fprofile-arcs
        QMAKE_CXXFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE += -O0
    }
}

CONFIG(crosstgt) {
### To be able to build Target run qmake as follows:
####### qmake CONFIG+=crosstgt
    CONFIG_SUFFIX = $${CONFIG_SUFFIX}_tgt
    DEFINES += TARGET_BUILD
}

OBJECTS_DIR = obj_$${CONFIG_SUFFIX}
MOC_DIR = moc_$${CONFIG_SUFFIX}
DESTDIR = lib_$${CONFIG_SUFFIX}

######################
