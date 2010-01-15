PROJECT(xizero)

#set minimum cmake version
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE "ON")
SET(CMAKE_VERBOSE_MAKEFILE "ON")


#----------------------------------------------------------------------------------------------------
# General Settings
#----------------------------------------------------------------------------------------------------
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".lib" ".a")



#----------------------------------------------------------------------------------------------------
# Options, that can be changed be the User in order to customise xizero
#----------------------------------------------------------------------------------------------------


SET( EXTRA_FLAGS
	"-fsingle-precision-constant -ffast-math -fomit-frame-pointer -funroll-loops -floop-optimize -funit-at-a-time -Wextra -Wall -Weffc++" CACHE STRING
	"" )

FIND_PACKAGE(OpenAL REQUIRED)
#very weird here, need to both set the path AND include with full path or else cmake errors out...
SET(CMAKE_MODULE_PATH ${xizero_SOURCE_DIR}/cmake)
include(${xizero_SOURCE_DIR}/cmake/FindALUT.cmake)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLU REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
LIST(APPEND xizero_libraries ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUT_LIBRARIES} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} )
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR} ${ALUT_INCLUDE_DIR} )
#----------------------------------------------------------------------------------------------------
# Source listing
#----------------------------------------------------------------------------------------------------

# We define the include paths here, our minimal source dir is one

INCLUDE_DIRECTORIES(${xizero_SOURCE_DIR}  )

SET(xizeroSrc
	src/xizero.cc
)

SET(unittestSrc
	src/unittest.cc
)

#add all header to project so they show up in qtcreator filelist
FILE( GLOB_RECURSE header  "*.h?" )

#----------------------------------------------------------------------------------------------------
# Build target defintions
#----------------------------------------------------------------------------------------------------

# Here we define the executable xizero ( or on Windows xizero.exe )

#auto-remove whitespaces before/after lib paths
cmake_policy(SET CMP0004 OLD)
#ignore warnings about macosx app bundle output dir
cmake_policy(SET CMP0006 OLD)

ADD_DEFINITIONS( ${freetypeCFLAGS} "-g" )
ADD_DEFINITIONS( ${EXTRA_FLAGS} )

ADD_EXECUTABLE(xizero ${xizeroSrc} ${header} )
ADD_EXECUTABLE(unittest ${unittestSrc} ${header} )

TARGET_LINK_LIBRARIES(xizero ${xizero_libraries}  )
TARGET_LINK_LIBRARIES(unittest ${xizero_libraries} )

add_custom_target(dat ${xizero_SOURCE_DIR}/dat/makexizerodat.sh )

add_dependencies( xizero dat )
add_dependencies( unittest dat )
