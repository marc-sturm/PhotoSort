TEMPLATE = subdirs
CONFIG += console

#Library targets and depdendencies
SUBDIRS = cppCORE\
        cppGUI\
        PhotoSort

cppGUI.depends = cppCORE
PhotoSort.depends = cppCORE cppGUI
