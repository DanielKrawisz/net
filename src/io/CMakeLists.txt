# everything except crypto and net.
add_library (io STATIC)
add_library (Io::io ALIAS io)

set_target_properties (io PROPERTIES
  OUTPUT_NAME "io_io"
)

target_sources (
  io

  PRIVATE

  wait_for_enter.cpp
  log.cpp
  arg_parser.cpp
  random.cpp
  #run.cpp
)

target_compile_features (io PUBLIC cxx_std_23)
set_target_properties (io PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  io

  PUBLIC

  Data::async
  Data::string
  Boost::log_setup
  Boost::log
)

target_include_directories (
  io
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

configure_file (
    ${PROJECT_SOURCE_DIR}/include/io/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/io/version.hpp
)

get_target_property (OUT Io::io LINK_LIBRARIES)

include (GNUInstallDirs)

# copy include directory into install destination
install (
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
  TARGETS io
  EXPORT IoTargets
  ARCHIVE
)

install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/io/version.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/io
)

install (
  EXPORT IoTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/io
  FILE ioTargets.cmake
  NAMESPACE Io::
)

include (CMakePackageConfigHelpers)
include (GNUInstallDirs)

# generate the config file that includes the exports
configure_package_config_file (${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/ioConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/dataFunctions"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file (
  "${CMAKE_CURRENT_BINARY_DIR}/ioConfigVersion.cmake"
  COMPATIBILITY AnyNewerVersion
)

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/ioConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/ioConfigVersion.cmake
  DESTINATION lib/cmake/io
)

set (CPACK_PACKAGE_VENDOR "Daniel Krawisz")
set (CPACK_PACKAGE_CONTACT "email@example.com")
set (CPACK_PACKAGE_DESCRIPTION "WhatsOnChain API")
include (CPack)
