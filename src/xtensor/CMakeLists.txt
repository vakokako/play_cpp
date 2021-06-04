# find_package(QT COMPONENTS Widgets REQUIRED)

find_package(xtl CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)

add_executable(tensor tensor.cpp)
# set_target_properties( GuiQt PROPERTIES CMAKE_AUTOMOC ON )
target_link_libraries(
    tensor
  project_warnings
  project_options
  xtl xtensor::use_xsimd)
target_compile_features(tensor PUBLIC cxx_nullptr cxx_lambdas)
