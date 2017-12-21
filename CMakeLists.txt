## Add CxxAdaptor.lib to project using source file CppAdaptor.cxx.
#add_library (CxxAdaptor STATIC CppAdaptor.cpp)
#
#target_link_libraries(CxxAdaptor vtkPVPythonCatalyst)
include ( CMakePackageConfigHelpers )
cmake_minimum_required (VERSION 3.6)
project (FEAPCatalyst)

set(FEAPCATALYST_MAJOR_VERSION 0)
set(FEAPCATALYST_MINOR_VERSION 1)
set(FEAPCATALYST_PATCH_VERSION 0)
set(FEAPCATALYST_VERSION
  ${FEAPCATALYSTADAPTER_MAJOR_VERSION}.${FEAPCATALYSTADAPTER_MINOR_VERSION}.${FEAPCATALYSTADAPTER_PATCH_VERSION})


find_package(ParaView REQUIRED COMPONENTS vtkPVCatalyst vtkIOXML vtkPVAnimation)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
#  set(DEF_INSTALL_CMAKE_DIR lib/CMake/FooBar)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

set ( CMAKE_CONFIG_DEST "CMAKE" )


# Make relative paths absolute (needed later on)
#foreach(p LIB BIN INCLUDE CMAKE)
#  set(var INSTALL_${p}_DIR)
#  if(NOT IS_ABSOLUTE "${${var}}")
#    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
#  endif()
#endforeach()

set(PNAME "CxxAdaptor")
message(${PNAME})
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set(PNAME "CxxAdaptor")
else( )
  set(PNAME "CxxAdaptor_32")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )


add_subdirectory(adapter)


#configure_package_config_file(FEAPCatalystConfig.cmake.in
#                              "${CMAKE_BINARY_DIR}/FEAPCatalyst-config.cmake"
#                              INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}" )
#
#message(${PNAME})
#export(TARGETS ${PNAME}
#  FILE "${PROJECT_BINARY_DIR}/FEAPCatalystTargets.cmake")
#
#export ( PACKAGE FEAPCatalyst )
#
#
#configure_package_config_file (
#       "FEAPCatalystConfig.cmake.in"
#       "${CMAKE_BINARY_DIR}/export/FEAPCatalyst-config.cmake"
#       INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}")
#
#
#install(FILES "${CMAKE_BINARY_DIR}/FEAPCatalyst-config.cmake"
#          DESTINATION ${INSTALL_CMAKE_DIR})
#
#install (
#      EXPORT FEAPCatalystTargets
#      DESTINATION ${INSTALL_CMAKE_DIR} FILE "FEAPCatalystTargets.cmake" )
#
