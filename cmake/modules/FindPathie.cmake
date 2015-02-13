# Try to find the Pathie library.
# This will define:
#
# Pathie_FOUND       - Pathie library is available
# Pathie_INCULDE_DIR - Where the pathie.hpp header file is
# Pathie_LIBRARIES   - The libraries to link in.

find_path(Pathie_INCLUDE_DIR pathie.hpp PATH_SUFFIXES pathie)
find_library(Pathie_LIBRARY NAMES pathie pathie_cpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pathie DEFAULT_MSG Pathie_INCLUDE_DIR Pathie_LIBRARY)
set(Pathie_LIBRARIES ${Pathie_LIBRARY})
mark_as_advanced(Pathie_INCLUDE_DIR Pathie_LIBRARIES Pathie_LIBRARY)
