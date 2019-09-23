TARGET = MayaNGL

QT+=gui opengl core

CONFIG += console
CONFIG -= app_bundle

isEqual(QT_MAJOR_VERSION, 5){
	cache()
	DEFINES +=QT5BUILD
}

OBJECTS_DIR = obj

MOC_DIR = moc

HEADERS += $$PWD/include/*.h \
           $$PWD/include/MayaNGL/*.h* \
           $$PWD/include/MayaNGL/Common/*.h* \
           $$PWD/include/MayaNGL/Mouse/*.h* \
           $$PWD/include/MayaNGL/Camera/*.h* \
           $$PWD/include/MayaNGL/Viewport/*.h* \
           $$PWD/include/MayaNGL/Viewport/Grid/*.h* \
           $$PWD/include/MayaNGL/Viewport/ProjectionText/*.h* \
           $$PWD/include/MayaNGL/Viewport/ViewAxis/*.h* \
           $$PWD/include/MayaNGL/Selection/*.h* \
           $$PWD/include/MayaNGL/SelectableMap/*.h* \
           $$PWD/include/MayaNGL/Gizmo/*.h*

SOURCES += $$PWD/src/*.cpp \
           $$PWD/src/MayaNGL/*.cpp \
           $$PWD/src/MayaNGL/Common/*.cpp \
           $$PWD/src/MayaNGL/Mouse/*.cpp \
           $$PWD/src/MayaNGL/Camera/*.cpp \
           $$PWD/src/MayaNGL/Viewport/*.cpp \
           $$PWD/src/MayaNGL/Viewport/Grid/*.cpp \
           $$PWD/src/MayaNGL/Viewport/ProjectionText/*.cpp \
           $$PWD/src/MayaNGL/Viewport/ViewAxis/*.cpp \
           $$PWD/src/MayaNGL/Selection/*.cpp \
           $$PWD/src/MayaNGL/SelectableMap/*.cpp \
           $$PWD/src/MayaNGL/Gizmo/*.cpp

OTHER_FILES += $$PWD/shaders/*.glsl \
               $$PWD/shaders/MayaNGL/*.glsl \
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


