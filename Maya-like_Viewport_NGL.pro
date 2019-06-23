TARGET=Maya-like_Viewport_NGL

QT+=gui opengl core

CONFIG += console
CONFIG -= app_bundle

isEqual(QT_MAJOR_VERSION, 5){
	cache()
	DEFINES +=QT5BUILD
}

QMAKE_CXX=/opt/rh/llvm-toolset-7/root/usr/bin/clang++

OBJECTS_DIR = obj

MOC_DIR = moc

HEADERS += $$PWD/include/*.h \
           $$PWD/include/Viewport/*.h* \
           $$PWD/include/Viewport/Camera/*.h* \
           $$PWD/include/Viewport/Grid/*.h* \
           $$PWD/include/Viewport/LookAt/*.h* \
           $$PWD/include/Viewport/Mouse/*.h* \
           $$PWD/include/Viewport/ProjectionText/*.h* \
           $$PWD/include/Viewport/Select/*.h* \
           $$PWD/include/Viewport/ViewAxis/*.h* \
           include/Viewport/Camera/Camera_Def.hpp

SOURCES += $$PWD/src/*.cpp \
           $$PWD/src/Viewport/*.cpp \
           $$PWD/src/Viewport/Camera/*.cpp \
           $$PWD/src/Viewport/Grid/*.cpp \
           $$PWD/src/Viewport/Mouse/*.cpp \
           $$PWD/src/Viewport/ProjectionText/*.cpp \
           $$PWD/src/Viewport/Select/*.cpp \
           $$PWD/src/Viewport/ViewAxis/*.cpp \
           src/Viewport/LookAt/LookAt.cpp

OTHER_FILES += $$PWD/shaders/Viewport/*.glsl \
               README.md

INCLUDEPATH += ./include

DESTDIR = ./


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

win32: {
        CONFIG += c++1z
        QMAKE_CXXFLAGS_WARN_ON += "-Wno-unused-variable"
}


