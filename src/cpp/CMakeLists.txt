# string(CONCAT DISABLED_CHECKS "-llvmlibc-implementation-in-namespace,"
#                               "-llvmlibc-restrict-system-libc-headers,"
#                               "-llvmlibc-callee-namespace")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,${DISABLED_CHECKS};")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter='.*(?<!exclude\\.h)$$'")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-line-filter=[{\"name\":\"clang_tidy_exclude.h\",\"lines\":[[9999999,9999999]]},{\"name\":\".h\"},{\"name\":\".cpp\"}]")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=modernize-use-trailing-return-type;-header-filter='.*'")

# -line-filter=[{"name":"coco.cpp","lines":[[9999999,9999999]]},{"name":".h"},{"name":".cpp"}]

find_package(xtl CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(Boost COMPONENTS  REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

list(APPEND LIBRARIES xtl xtensor::use_xsimd)

add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE ${LIBRARIES})


add_executable(cpp20 cpp20.cpp)
target_compile_features(cpp20 PRIVATE cxx_std_20)
target_link_libraries(cpp20 PRIVATE ${LIBRARIES})

add_executable(benchmark benchmark.cpp)
target_compile_features(benchmark PRIVATE cxx_std_17)
target_link_libraries(benchmark PRIVATE ${LIBRARIES} benchmark::benchmark)

list(APPEND TESTS test_layout tuple_return_test struct_return_test)
foreach(TESTNAME ${TESTS})
    add_executable(${TESTNAME} ${TESTNAME}.cpp)
    target_link_libraries(${TESTNAME} PRIVATE ${LIBRARIES} GTest::gtest GTest::gtest_main)
endforeach()

add_executable(valeri valeri.cpp)