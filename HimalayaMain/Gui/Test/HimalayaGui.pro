# build all components recursive

TEMPLATE = subdirs
SUBDIRS = ../Components/Programs/Test/TestPrograms.pro\
          ../Components/Reagents/Test/TestReagents.pro

CONFIG += ordered
