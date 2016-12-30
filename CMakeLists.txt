#
# project: lesser-panda-bgfx
#

cmake_minimum_required(VERSION 2.8)

# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

fips_setup()

fips_project(lesser-panda-bgfx)

# if (FIPS_MACOS)
#   include_directories(../fips-bgfx/bx/include/compat/osx)
# elseif (FIPS_IOS)
#   include_directories(../fips-bgfx/bx/include/compat/ios)
# elseif (FIPS_PNACL)
#   include_directories(../fips-bgfx/bx/include/compat/nacl)
# elseif (FIPS_WINDOWS)
#   include_directories(../fips-bgfx/bx/include/compat/msvc)
# endif()

fips_add_subdirectory(src)

fips_finish()
