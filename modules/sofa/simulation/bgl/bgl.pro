# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./modules/sofa/simulation/bgl
# Target is a library:  sofabgl$$LIBSUFFIX
load(sofa/pre)

TEMPLATE = lib
TARGET = sofabgl

HEADERS += bgl.h \
           bfs_adapter.h \
           dfs_adapter.h \
           dfv_adapter.h \
           BglNode.h \
		   BglMultiMappingElement.h \
           BglSimulation.h \
           BglGraphManager.h \
           BglGraphManager.inl \
           GetObjectsVisitor.h

SOURCES += BglNode.cpp \
		   BglMultiMappingElement.cpp \
           BglSimulation.cpp \
           BglGraphManager.cpp  \
           GetObjectsVisitor.cpp

contains(DEFINES,SOFA_SMP){
HEADERS += \ SMPBglSimulation.h

SOURCES += \ SMPBglSimulation.cpp
}

DEFINES += SOFA_BUILD_SIMULATION_BGL

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$ROOT_SRC_DIR/applications
DEPENDPATH -= $$ROOT_SRC_DIR/applications

load(sofa/post)
