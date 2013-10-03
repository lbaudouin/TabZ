################################################################################
#
# PROJECT CONFIGURATION
#
################################################################################

PROJECT(TabZ)
cmake_minimum_required(VERSION 2.8)
SET( CMAKE_VERBOSE_MAKEFILE 0 )
SET( CMAKE_BUILD_TYPE Release )
SET( CMAKE_COLOR_MAKEFILE ON )
SET( CMAKE_CXX_FLAGS "-O3 -pipe -march=native -Wall -Wno-unused-function" )
SET( MAJOR_VERSION 0 )
SET( MINOR_VERSION 1 )
SET( PATCH_VERSION 1 )
SET( TabZ_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}" )

SET( HEADER_PATH ${CMAKE_SOURCE_DIR})
SET( SOURCE_PATH  ${CMAKE_SOURCE_DIR} )
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( INSTALL_BIN_PATH ${CMAKE_INSTALL_PREFIX}/bin )
SET( INSTALL_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/tabz )
SET( INSTALL_ICONS_PATH ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor )
SET( INSTALL_SHARE_PATH ${CMAKE_INSTALL_PREFIX}/share/ )

CONFIGURE_FILE( ${SOURCE_PATH}/ressource.qrc.in ${SOURCE_PATH}/ressource.qrc )

################################################################################
#
# EXTERNAL LIBS
#
################################################################################
FIND_PACKAGE( Qt4 4.8 REQUIRED )
SET(QT_USE_QTXML true)
SET(QT_USE_QTNETWORK true )
INCLUDE( ${QT_USE_FILE})
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
LINK_DIRECTORIES( ${QT_LIBRARIES} )

################################################################################
#
# BUILDING TARGET
#
################################################################################

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES( ${HEADER_PATH} )
INCLUDE_DIRECTORIES( ${HEADER_PATH}/qtsingleapplication/ )

LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/qtsingleapplication/ )

# binary configuration

SET( TabZ_SRC
	${SOURCE_PATH}/mainwindow.cpp
	${SOURCE_PATH}/tab.cpp
	${SOURCE_PATH}/highlighter.cpp
	${SOURCE_PATH}/guitar.cpp
	${SOURCE_PATH}/xta.cpp
	${SOURCE_PATH}/options.cpp
	${SOURCE_PATH}/optionsform.cpp
	${SOURCE_PATH}/chords.cpp
	${SOURCE_PATH}/httpupdate.cpp
	${SOURCE_PATH}/qprintpreview.cpp
	${SOURCE_PATH}/downloadxta.cpp
	${SOURCE_PATH}/chordsmanager.cpp
	${SOURCE_PATH}/readwritexml.cpp
	${SOURCE_PATH}/recent.cpp
	${SOURCE_PATH}/lilypond.cpp
	${SOURCE_PATH}/mytextedit.cpp
	${SOURCE_PATH}/mycompleter.cpp
	${SOURCE_PATH}/findreplacedialog.cpp
	${SOURCE_PATH}/epubgenerator.cpp
	${SOURCE_PATH}/qtsingleapplication/qtsinglecoreapplication.cpp
	${SOURCE_PATH}/qtsingleapplication/qtsingleapplication.cpp
	#${SOURCE_PATH}/qtsingleapplication/qtlockedfile_win.cpp
	#${SOURCE_PATH}/qtsingleapplication/qtlockedfile_unix.cpp
	#${SOURCE_PATH}/qtsingleapplication/qtlockedfile.cpp
	${SOURCE_PATH}/qtsingleapplication/qtlocalpeer.cpp
        ${SOURCE_PATH}/qwidgets/mycheckabletree.cpp
)

SET( TabZ_MOCS
	${HEADER_PATH}/mainwindow.h
	${HEADER_PATH}/tab.h
	${HEADER_PATH}/highlighter.h
	${HEADER_PATH}/guitar.h
	${HEADER_PATH}/xta.h
	${HEADER_PATH}/xta_info.h
	${HEADER_PATH}/options.h
	${HEADER_PATH}/optionsform.h
	${HEADER_PATH}/chords.h
	${HEADER_PATH}/httpupdate.h
	${HEADER_PATH}/qprintpreview.h
	${HEADER_PATH}/qprogressbardialog.h
	${HEADER_PATH}/downloadxta.h
	${HEADER_PATH}/mycheckbox.h
	${HEADER_PATH}/mytabwidget.h
	${HEADER_PATH}/chordsmanager.h
	${HEADER_PATH}/tools.h
	${HEADER_PATH}/colorregexpform.h
	${HEADER_PATH}/readwritexml.h
	${HEADER_PATH}/colorregexp.h
	${HEADER_PATH}/recent.h
	${HEADER_PATH}/lilypond.h
	${HEADER_PATH}/openeditlayout.h
	${HEADER_PATH}/mytextedit.h
	${HEADER_PATH}/fontlabellayout.h
	${HEADER_PATH}/mycompleter.h
	${HEADER_PATH}/findreplacedialog.h
	${HEADER_PATH}/epubgenerator.h
	${HEADER_PATH}/qtsingleapplication/qtsinglecoreapplication.h
	${SOURCE_PATH}/qtsingleapplication/qtsingleapplication.h
	${HEADER_PATH}/qtsingleapplication/qtlocalpeer.h
        ${HEADER_PATH}/qwidgets/mycheckabletree.h
)

SET ( TabZ_UIS
	${SOURCE_PATH}/mainwindow.ui
)

SET ( TabZ_RCC
	${SOURCE_PATH}/ressource.qrc
)

set ( TabZ_TRS
	${SOURCE_PATH}/TabZ_fr.ts
)

QT4_CREATE_TRANSLATION( APP_TRS ${TabZ_TRS})
QT4_WRAP_UI( APP_UIS ${TabZ_UIS})
QT4_ADD_RESOURCES( APP_RCC ${TabZ_RCC} )
QT4_WRAP_CPP( APP_MOCS ${TabZ_MOCS})

# Make and link
ADD_EXECUTABLE( tabz ${SOURCE_PATH}/main.cpp ${SOURCE_PATH}/mainwindow.cpp ${APP_TRS} ${TabZ_SRC} ${APP_UIS} ${APP_RCC} ${APP_MOCS})

TARGET_LINK_LIBRARIES( tabz ${QT_LIBRARIES} )

#SET_TARGET_PROPERTIES( tabz PROPERTIES
#  DEBUG_POSTFIX "_d"
#  VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}
#  SOVERSION ${MAJOR_VERSION}.${MINOR_VERSION_VERSION}
#)


################################################################################
#
# INSTALL PROCEDURE
#
################################################################################
INSTALL( TARGETS tabz DESTINATION ${INSTALL_BIN_PATH} )
INSTALL( FILES ${SOURCE_PATH}/data/icon16/TabZ.png DESTINATION ${INSTALL_ICONS_PATH}/16x16/apps/ )
INSTALL( FILES ${SOURCE_PATH}/data/icon32/TabZ.png DESTINATION ${INSTALL_ICONS_PATH}/32x32/apps/ )
INSTALL( FILES ${SOURCE_PATH}/data/icon64/TabZ.png DESTINATION ${INSTALL_ICONS_PATH}/64x64/apps/ )
INSTALL( FILES ${SOURCE_PATH}/data/tabz DESTINATION ${INSTALL_SHARE_PATH}/menu/ )
INSTALL( FILES ${SOURCE_PATH}/data/tabz.desktop DESTINATION ${INSTALL_SHARE_PATH}/applications/ )

################################################################################
#
# Build Deb
#
################################################################################
SET( CPACK_GENERATOR "DEB" )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Read/Modify tabs" )
SET( CPACK_DEBIAN_PACKAGE_MAINTAINER  "Léo Baudouin" )
SET( CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION} )
SET( CPACK_PACKAGE_VERSION_MINOR ${MINOR_VERSION} )
#SET( CPACK_COMPONENTS_ALL libraries headers cmakemodule )
SET( CPACK_DEBIAN_PACKAGE_DEPENDS "libqtcore4, libqtgui4, libqt4-xml" )
SET( CPACK_PACKAGE_NAME "tabz" )
set( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${PATCH_VERSION}" )
find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
if(DPKG_PROGRAM)
  execute_process(
	COMMAND ${DPKG_PROGRAM} --print-architecture
	OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
	OUTPUT_STRIP_TRAILING_WHITESPACE )
else(DPKG_PROGRAM)
  set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_NAME} )
endif(DPKG_PROGRAM)
SET( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" )

INCLUDE(CPack)



################################################################################
#
# UNINSTALL PROCEDURE
#
################################################################################
#CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" 
#		"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#		IMMEDIATE @ONLY )

#add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
