QT += core gui

INCLUDEPATH += \
	$$PWD/inc \
	$$PWD/../sym-library/inc \
	$$PWD/../plugin/inc
	
HEADERS += \
	$$PWD/inc/QPiglerAPI.h \
    $$PWD/../sym-library/inc/PiglerAPI.h \
    $$PWD/../plugin/inc/PiglerProtocol.h