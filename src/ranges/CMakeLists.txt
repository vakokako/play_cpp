# find_package(QT COMPONENTS Widgets REQUIRED)

find_package(range-v3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(ranges ranges.cpp)
# set_target_properties( GuiQt PROPERTIES CMAKE_AUTOMOC ON )
target_link_libraries(
    ranges
  project_warnings
  project_options
  range-v3
  fmt::fmt
  )
target_compile_features(ranges PUBLIC cxx_nullptr cxx_lambdas)
