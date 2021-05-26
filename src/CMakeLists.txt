add_subdirectory(gui_qt)

add_executable(intro main.cpp)
target_link_libraries(
  intro
  PRIVATE project_options
          project_warnings
          docopt_s
          fmt::fmt
          spdlog::spdlog)