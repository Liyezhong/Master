!include(HimalayaDataContainer.pri):error("HimalayaDataContainer.pri not found")

TARGET = HimalayaDataContainer # functional unit name

# header file location
HEADERS += ../Helper/Include/Global.h \
 ../Containers/ContainerBase/Include/*.h \
 ../Containers/ContainerBase/Commands/Include/*.h \
 ../Containers/Programs/Include/*.h \
 ../Containers/Programs/Commands/Include/*.h \
 ../Containers/Reagents/Include/*.h \
 ../Containers/Reagents/Commands/Include/*.h \
 ../Containers/ReagentGroups/Include/*.h \
 ../Containers/ReagentGroups/Commands/Include/*.h \
 ../Containers/ReagentStations/Include/*.h \
 ../Containers/ReagentStations/Commands/Include/*.h \
 ../Containers/ReagentGroupColor/Include/*.h \
 ../SpecialVerifiers/Include/*h \
 ../Containers/DashboardStations/Commands/Include/*h \
 ../Containers/DashboardStations/Include/*h


# source file location
SOURCES += ../Containers/ContainerBase/Source/*.cpp \
 ../Containers/ContainerBase/Commands/Source/*.cpp \
 ../Containers/Programs/Source/*.cpp \
 ../Containers/Programs/Commands/Source/*.cpp \
 ../Containers/Reagents/Source/*.cpp \
 ../Containers/Reagents/Commands/Source/*.cpp \
 ../Containers/ReagentGroups/Source/*.cpp \
 ../Containers/ReagentGroups/Commands/Source/*.cpp \
 ../Containers/ReagentStations/Source/*.cpp \
 ../Containers/ReagentStations/Commands/Source/*.cpp \
 ../Containers/ReagentGroupColor/Source/*.cpp \
 ../SpecialVerifiers/Source/*.cpp \
 ../Containers/DashboardStations/Commands/Source/*.cpp \
 ../Containers/DashboardStations/Source/*.cpp
# Resource file location
RESOURCES += ../Containers/Programs/Resources/ProgramsXml.qrc \
 ../Containers/Reagents/Resources/ReagentsXml.qrc \
 ../Containers/ReagentGroups/Resources/ReagentGroupsXml.qrc \
 ../Containers/ReagentStations/Resources/StationsXml.qrc \
 ../Containers/DashboardStations/Resources/DashboardStationsXml.qrc
