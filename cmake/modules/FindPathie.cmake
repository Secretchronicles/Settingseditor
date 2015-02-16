# This file is part of the TSC Settingseditor.
# Copyright © 2015 Marvin Gülker
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
