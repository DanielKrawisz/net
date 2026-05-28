
add_library (data_async STATIC)
add_library (data_rate_limiter STATIC)

add_library (Data::async ALIAS data_async)
add_library (Data::rate_limiter ALIAS data_rate_limiter)

set_target_properties (data_async PROPERTIES
  OUTPUT_NAME "data_async"
)

set_target_properties (data_rate_limiter PROPERTIES
  OUTPUT_NAME "data_rate_limiter"
)

target_sources (
  data_async

  PRIVATE

  async.cpp
)

target_sources (
  data_rate_limiter

  PRIVATE

  rate_limiter.cpp
)

target_include_directories (
  data_async
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories (
  data_rate_limiter
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features (data_async PUBLIC cxx_std_23)
set_target_properties (data_async PROPERTIES CXX_EXTENSIONS OFF)

target_compile_features (data_rate_limiter PUBLIC cxx_std_23)
set_target_properties (data_rate_limiter PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries (
  data_async

  PUBLIC

  Data::core
)

target_link_libraries (
  data_rate_limiter

  PUBLIC

  Data::core
)

include (GNUInstallDirs)

# copy include directory into install destination
install (
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
  TARGETS data_async data_rate_limiter
  EXPORT DataTargets
  ARCHIVE
)

install (
  EXPORT DataTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/net
  FILE dataNetTargets.cmake
  NAMESPACE Data::
)
