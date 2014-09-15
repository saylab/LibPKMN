########################################################################
# Toolchain file for cross building with MinGW for Windows
# Usage:  cmake -DCMAKE_TOOLCHAIN_FILE=(SOURCE DIR)/cmake/Toolchains/mingw_cross.cmake \
# -DCMAKE_INSTALL_PREFIX=./install ../
########################################################################
set( CMAKE_SYSTEM_NAME Windows )

#
# WIN_ARCH must be i686 or x86_64
#
if(NOT DEFINED WIN_ARCH)
    set(WIN_ARCH "i686")
endif(NOT DEFINED WIN_ARCH)

set( CMAKE_C_COMPILER ${WIN_ARCH}-w64-mingw32-gcc )
set( CMAKE_CXX_COMPILER ${WIN_ARCH}-w64-mingw32-g++ )
set( CMAKE_RC_COMPILER ${WIN_ARCH}-w64-mingw32-windres )

set( CMAKE_FIND_ROOT_PATH /usr/${WIN_ARCH}-w64-mingw32
                          /usr/${WIN_ARCH}-w64-mingw32/local )
