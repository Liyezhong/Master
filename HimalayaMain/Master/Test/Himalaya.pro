# build all components recursive

TEMPLATE = subdirs

SUBDIRS += ../../../../Platform/Master/Components/EventHandler/Test/EventHandler.pro \
            ../Components/Scheduler/Test/Scheduler.pro

CONFIG += ordered
