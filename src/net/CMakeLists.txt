
# networking
add_library (net STATIC)

set_target_properties (net PROPERTIES
  OUTPUT_NAME "net"
)

target_include_directories (
  net

  INTERFACE

  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_sources (
  net

  PRIVATE

  JSON.cpp

  URL.cpp

  beast/http.cpp
  HTTP.cpp
  REST.cpp
  TCP.cpp
  #websocket.cpp
  HTTP_client.cpp
  HTTP_server.cpp

  email.cpp
)

target_compile_features (net PUBLIC cxx_std_23)
set_target_properties (net PROPERTIES CXX_EXTENSIONS ON)

target_link_libraries (
  net

  PUBLIC

  Data::numbers
  Data::async
  Data::rate_limiter
  nlohmann_json::nlohmann_json
  taocpp::pegtl
  OpenSSL::SSL
  OpenSSL::Crypto
)

configure_file (
    ${PROJECT_SOURCE_DIR}/include/net/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/net/version.hpp
)

get_target_property (OUT net LINK_LIBRARIES)

include (GNUInstallDirs)

# copy include directory into install destination
install (
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
  TARGETS net
  EXPORT NetTargets
  ARCHIVE
)

install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/net/version.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/net
)

install (
  EXPORT NetTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/net
  FILE netTargets.cmake
  NAMESPACE Net::
)

include (CMakePackageConfigHelpers)
include(GNUInstallDirs)

# generate the config file that includes the exports
configure_package_config_file (${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/netConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/dataFunctions"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file (
  "${CMAKE_CURRENT_BINARY_DIR}/netConfigVersion.cmake"
  COMPATIBILITY AnyNewerVersion
)

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/netConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/netConfigVersion.cmake
  DESTINATION lib/cmake/net
)

set (CPACK_PACKAGE_VENDOR "Daniel Krawisz")
set (CPACK_PACKAGE_CONTACT "email@example.com")
set (CPACK_PACKAGE_DESCRIPTION "net API")
include (CPack)
