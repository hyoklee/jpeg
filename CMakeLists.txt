cmake_minimum_required (VERSION 3.2.2)
PROJECT (JPEG C)

#-----------------------------------------------------------------------------
# Basic JPEG stuff here
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
set (JPEG_RESOURCES_DIR          ${JPEG_SOURCE_DIR}/config/cmake)
set (JPEG_SRC_DIR                ${JPEG_SOURCE_DIR}/src)
set (JPEG_TEST_DIR               ${JPEG_SOURCE_DIR}/testfiles)
set (JPEG_EXAMPLES_SOURCE_DIR    ${JPEG_SOURCE_DIR}/examples)

#-----------------------------------------------------------------------------
# Set the core names of all the libraries
#-----------------------------------------------------------------------------
set (JPEG_LIB_CORENAME           "jpeg")

#-----------------------------------------------------------------------------
# Set the true names of all the libraries if customized by external project
#-----------------------------------------------------------------------------
set (JPEG_LIB_NAME              "${JPEG_EXTERNAL_LIB_PREFIX}${JPEG_LIB_CORENAME}")

#-----------------------------------------------------------------------------
# Set the target names of all the libraries
#-----------------------------------------------------------------------------
set (JPEG_LIB_TARGET             "${JPEG_LIB_CORENAME}-static")
set (JPEG_LIBSH_TARGET           "${JPEG_LIB_CORENAME}-shared")

if (APPLE)
  option (JPEG_BUILD_FRAMEWORKS "TRUE to build as frameworks libraries, FALSE to build according to BUILD_SHARED_LIBS" FALSE)
endif ()

if (NOT JPEG_INSTALL_BIN_DIR)
  set (JPEG_INSTALL_BIN_DIR bin)
endif ()
if (NOT JPEG_INSTALL_LIB_DIR)
  if (APPLE)
    set (JPEG_INSTALL_FMWK_DIR ${CMAKE_INSTALL_FRAMEWORK_PREFIX})
  endif ()
  set (JPEG_INSTALL_LIB_DIR lib)
endif ()
if (NOT JPEG_INSTALL_INCLUDE_DIR)
  set (JPEG_INSTALL_INCLUDE_DIR include)
endif ()
if (NOT JPEG_INSTALL_DATA_DIR)
  if (NOT WIN32)
    if (APPLE)
      if (JPEG_BUILD_FRAMEWORKS)
        set (JPEG_INSTALL_EXTRA_DIR ../SharedSupport)
      else ()
        set (JPEG_INSTALL_EXTRA_DIR share)
      endif ()
      set (JPEG_INSTALL_FWRK_DIR ${CMAKE_INSTALL_FRAMEWORK_PREFIX})
    endif ()
    set (JPEG_INSTALL_DATA_DIR share)
    set (JPEG_INSTALL_CMAKE_DIR share/cmake)
  else ()
    set (JPEG_INSTALL_DATA_DIR ".")
    set (JPEG_INSTALL_CMAKE_DIR cmake)
  endif ()
endif ()

if (DEFINED ADDITIONAL_CMAKE_PREFIX_PATH AND EXISTS "${ADDITIONAL_CMAKE_PREFIX_PATH}")
  set (CMAKE_PREFIX_PATH ${ADDITIONAL_CMAKE_PREFIX_PATH} ${CMAKE_PREFIX_PATH})
endif ()

#-----------------------------------------------------------------------------
# parse the full version number from jpeglib.h and include in JPEG_FULL_VERSION
#-----------------------------------------------------------------------------
file (READ ${JPEG_SRC_DIR}/jpeglib.h _jpeglib_h_contents)
string (REGEX REPLACE ".*#define[ \t]+JPEG_LIB_VERSION[ \t]+([0-9][0-9]*).*$"
    "\\1" JPEG_FULL_VERSION ${_jpeglib_h_contents})
string (REGEX REPLACE ".*#define[ \t]+JPEG_LIB_VERSION_MAJOR[ \t]+([0-9][0-9]*).*$"
    "\\1" JPEG_VERS_MAJOR ${_jpeglib_h_contents})
string (REGEX REPLACE ".*#define[ \t]+JPEG_LIB_VERSION_MINOR[ \t]+([0-9][0-9]*).*$"
    "\\1" JPEG_VERS_MINOR ${_jpeglib_h_contents})
#message (STATUS "VERSION: ${JPEG_FULL_VERSION}")

#set (JPEG_VERS_MAJOR 8)
#set (JPEG_VERS_MINOR 0)
set (JPEG_VERS_RELEASE 0)

#-----------------------------------------------------------------------------
set (JPEG_PACKAGE "jpeg")
set (JPEG_PACKAGE_NAME "jpeg")
set (JPEG_PACKAGE_VERSION "${JPEG_VERS_MAJOR}.${JPEG_VERS_MINOR}")
set (JPEG_PACKAGE_VERSION_MAJOR "${JPEG_VERS_MAJOR}.${JPEG_VERS_MINOR}")
set (JPEG_PACKAGE_VERSION_MINOR "${JPEG_VERS_RELEASE}")
set (JPEG_PACKAGE_STRING "${JPEG_PACKAGE_NAME} ${JPEG_PACKAGE_VERSION}")
set (JPEG_PACKAGE_TARNAME "${JPEG_PACKAGE_NAME}${HDF_PACKAGE_EXT}")
set (JPEG_PACKAGE_URL "http://www.hdfgroup.org")
set (JPEG_PACKAGE_BUGREPORT "help@hdfgroup.org")
set (JPEG_PACKAGE_SOVERSION "${JPEG_VERS_MAJOR}.${JPEG_VERS_MINOR}.${JPEG_VERS_RELEASE}")
set (JPEG_PACKAGE_SOVERSION_MAJOR "${JPEG_VERS_MAJOR}")

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
include (${JPEG_RESOURCES_DIR}/JPEGMacros.cmake)
SET_HDF_BUILD_TYPE()

#-----------------------------------------------------------------------------
# Setup output Directories
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
  if (WIN32)
    set (CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${HDF_CFG_NAME})
    set (CMAKE_PDB_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all pdb files."
    )
  else ()
    set (CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${HDF_CFG_NAME})
  endif ()
else ()
  # if we are externally configured, but the project uses old cmake scripts
  # this may not be set
  if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects
#-----------------------------------------------------------------------------
if (NOT JPEG_EXPORTED_TARGETS)
  set (JPEG_EXPORTED_TARGETS "jpeg-targets")
endif ()

#-----------------------------------------------------------------------------
# To include a library in the list exported by the project AT BUILD TIME,
# add it to this variable. This is NOT used by Make Install, but for projects
# which include JPEG as a sub-project within their build tree
#-----------------------------------------------------------------------------
set_global_variable (JPEG_LIBRARIES_TO_EXPORT "")

#-----------------------------------------------------------------------------
# Run all the CMake configuration tests for our build environment
#-----------------------------------------------------------------------------
include (${JPEG_RESOURCES_DIR}/ConfigureChecks.cmake)

#-----------------------------------------------------------------------------
# Mac OS X Options
#-----------------------------------------------------------------------------
if (JPEG_BUILD_FRAMEWORKS AND NOT BUILD_SHARED_LIBS)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries")
endif ()

#-----------------------------------------------------------------------------
# Option to Build Shared and Static libs, default is shared
#-----------------------------------------------------------------------------
option (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

#-----------------------------------------------------------------------------
# When building utility executables that generate other (source) files :
# we make use of the following variables defined in the root CMakeLists.
# Certain systems may add /Debug or /Release to output paths
# and we need to call the executable from inside the CMake configuration
#-----------------------------------------------------------------------------
set (EXE_EXT "")
if (WIN32)
  set (EXE_EXT ".exe")
  add_definitions (-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)
  add_definitions (-D_CRT_SECURE_NO_WARNINGS)
  add_definitions (-D_CONSOLE)
endif ()

if (MSVC)
  set (CMAKE_MFC_FLAG 0)
endif ()

set (MAKE_SYSTEM)
if (CMAKE_BUILD_TOOL MATCHES "make")
  set (MAKE_SYSTEM 1)
endif ()

set (CFG_INIT "/${CMAKE_CFG_INTDIR}")
if (MAKE_SYSTEM)
  set (CFG_INIT "")
endif ()

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -fomit-frame-pointer -finline-functions -fno-common")
endif ()
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_LOADED)
  set (CMAKE_CXX_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_CXX_FLAGS} -fomit-frame-pointer -finline-functions -fno-common")
endif ()

#-----------------------------------------------------------------------------
# This is in here to help some of the GCC based IDES like Eclipse
# and code blocks parse the compiler errors and warnings better.
#-----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif ()
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_LOADED)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif ()

#-----------------------------------------------------------------------------
set (MEMORYMGR jmemnobs)
option (ENABLE_MEMORYMGR "Select memory manager" OFF)
if (ENABLE_MEMORYMGR)
  set (MEMORYMGR jmemname)
endif ()

#-----------------------------------------------------------------------------
# Include user macros
#-----------------------------------------------------------------------------
include (UserMacros.cmake)

#-----------------------------------------------------------------------------
# All libs/tests/examples need the main include directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${JPEG_BINARY_DIR} ${JPEG_SOURCE_DIR}/src ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory (src)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
option (BUILD_TESTING "Build JPEG Unit Testing" OFF)
if (BUILD_TESTING)
  set (DART_TESTING_TIMEOUT 1200 CACHE STRING
       "Timeout in seconds for each test (default 1200=20minutes)")
  enable_testing ()
  include (CTest)

#-----------------------------------------------------------------------------
# Option to build examples
#-----------------------------------------------------------------------------
  option (JPEG_BUILD_EXAMPLES  "Build JPEG Library Examples" OFF)
  if (JPEG_BUILD_EXAMPLES)
    add_subdirectory (examples)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Generate the jconfig.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
configure_file (${JPEG_RESOURCES_DIR}/jconfig.h.in ${JPEG_BINARY_DIR}/jconfig.h @ONLY)

include (CMakePackageConfigHelpers)

#-----------------------------------------------------------------------------
# Check for Installation Utilities
#-----------------------------------------------------------------------------
if (WIN32)
  set (PF_ENV_EXT "(x86)")
  find_program (NSIS_EXECUTABLE NSIS.exe PATHS "$ENV{ProgramFiles}\\NSIS" "$ENV{ProgramFiles${PF_ENV_EXT}}\\NSIS")
  if(NOT CPACK_WIX_ROOT)
    file(TO_CMAKE_PATH "$ENV{WIX}" CPACK_WIX_ROOT)
  endif ()
  find_program (WIX_EXECUTABLE candle  PATHS "${CPACK_WIX_ROOT}/bin")
endif ()

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
if (NOT JPEG_INSTALL_NO_DEVELOPMENT)
  install (
      FILES ${PROJECT_BINARY_DIR}/jconfig.h
      DESTINATION ${JPEG_INSTALL_INCLUDE_DIR}
      COMPONENT headers
  )
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  install (
      EXPORT ${JPEG_EXPORTED_TARGETS}
      DESTINATION ${JPEG_INSTALL_CMAKE_DIR}/jpeg
      FILE ${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-targets.cmake
      NAMESPACE ${PACKAGE_NAMESPACE}
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  export (
      TARGETS ${JPEG_LIBRARIES_TO_EXPORT} ${JPEG_LIB_DEPENDENCIES}
      FILE ${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-targets.cmake
      NAMESPACE ${PACKAGE_NAMESPACE}
  )
  export (PACKAGE ${JPEG_PACKAGE}${JPEG_PACKAGE_EXT})
endif ()

#-----------------------------------------------------------------------------
# Set includes needed for build
#-----------------------------------------------------------------------------
set (JPEG_INCLUDES_BUILD_TIME
    ${JPEG_SRC_DIR} ${JPEG_BINARY_DIR}
)

#-----------------------------------------------------------------------------
# Set variables needed for installation
#-----------------------------------------------------------------------------
set (JPEG_VERSION_STRING ${JPEG_PACKAGE_VERSION})
set (JPEG_VERSION_MAJOR  ${JPEG_PACKAGE_VERSION_MAJOR})
set (JPEG_VERSION_MINOR  ${JPEG_PACKAGE_VERSION_MINOR})

#-----------------------------------------------------------------------------
# Configure the jpeg-config.cmake file for the build directory
#-----------------------------------------------------------------------------
set (INCLUDE_INSTALL_DIR ${JPEG_INSTALL_INCLUDE_DIR})
set (SHARE_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/${JPEG_INSTALL_CMAKE_DIR}" )
set (CURRENT_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
configure_package_config_file (
    ${JPEG_RESOURCES_DIR}/jpeg-config.cmake.in
    "${JPEG_BINARY_DIR}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config.cmake"
    INSTALL_DESTINATION "${JPEG_INSTALL_CMAKE_DIR}/jpeg"
    PATH_VARS INCLUDE_INSTALL_DIR SHARE_INSTALL_DIR CURRENT_BUILD_DIR
    INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

#-----------------------------------------------------------------------------
# Configure the jpeg-config.cmake file for the install directory
#-----------------------------------------------------------------------------
set (INCLUDE_INSTALL_DIR ${JPEG_INSTALL_INCLUDE_DIR})
set (SHARE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${JPEG_INSTALL_CMAKE_DIR}" )
set (CURRENT_BUILD_DIR "${CMAKE_INSTALL_PREFIX}" )
configure_package_config_file (
    ${JPEG_RESOURCES_DIR}/jpeg-config.cmake.in
    "${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config.cmake"
    INSTALL_DESTINATION "${JPEG_INSTALL_CMAKE_DIR}/jpeg"
    PATH_VARS INCLUDE_INSTALL_DIR SHARE_INSTALL_DIR CURRENT_BUILD_DIR
)
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  install (
      FILES ${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config.cmake
      DESTINATION ${JPEG_INSTALL_CMAKE_DIR}/jpeg
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Configure the jpeg-config-version.cmake file for the install directory
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  configure_file (
      ${JPEG_RESOURCES_DIR}/jpeg-config-version.cmake.in
      ${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config-version.cmake @ONLY
  )
  install (
      FILES ${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config-version.cmake
      DESTINATION ${JPEG_INSTALL_CMAKE_DIR}/jpeg
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Add Document File(s) to CMake Install
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  install (
      FILES
          ${JPEG_SOURCE_DIR}/install_docs/README
          ${JPEG_SOURCE_DIR}/install_docs/libjpeg.txt
          ${JPEG_SOURCE_DIR}/install_docs/install.txt
          ${JPEG_SOURCE_DIR}/RELEASE.txt
      DESTINATION ${JPEG_INSTALL_DATA_DIR}
      COMPONENT documents
  )
endif ()

#-----------------------------------------------------------------------------
# Check for Installation Utilities
#-----------------------------------------------------------------------------
if (WIN32)
  set (PF_ENV_EXT "(x86)")
  find_program (NSIS_EXECUTABLE NSIS.exe PATHS "$ENV{ProgramFiles}\\NSIS" "$ENV{ProgramFiles${PF_ENV_EXT}}\\NSIS")
  if(NOT CPACK_WIX_ROOT)
    file(TO_CMAKE_PATH "$ENV{WIX}" CPACK_WIX_ROOT)
  endif()
  find_program (WIX_EXECUTABLE candle  PATHS "${CPACK_WIX_ROOT}/bin")
endif ()

#-----------------------------------------------------------------------------
# Configure the README.txt file for the windows binary package
#-----------------------------------------------------------------------------
if (WIN32)
  configure_file (${JPEG_SOURCE_DIR}/install_docs/README ${JPEG_BINARY_DIR}/README.txt @ONLY)
endif ()

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  set (CPACK_PACKAGE_VENDOR "HDF_Group")
  set (CPACK_PACKAGE_NAME "${JPEG_PACKAGE_NAME}")
  if (CDASH_LOCAL)
    set (CPACK_PACKAGE_VERSION "${JPEG_PACKAGE_VERSION}")
  else ()
    set (CPACK_PACKAGE_VERSION "${JPEG_PACKAGE_VERSION_STRING}")
  endif ()
  set (CPACK_PACKAGE_VERSION_MAJOR "${JPEG_PACKAGE_VERSION_MAJOR}")
  set (CPACK_PACKAGE_VERSION_MINOR "${JPEG_PACKAGE_VERSION_MINOR}")
  set (CPACK_PACKAGE_VERSION_PATCH "")
  set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/install_docs/README")
  set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/RELEASE.txt")
  set (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/RELEASE.txt")
  set (CPACK_PACKAGE_RELOCATABLE TRUE)
  set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "jpeg9c Installation")
  set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}/${CPACK_PACKAGE_NAME}/${CPACK_PACKAGE_VERSION}")

  set (CPACK_GENERATOR "TGZ")
  if (WIN32)
    set (CPACK_GENERATOR "ZIP")

    if (NSIS_EXECUTABLE)
      list (APPEND CPACK_GENERATOR "NSIS")
    endif ()
    # Installers for 32- vs. 64-bit CMake:
    #  - Root install directory (displayed to end user at installer-run time)
    #  - "NSIS package/display name" (text used in the installer GUI)
    #  - Registry key used to store info about the installation
    set (CPACK_NSIS_PACKAGE_NAME "${JPEG_PACKAGE_STRING}")
    if (CMAKE_CL_64)
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} (Win64)")
    else ()
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    endif ()
    # set the install/unistall icon used for the installer itself
    # There is a bug in NSI that does not handle full unix paths properly.
    #set (CPACK_NSIS_MUI_ICON "${JPEG_RESOURCES_DIR}\\\\JPEG.ico")
    #set (CPACK_NSIS_MUI_UNIICON "${JPEG_RESOURCES_DIR}\\\\JPEG.ico")
    # set the package header icon for MUI
    #set (CPACK_PACKAGE_ICON "${JPEG_RESOURCES_DIR}\\\\JPEG.bmp")
    set (CPACK_NSIS_DISPLAY_NAME "${CPACK_NSIS_PACKAGE_NAME}")
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_VERSION}")
    set (CPACK_NSIS_CONTACT "${JPEG_PACKAGE_BUGREPORT}")
    set (CPACK_NSIS_MODIFY_PATH ON)
    set (CPACK_NSIS_PACKAGE_NAME "JPEG ${JPEG_PACKAGE_VERSION}")
    if (WIX_EXECUTABLE)
      list (APPEND CPACK_GENERATOR "WIX")
    endif ()
#WiX variables
    set (CPACK_WIX_UNINSTALL "1")
# .. variable:: CPACK_WIX_LICENSE_RTF
#  RTF License File
#
#  If CPACK_RESOURCE_FILE_LICENSE has an .rtf extension it is used as-is.
#
#  If CPACK_RESOURCE_FILE_LICENSE has an .txt extension it is implicitly
#  converted to RTF by the WiX Generator.
#  The expected encoding of the .txt file is UTF-8.
#
#  With CPACK_WIX_LICENSE_RTF you can override the license file used by the
#  WiX Generator in case CPACK_RESOURCE_FILE_LICENSE is in an unsupported
#  format or the .txt -> .rtf conversion does not work as expected.
    set (CPACK_RESOURCE_FILE_LICENSE "${JPEG_BINARY_DIR}/README.txt")
# .. variable:: CPACK_WIX_PRODUCT_ICON
#  The Icon shown next to the program name in Add/Remove programs.
#    set(CPACK_WIX_PRODUCT_ICON "${JPEG_RESOURCES_DIR}\\\\hdf.ico")
#
# .. variable:: CPACK_WIX_UI_BANNER
#
#  The bitmap will appear at the top of all installer pages other than the
#  welcome and completion dialogs.
#
#  If set, this image will replace the default banner image.
#
#  This image must be 493 by 58 pixels.
#
# .. variable:: CPACK_WIX_UI_DIALOG
#
#  Background bitmap used on the welcome and completion dialogs.
#
#  If this variable is set, the installer will replace the default dialog
#  image.
#
#  This image must be 493 by 312 pixels.
#
  elseif (APPLE)
    list (APPEND CPACK_GENERATOR "STGZ")
    list (APPEND CPACK_GENERATOR "DragNDrop")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    #set (CPACK_PACKAGE_ICON "${JPEG_RESOURCES_DIR}/hdf.icns")

    option (JPEG_PACK_MACOSX_FRAMEWORK  "Package the JPEG Library in a Framework" OFF)
    if (JPEG_PACK_MACOSX_FRAMEWORK AND JPEG_BUILD_FRAMEWORKS)
      list (APPEND CPACK_GENERATOR "Bundle")
      set (CPACK_BUNDLE_NAME "${JPEG_PACKAGE_STRING}")
      set (CPACK_BUNDLE_LOCATION "/")    # make sure CMAKE_INSTALL_PREFIX ends in /
      set (CMAKE_INSTALL_PREFIX "/${CPACK_BUNDLE_NAME}.framework/Versions/${CPACK_PACKAGE_VERSION}/${CPACK_PACKAGE_NAME}/")
      set (CPACK_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      #-----------------------------------------------------------------------------
      # Configure the Info.plist file for the install bundle
      #-----------------------------------------------------------------------------
      configure_file (
          ${JPEG_RESOURCES_DIR}/CPack.Info.plist.in
          ${JPEG_BINARY_DIR}/CMakeFiles/Info.plist @ONLY
      )
      configure_file (
          ${JPEG_RESOURCES_DIR}/PkgInfo.in
          ${JPEG_BINARY_DIR}/CMakeFiles/PkgInfo @ONLY
      )
      install (
          FILES ${JPEG_BINARY_DIR}/CMakeFiles/PkgInfo
          DESTINATION ..
      )
    endif ()
  else ()
    list (APPEND CPACK_GENERATOR "STGZ")
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

    set (CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
    set (CPACK_DEBIAN_PACKAGE_MAINTAINER "${JPEG_PACKAGE_BUGREPORT}")

#      list (APPEND CPACK_GENERATOR "RPM")
    set (CPACK_RPM_PACKAGE_RELEASE "1")
    set (CPACK_RPM_COMPONENT_INSTALL ON)
    set (CPACK_RPM_PACKAGE_RELOCATABLE ON)
  endif ()

  # By default, do not warn when built on machines using only VS Express:
  if (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    set (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
  endif ()
  include (InstallRequiredSystemLibraries)

  set (CPACK_INSTALL_CMAKE_PROJECTS "${JPEG_BINARY_DIR};JPEG;libraries;/")
  set (CPACK_INSTALL_CMAKE_PROJECTS "${JPEG_BINARY_DIR};JPEG;headers;/")
  set (CPACK_INSTALL_CMAKE_PROJECTS "${JPEG_BINARY_DIR};JPEG;configinstall;/")

  set (CPACK_ALL_INSTALL_TYPES Full Developer User)
  set (CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Everything")

  set(CPACK_COMPONENTS_ALL libraries headers documents configinstall)

  include (CPack)

  cpack_add_component_group(Runtime)

  cpack_add_component_group(Documents
      EXPANDED
      DESCRIPTION "Release notes for jpeg9c"
  )

  cpack_add_component_group(Development
      EXPANDED
      DESCRIPTION "All of the tools you'll need to develop applications"
  )

  cpack_add_component (libraries
      DISPLAY_NAME "JPEG Libraries"
      REQUIRED
      GROUP Runtime
      INSTALL_TYPES Full Developer User
  )
  cpack_add_component (headers
      DISPLAY_NAME "JPEG Headers"
      DEPENDS libraries
      GROUP Development
      INSTALL_TYPES Full Developer
  )
  cpack_add_component (documents
      DISPLAY_NAME "JPEG Documents"
      GROUP Documents
      INSTALL_TYPES Full Developer
  )
  cpack_add_component (configinstall
      DISPLAY_NAME "JPEG CMake files"
      DEPENDS libraries
      GROUP Development
      INSTALL_TYPES Full Developer User
  )

endif ()
