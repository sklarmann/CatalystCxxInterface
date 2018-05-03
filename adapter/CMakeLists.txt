#cmake_minimum_required (VERSION 3.6)
## Offer the user the choice of overriding the installation directories
#set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
#set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
#set(INSTALL_INCLUDE_DIR include CACHE PATH
#  "Installation directory for header files")
#if(WIN32 AND NOT CYGWIN)
#  set(DEF_INSTALL_CMAKE_DIR CMake)
#else()
#  set(DEF_INSTALL_CMAKE_DIR lib/CMake/FooBar)
#endif()
#set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
#  "Installation directory for CMake files")
#
#
## Make relative paths absolute (needed later on)
#foreach(p LIB BIN INCLUDE CMAKE)
#  set(var INSTALL_${p}_DIR)
#  if(NOT IS_ABSOLUTE "${${var}}")
#    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
#  endif()
#endforeach()

include(${PARAVIEW_USE_FILE})


set(SOURCE CppAdaptor.cpp management.h gridHandler.h)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("sources" FILES ${SOURCE})

message(${CMAKE_BINARY_DIR})


add_library (${PNAME} SHARED ${SOURCE})



target_link_libraries(${PNAME} PRIVATE vtkPVCatalyst vtkIOXML vtkPVAnimation)

if((${BUILDCONF} MATCHES ${BUILD_BOTH}))
  set_target_properties( ${PNAME}
      PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/FEAP/DebugCol"
      LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/FEAP/DebugCol"
      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/FEAP/DebugCol"
      ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/FEAP/ReleaseCol"
      LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/FEAP/ReleaseCol"
      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/FEAP/ReleaseCol"
  )
endif()


if((${BUILDCONF} MATCHES ${BUILD_ADAPTER}))
    #set_target_properties(CxxAdaptor PROPERTIES DEBUG_POSTFIX d)
    install(TARGETS ${PNAME}
        EXPORT FEAPCatalystTargets
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
        #PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/foo"
        COMPONENT dev)
endif()
