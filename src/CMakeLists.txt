# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/8-10>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

 cmake_minimum_required(VERSION 2.8.0)
 project(SCHEMATIC)
 
 set (CMAKE_CXX_FLAGS            "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
 set (CMAKE_INSTALL_PREFIX       "../install" )

 find_package(LibXml2 REQUIRED)
 find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

    include_directories( ${SCHEMATIC_SOURCE_DIR}
                         ${LIBXML2_INCLUDE_DIR}
                       )

                    set( includes     Indentation.h
                                      XmlUtil.h 
                                      Point.h
                                      Box.h
                                      Term.h
                                      Net.h
                                      Instance.h
                                      Node.h
                                      Line.h
                                      Shape.h
                                      TermShape.h
                                      BoxShape.h
                                      LineShape.h
                                      EllipseShape.h
                                      ArcShape.h
                                      Symbol.h
                                      Cell.h
                                      InstancesWidget.h
                                      InstancesModel.h
                                      CellsLib.h
                                      CellsModel.h
                       )

                    set( mocIncludes  SaveCellDialog.h
                                      OpenCellDialog.h
                                      CellWidget.h
                                      CellViewer.h
                       )

                    set( cpps         Indentation.cpp
                                      XmlUtil.cpp 
                                      Point.cpp
                                      Box.cpp
                                      Term.cpp
                                      Net.cpp
                                      Instance.cpp
                                      Node.cpp
                                      Line.cpp
                                      Shape.cpp
                                      TermShape.cpp
                                      BoxShape.cpp
                                      LineShape.cpp
                                      EllipseShape.cpp
                                      ArcShape.cpp
                                      Symbol.cpp
                                      Cell.cpp
                                      SaveCellDialog.cpp
                                      OpenCellDialog.cpp
                                      CellWidget.cpp
                                      CellViewer.cpp
                                      InstancesWidget.cpp
                                      InstancesModel.cpp
                                      CellsLib.cpp
                                      CellsModel.cpp
                                      Main.cpp
                       )

           #qt4_wrap_cpp( mocCpps      ${mocIncludes} )
           qt5_wrap_cpp( mocCpps ${mocIncludes} )

         add_executable( tme810       ${cpps} ${mocCpps} )
  target_link_libraries( tme810       Qt5::Widgets Qt5::Core ${LIBXML2_LIBRARIES} )
                install( TARGETS      tme810                      DESTINATION bin )
                install( FILES        ${includes} ${mocIncludes}  DESTINATION include )