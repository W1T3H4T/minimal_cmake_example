cmake_minimum_required(VERSION 3.16) # setting this is required
project(example_project VERSION 1.1.0) # this sets the project name

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()

###############################################################################
## file globbing ##############################################################
###############################################################################

# Keep source lists explicit so adding/removing files is deterministic.
set(app_sources
  src/main/main.cpp
)

set(lib_sources
  src/main/frobnication.cpp
)

set(lib_headers
  src/main/frobnication.h
  src/main/fs.h
)

set(test_sources
  src/test/test.cpp
)

file(GLOB_RECURSE data CONFIGURE_DEPENDS resources/*)

###############################################################################
## target definitions #########################################################
###############################################################################

add_library(example_lib ${lib_sources} ${lib_headers})

# The data is added to the executable so it appears in IDE project trees.
add_executable(example ${app_sources} ${data})

target_compile_features(example_lib PUBLIC cxx_std_14)
target_compile_options(example_lib PUBLIC -Wall -Wfloat-conversion)

# This allows to include files relative to src/main with a <> pair.
target_include_directories(example_lib PUBLIC src/main)

target_link_libraries(example PRIVATE example_lib)
target_compile_features(example PRIVATE cxx_std_14)

# Copy resources next to the built executable so relative paths work.
add_custom_command(TARGET example POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/resources
  $<TARGET_FILE_DIR:example>/resources
)

###############################################################################
## dependencies ###############################################################
###############################################################################

# This defines the variables Boost_LIBRARIES that containts all library names
# that we need to link into the program.
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.36.0 COMPONENTS filesystem system QUIET)

if(Boost_FOUND)
  target_link_libraries(example PRIVATE
    Boost::filesystem
    Boost::system
    # here you can add any library dependencies
  )
  target_compile_definitions(example PRIVATE EXAMPLE_USE_BOOST_FILESYSTEM=1)
else()
  message(WARNING "Boost filesystem/system not found. Falling back to std::filesystem.")
  target_compile_features(example PRIVATE cxx_std_17)
  target_compile_definitions(example PRIVATE EXAMPLE_USE_BOOST_FILESYSTEM=0)
endif()

###############################################################################
## testing ####################################################################
###############################################################################

# This is for our testing framework,
# we don't add REQUIRED because it's just for testing.
# People who might want to build the project to use it should not be required
# to install testing dependencies.
include(CTest)
find_package(GTest)

if(GTest_FOUND)
  add_executable(unit_tests ${test_sources})

  if(TARGET GTest::gtest_main)
    target_link_libraries(unit_tests PRIVATE GTest::gtest_main example_lib)
  else()
    target_link_libraries(unit_tests PRIVATE ${GTEST_BOTH_LIBRARIES} example_lib)
    target_include_directories(unit_tests PRIVATE ${GTEST_INCLUDE_DIRS})
  endif()

  add_test(NAME unit_tests COMMAND unit_tests)
endif()

###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
include(GNUInstallDirs)

install(TARGETS example DESTINATION ${CMAKE_INSTALL_BINDIR})

# This is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package.
install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_BINDIR})

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME "MyExample")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
