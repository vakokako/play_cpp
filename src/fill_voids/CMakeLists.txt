message(WARNING "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_package(xtl CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_package(BDABaseFunctions CONFIG REQUIRED)

add_executable(fill_voids fill_voids.hpp fill_voids.cpp libdivide.h)
target_link_libraries(
    fill_voids
    project_warnings
    project_options
    xtl xtensor::ust_xsimd
    BDA::BDABaseFunctions)