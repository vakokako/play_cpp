# find_package(QT COMPONENTS Widgets REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} ${Qt_REQUIRED_VERSION} COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
add_executable(GuiQt GuiQt.cpp)
# set_target_properties( GuiQt PROPERTIES CMAKE_AUTOMOC ON )
target_link_libraries(
  GuiQt
  project_warnings
  project_options
  Qt${QT_VERSION_MAJOR}::Widgets)
target_compile_features(GuiQt PUBLIC cxx_nullptr cxx_lambdas)
