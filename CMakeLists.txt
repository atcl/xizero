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
# Load needed Modules
#----------------------------------------------------------------------------------------------------
# PKG-Config
FIND_PACKAGE( PkgConfig )


#----------------------------------------------------------------------------------------------------
# Options, that can be changed be the User in order to customise xizero
#----------------------------------------------------------------------------------------------------
SET( FLTK_CONFIG
	/usr/bin/fltk-config CACHE STRING
	"" )

SET( AUDIERE_CONFIG
	/usr/bin/audiere-config CACHE STRING
	"" )

SET( FREETYPE_CONFIG
	/usr/bin/freetype-config CACHE STRING
	"" )
	
EXECUTE_PROCESS(
	COMMAND ${FREETYPE_CONFIG} "--cflags"
	OUTPUT_VARIABLE freetypeCFLAGS
	ERROR_VARIABLE shell_error
)

EXECUTE_PROCESS(
	COMMAND ${FLTK_CONFIG} "--ldstaticflags" 
	OUTPUT_VARIABLE fltkLDFLAGS
	ERROR_VARIABLE shell_error
)

EXECUTE_PROCESS(
	COMMAND ${AUDIERE_CONFIG} "--libs"
	OUTPUT_VARIABLE audiereLDFLAGS
	ERROR_VARIABLE shell_error
)

#----------------------------------------------------------------------------------------------------
# Source listing
#----------------------------------------------------------------------------------------------------

# We define the include paths here, our minimal source dir is one

INCLUDE_DIRECTORIES(${xizero_SOURCE_DIR} )

SET(xizeroSrc
	src/xizero.cc
)

SET(testxSrc
	src/testx.cc
)

SET(unittestSrc
	src/unittest.cc
)

#----------------------------------------------------------------------------------------------------
# Build target defintions
#----------------------------------------------------------------------------------------------------

# Here we define the executable xizero ( or on Windows xizero.exe )

#auto-remove whitespaces before/after lib paths
cmake_policy(SET CMP0004 OLD)
#ignore warnings about macosx app bundle output dir
cmake_policy(SET CMP0006 OLD)

ADD_DEFINITIONS( ${freetypeCFLAGS} "-g" )

ADD_EXECUTABLE(xizero ${xizeroSrc} )
#ADD_EXECUTABLE(testx ${testxSrc} )
ADD_EXECUTABLE(unittest ${unittestSrc} )



# Here the wxWidgets Libraries are added. These are set for us by the FIND Script. If you need other Libraries, you can add them here as well.
TARGET_LINK_LIBRARIES(xizero ${audiereLDFLAGS} ${fltkLDFLAGS})
#TARGET_LINK_LIBRARIES(testx ${audiereLDFLAGS} ${fltkLDFLAGS})
TARGET_LINK_LIBRARIES(unittest ${audiereLDFLAGS} ${fltkLDFLAGS})
