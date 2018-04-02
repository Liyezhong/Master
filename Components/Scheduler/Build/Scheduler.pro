!include(Scheduler.pri) {
    error("Scheduler.pri not found")
}

TARGET = Scheduler

INCLUDEPATH += ../../../../Shared/Common/Components \
               ../../../../Shared/Externals/Hypodermic \

SOURCES += ../Source/*.cpp \
            ../Commands/Source/*.cpp

HEADERS += ../Include/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Interface/*.h \
../../../../Shared/Master/Components/DeviceControl/Test/Mock/*.h \
           ../../../../Shared/Master/Components/DeviceControl/Include/Global/*.h \
            ../Commands/Include/*.h

INCLUDEPATH +=../../../../Shared/Externals/boost_1_66_0/
LIBS+="L../../../../Shared/Externals/boost_1_66_0/state/lib/"

RESOURCES +=

QT += xml

















