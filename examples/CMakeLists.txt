cmake_minimum_required (VERSION 3.2.2)
PROJECT (JPEG_EXAMPLES)

#-----------------------------------------------------------------------------
# Add Test for cjpeg
#-----------------------------------------------------------------------------
add_executable (cjpeg cjpeg.c rdppm.c rdgif.c rdtarga.c rdrle.c rdbmp.c rdswitch.c cdjpeg.c)
target_include_directories(cjpeg PRIVATE "${JPEG_SOURCE_DIR}")
TARGET_C_PROPERTIES (cjpeg STATIC)
target_link_libraries (cjpeg PRIVATE ${JPEG_LIB_TARGET})

#-- Adding test for djpeg
add_executable (djpeg djpeg.c wrppm.c wrgif.c wrtarga.c wrrle.c wrbmp.c rdcolmap.c cdjpeg.c)
target_include_directories(djpeg PRIVATE "${JPEG_SOURCE_DIR}")
TARGET_C_PROPERTIES (djpeg STATIC)
target_link_libraries (djpeg PRIVATE ${JPEG_LIB_TARGET})

#-- Adding test for jpegtran
add_executable (jpegtran jpegtran.c rdswitch.c cdjpeg.c transupp.c)
target_include_directories(jpegtran PRIVATE "${JPEG_SOURCE_DIR}")
TARGET_C_PROPERTIES (jpegtran STATIC)
target_link_libraries (jpegtran PRIVATE ${JPEG_LIB_TARGET})

#-- Adding test for rdjpgcom
add_executable (rdjpgcom rdjpgcom.c)
target_include_directories(rdjpgcom PRIVATE "${JPEG_SOURCE_DIR}")
TARGET_C_PROPERTIES (rdjpgcom STATIC)
target_link_libraries (rdjpgcom  PRIVATE ${JPEG_LIB_TARGET})

#add_test (NAME rdjpgcom COMMAND $<TARGET_FILE:rdjpgcom>)

#-- Adding test for wrjpgcom
add_executable (wrjpgcom wrjpgcom.c)
target_include_directories(wrjpgcom PRIVATE "${JPEG_SOURCE_DIR}")
TARGET_C_PROPERTIES (wrjpgcom STATIC)
target_link_libraries (wrjpgcom PRIVATE ${JPEG_LIB_TARGET})

#add_test (NAME wrjpgcom COMMAND $<TARGET_FILE:wrjpgcom>)

#---------------------------------------------------------
if (BUILD_TESTING)

  # Test support files
  set (TEST_FILES
      testorig.jpg
      testimg.ppm
      testimg.bmp
      testimg.jpg
      testprog.jpg
      testimgp.jpg
  )

  foreach (test_file ${TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${test_file}")
    #MESSAGE (STATUS " Copying ${test_file}")
    add_custom_command (
        TARGET     cjpeg
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${JPEG_TEST_DIR}/${test_file} ${dest}
    )
  endforeach ()

  add_test (
      NAME djpeg-ppm
      COMMAND $<TARGET_FILE:djpeg> -dct int -ppm -outfile testout.ppm  testorig.jpg
  )

  add_test (
      NAME djpeg-bmp
      COMMAND $<TARGET_FILE:djpeg> -dct int -bmp -colors 256 -outfile testout.bmp  testorig.jpg
  )

  add_test (
      NAME cjpeg-dct
      COMMAND $<TARGET_FILE:cjpeg> -dct int -outfile testout.jpg  testimg.ppm
  )

  add_test (
      NAME djpeg-ppmp
      COMMAND $<TARGET_FILE:djpeg> -dct int -ppm -outfile testoutp.ppm testprog.jpg
  )

  add_test (
      NAME cjpeg-prog
      COMMAND $<TARGET_FILE:cjpeg> -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
  )

  add_test (
      NAME jpegtran
      COMMAND $<TARGET_FILE:jpegtran> -outfile testoutt.jpg testprog.jpg
  )

  # now compare the output with the reference
  add_test (
      NAME djpeg-ppm-cmp
      COMMAND ${CMAKE_COMMAND} -E compare_files testimg.ppm testout.ppm
  )
  add_test (
      NAME djpeg-bmp-cmp
      COMMAND ${CMAKE_COMMAND} -E compare_files testimg.bmp testout.bmp
  )
  add_test (
      NAME cjpeg-dct-cmp
      COMMAND ${CMAKE_COMMAND} -E compare_files testimg.jpg testout.jpg
  )
  add_test (
      NAME djpeg-ppmp-cmp
      COMMAND ${CMAKE_COMMAND} -E compare_files testimg.ppm testoutp.ppm
  )
  add_test (
      NAME cjpeg-prog-cmp
      COMMAND ${CMAKE_COMMAND} -E compare_files testimgp.jpg testoutp.jpg
  )
  add_test (
      NAME jpegtran-cmp
      COMMAND ${CMAKE_COMMAND} -E compare_files testorig.jpg testoutt.jpg
  )

endif ()
