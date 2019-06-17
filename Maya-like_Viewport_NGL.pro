TARGET=Maya-like_Viewport_NGL

QT+=gui opengl core

isEqual(QT_MAJOR_VERSION, 5){
	cache()
	DEFINES +=QT5BUILD
}

QMAKE_CXX=/opt/rh/llvm-toolset-7/root/usr/bin/clang++

OBJECTS_DIR = obj

MOC_DIR = moc

CONFIG -= app_bundle

HEADERS += $$PWD/include/*.h \
           $$PWD/include/Viewport/*.h \
           $$PWD/include/Viewport/*.hpp

SOURCES += $$PWD/src/*.cpp \
           $$PWD/src/Viewport/*.cpp

OTHER_FILES += $$PWD/shaders/Viewport/*.glsl \
               README.md

INCLUDEPATH += ./include

DESTDIR = ./

CONFIG += console

!equals(PWD, $${OUT_PWD}){
    copydata.commands = echo "creating destination dirs" ;
    copydata.commands += mkdir -p $$OUT_PWD/shaders ;
    copydata.commands += echo "copying files" ;
    copydata.commands += $(COPY_DIR) $$PWD/shaders/* $$OUT_PWD/shaders/ ;
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){
	message("including $HOME/NGL")
	include($(HOME)/NGL/UseNGL.pri)
}
else{
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}


