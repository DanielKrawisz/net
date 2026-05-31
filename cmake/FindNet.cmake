find_package (PkgConfig)
pkg_check_modules (PC_NET QUIET Net)

find_path(NET_INCLUDE_DIR
NAMES net.hpp
HINTS ENV NET_INC_DIR
      ENV NET_DIR
      $ENV{DATA_DIR}/include
PATH_SUFFIXES include/net
  DOC "The directory containing the Net header files"
)

find_library (NET_LIBRARY NAMES data
  HINTS ENV NET_LIB_DIR
  ENV NET_DIR
  $ENV{DATA_DIR}/lib
  PATH_SUFFIXES lib/data
  DOC "Path to the Net library"
)

message (STATUS "${NET_INCLUDE_DIR}")
if (NET_INCLUDE_DIR)
    file (READ "${NET_INCLUDE_DIR}/net/version.hpp" ver)
    string (REGEX MATCH "#define DATA_VERSION \"([0-9*.]*)\"" _ ${ver})
    set (NET_VERSION ${CMAKE_MATCH_1})
endif ()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Net
  FOUND_VAR NET_FOUND
  REQUIRED_VARS
    NET_LIBRARY
    NET_INCLUDE_DIR
  VERSION_VAR NET_VERSION
)

if (NET_FOUND)
  set (NET_LIBRARIES ${NET_LIBRARY})
  set (NET_INCLUDE_DIRS ${NET_INCLUDE_DIR})
endif ()

if (NET_FOUND AND NOT TARGET Net::Net)
  add_library (Net::Net UNKNOWN IMPORTED)
  set_target_properties (Net::Net PROPERTIES
    IMPORTED_LOCATION "${NET_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${NET_INCLUDE_DIR}"
  )
endif ()

mark_as_advanced (
  NET_INCLUDE_DIR
  NET_LIBRARY
)
