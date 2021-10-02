add_subdirectory(cpp)
add_subdirectory(gui_qt)
add_subdirectory(gui_algorithm)
add_subdirectory(ranges)
add_subdirectory(xtensor)
# add_subdirectory(fill_voids)
add_subdirectory(polymorphism)
add_subdirectory(play_cpp)

add_executable(intro main.cpp)
target_link_libraries(
  intro
  PRIVATE project_options
          project_warnings
          docopt_s
          fmt::fmt
          spdlog::spdlog)