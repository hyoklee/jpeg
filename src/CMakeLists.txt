cmake_minimum_required (VERSION 3.2.2)
project (JPEG_SRC C)

#-----------------------------------------------------------------------------
# Define jpeg Library
#-----------------------------------------------------------------------------
set (JPEG_SRCS
    ${JPEG_SRC_SOURCE_DIR}/jaricom.c
    ${JPEG_SRC_SOURCE_DIR}/jcapimin.c
    ${JPEG_SRC_SOURCE_DIR}/jcapistd.c
    ${JPEG_SRC_SOURCE_DIR}/jcarith.c
    ${JPEG_SRC_SOURCE_DIR}/jccoefct.c
    ${JPEG_SRC_SOURCE_DIR}/jccolor.c
    ${JPEG_SRC_SOURCE_DIR}/jcdctmgr.c
    ${JPEG_SRC_SOURCE_DIR}/jchuff.c
    ${JPEG_SRC_SOURCE_DIR}/jcinit.c
    ${JPEG_SRC_SOURCE_DIR}/jcmainct.c
    ${JPEG_SRC_SOURCE_DIR}/jcmarker.c
    ${JPEG_SRC_SOURCE_DIR}/jcmaster.c
    ${JPEG_SRC_SOURCE_DIR}/jcomapi.c
    ${JPEG_SRC_SOURCE_DIR}/jcparam.c
    ${JPEG_SRC_SOURCE_DIR}/jcprepct.c
    ${JPEG_SRC_SOURCE_DIR}/jcsample.c
    ${JPEG_SRC_SOURCE_DIR}/jctrans.c
    ${JPEG_SRC_SOURCE_DIR}/jdapimin.c
    ${JPEG_SRC_SOURCE_DIR}/jdapistd.c
    ${JPEG_SRC_SOURCE_DIR}/jdarith.c
    ${JPEG_SRC_SOURCE_DIR}/jdatadst.c
    ${JPEG_SRC_SOURCE_DIR}/jdatasrc.c
    ${JPEG_SRC_SOURCE_DIR}/jdcoefct.c
    ${JPEG_SRC_SOURCE_DIR}/jdcolor.c
    ${JPEG_SRC_SOURCE_DIR}/jddctmgr.c
    ${JPEG_SRC_SOURCE_DIR}/jdhuff.c
    ${JPEG_SRC_SOURCE_DIR}/jdinput.c
    ${JPEG_SRC_SOURCE_DIR}/jdmainct.c
    ${JPEG_SRC_SOURCE_DIR}/jdmarker.c
    ${JPEG_SRC_SOURCE_DIR}/jdmaster.c
    ${JPEG_SRC_SOURCE_DIR}/jdmerge.c
    ${JPEG_SRC_SOURCE_DIR}/jdpostct.c
    ${JPEG_SRC_SOURCE_DIR}/jdsample.c
    ${JPEG_SRC_SOURCE_DIR}/jdtrans.c
    ${JPEG_SRC_SOURCE_DIR}/jerror.c
    ${JPEG_SRC_SOURCE_DIR}/jfdctflt.c
    ${JPEG_SRC_SOURCE_DIR}/jfdctfst.c
    ${JPEG_SRC_SOURCE_DIR}/jfdctint.c
    ${JPEG_SRC_SOURCE_DIR}/jidctflt.c
    ${JPEG_SRC_SOURCE_DIR}/jidctfst.c
    ${JPEG_SRC_SOURCE_DIR}/jidctint.c
    ${JPEG_SRC_SOURCE_DIR}/jquant1.c
    ${JPEG_SRC_SOURCE_DIR}/jquant2.c
    ${JPEG_SRC_SOURCE_DIR}/jutils.c
    ${JPEG_SRC_SOURCE_DIR}/jmemmgr.c
    ${JPEG_SRC_SOURCE_DIR}/${MEMORYMGR}.c
)

set (JPEG_HDRS
    ${JPEG_SRC_SOURCE_DIR}/jdct.h
    ${JPEG_SRC_SOURCE_DIR}/jinclude.h
    ${JPEG_SRC_SOURCE_DIR}/jmemsys.h
    ${JPEG_SRC_SOURCE_DIR}/jpegint.h
    ${JPEG_SRC_SOURCE_DIR}/jversion.h
)

set (JPEG_PUBLIC_HEADERS
    ${JPEG_SRC_SOURCE_DIR}/jerror.h
    ${JPEG_SRC_SOURCE_DIR}/jmorecfg.h
    ${JPEG_SRC_SOURCE_DIR}/jpeglib.h
)

add_library (${JPEG_LIB_TARGET} STATIC ${JPEG_SRCS} ${JPEG_HDRS} ${JPEG_PUBLIC_HEADERS})
target_include_directories(${JPEG_LIB_TARGET} PRIVATE "${JPEG_SRC_SOURCE_DIR};${CMAKE_BINARY_DIR}")
TARGET_C_PROPERTIES (${JPEG_LIB_TARGET} STATIC)
target_link_libraries (${JPEG_LIB_TARGET} PRIVATE ${LINK_LIBS})
JPEG_SET_LIB_OPTIONS (${JPEG_LIB_TARGET} ${JPEG_LIB_NAME} STATIC)
set_target_properties(${JPEG_LIB_TARGET} PROPERTIES
    PUBLIC_HEADER "${JPEG_PUBLIC_HEADERS}"
    LINKER_LANGUAGE C
    INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
set_global_variable (JPEG_LIBRARIES_TO_EXPORT ${JPEG_LIB_TARGET})
set (install_targets ${JPEG_LIB_TARGET})

if (BUILD_SHARED_LIBS)
  add_library (${JPEG_LIBSH_TARGET} SHARED ${JPEG_SRCS} ${JPEG_HDRS} ${JPEG_PUBLIC_HEADERS})
  target_include_directories(${JPEG_LIBSH_TARGET} PRIVATE "${JPEG_SRC_SOURCE_DIR};${CMAKE_BINARY_DIR}")
  TARGET_C_PROPERTIES (${JPEG_LIBSH_TARGET} SHARED)
  target_link_libraries (${JPEG_LIBSH_TARGET} PRIVATE ${LINK_LIBS})
  JPEG_SET_LIB_OPTIONS (${JPEG_LIBSH_TARGET} ${JPEG_LIB_NAME} SHARED)
  set_target_properties(${JPEG_LIBSH_TARGET} PROPERTIES
      COMPILE_DEFINITIONS "JPEG_BUILT_AS_DYNAMIC_LIB"
      PUBLIC_HEADER "${JPEG_PUBLIC_HEADERS}"
      LINKER_LANGUAGE C
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS JPEG_BUILT_AS_DYNAMIC_LIB=1
  )
  set_global_variable (JPEG_LIBRARIES_TO_EXPORT "${JPEG_LIBRARIES_TO_EXPORT};${JPEG_LIBSH_TARGET}")
  set (install_targets ${install_targets} ${JPEG_LIBSH_TARGET})
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (JPEG_EXPORTED_TARGETS)
  if (BUILD_SHARED_LIBS)
    INSTALL_TARGET_PDB (${JPEG_LIBSH_TARGET} ${JPEG_INSTALL_BIN_DIR} libraries)
  endif (BUILD_SHARED_LIBS)
  INSTALL_TARGET_PDB (${JPEG_LIB_TARGET} ${JPEG_INSTALL_BIN_DIR} libraries)

  install (
      TARGETS
          ${install_targets}
      EXPORT
          ${JPEG_EXPORTED_TARGETS}
      LIBRARY DESTINATION ${JPEG_INSTALL_LIB_DIR} COMPONENT libraries
      ARCHIVE DESTINATION ${JPEG_INSTALL_LIB_DIR} COMPONENT libraries
      RUNTIME DESTINATION ${JPEG_INSTALL_BIN_DIR} COMPONENT libraries
      FRAMEWORK DESTINATION ${JPEG_INSTALL_FWRK_DIR} COMPONENT libraries
      PUBLIC_HEADER DESTINATION ${JPEG_INSTALL_INCLUDE_DIR} COMPONENT headers
  )
endif ()
