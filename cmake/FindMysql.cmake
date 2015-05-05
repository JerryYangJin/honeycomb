#  LIBMYSQL_INCLUDE_DIR - where to find mysql.h, etc.
#  LIBMYSQL_LIBRARY     - List of libraries when using libmysql.
#  LIBMYSQL_FOUND       - True if mysql found.

#  Read environment variables to find mysql include/lib directories
SET(LIBMYSQL_INCLUDE_DIR $ENV{LIBMYSQL_INCLUDE_DIR})
MESSAGE( STATUS "LIBMYSQL_INCLUDE_DIR:         " ${LIBMYSQL_INCLUDE_DIR} )
SET(LIBMYSQL_LIBRARY $ENV{LIBMYSQL_LIBRARY})
MESSAGE( STATUS "LIBMYSQL_LIBRARY:         " ${LIBMYSQL_LIBRARY} )

FIND_PATH(LIBMYSQL_INCLUDE_DIR mysql.h)

FIND_LIBRARY(LIBMYSQL_LIBRARY mysqlclient)

# handle the QUIETLY and REQUIRED arguments and set Libmysql_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBMYSQL DEFAULT_MSG LIBMYSQL_LIBRARY LIBMYSQL_INCLUDE_DIR)

SET(LIBMYSQL_VERSION 0)

IF(LIBMYSQL_FOUND)
  if (EXISTS "${LIBMYSQL_INCLUDE_DIR}/mysql_version.h")
    FILE(READ "${LIBMYSQL_INCLUDE_DIR}/mysql_version.h" _MYSQL_VERSION_CONENTS)
    STRING(REGEX REPLACE ".*#define MYSQL_SERVER_VERSION\\s+\"([0-9.]+)\".*" "\\1" LIBMYSQL_VERSION "${_MYSQL_VERSION_CONENTS}")
  endif()
ENDIF()

SET(LIBMYSQL_VERSION ${LIBMYSQL_VERSION} CACHE STRING "Version number of libmysql")
MESSAGE( STATUS "LIBMYSQL_VERSION:         " ${LIBMYSQL_VERSION} )

MARK_AS_ADVANCED(LIBMYSQL_LIBRARY LIBMYSQL_INCLUDE_DIR LIBMYSQL_VERSION)
