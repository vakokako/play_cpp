# find_package(QT COMPONENTS Widgets REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} ${Qt_REQUIRED_VERSION} COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
add_executable(GuiAlgorithm GuiAlgorithm.cpp)
# set_target_properties( GuiAlgorithm PROPERTIES CMAKE_AUTOMOC ON )
target_link_libraries(
  GuiAlgorithm
  project_warnings
  project_options
  Qt${QT_VERSION_MAJOR}::Widgets)
target_compile_features(GuiAlgorithm PUBLIC cxx_nullptr cxx_lambdas)
