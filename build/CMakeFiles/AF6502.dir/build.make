# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.22.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.22.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Volumes/Federico/Programmi/C++/af6502/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Volumes/Federico/Programmi/C++/af6502/build

# Include any dependencies generated for this target.
include CMakeFiles/AF6502.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/AF6502.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/AF6502.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AF6502.dir/flags.make

CMakeFiles/AF6502.dir/main.cpp.o: CMakeFiles/AF6502.dir/flags.make
CMakeFiles/AF6502.dir/main.cpp.o: /Volumes/Federico/Programmi/C++/af6502/src/main.cpp
CMakeFiles/AF6502.dir/main.cpp.o: CMakeFiles/AF6502.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Federico/Programmi/C++/af6502/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AF6502.dir/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AF6502.dir/main.cpp.o -MF CMakeFiles/AF6502.dir/main.cpp.o.d -o CMakeFiles/AF6502.dir/main.cpp.o -c /Volumes/Federico/Programmi/C++/af6502/src/main.cpp

CMakeFiles/AF6502.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AF6502.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Federico/Programmi/C++/af6502/src/main.cpp > CMakeFiles/AF6502.dir/main.cpp.i

CMakeFiles/AF6502.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AF6502.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Federico/Programmi/C++/af6502/src/main.cpp -o CMakeFiles/AF6502.dir/main.cpp.s

CMakeFiles/AF6502.dir/memory.cpp.o: CMakeFiles/AF6502.dir/flags.make
CMakeFiles/AF6502.dir/memory.cpp.o: /Volumes/Federico/Programmi/C++/af6502/src/memory.cpp
CMakeFiles/AF6502.dir/memory.cpp.o: CMakeFiles/AF6502.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Federico/Programmi/C++/af6502/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/AF6502.dir/memory.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AF6502.dir/memory.cpp.o -MF CMakeFiles/AF6502.dir/memory.cpp.o.d -o CMakeFiles/AF6502.dir/memory.cpp.o -c /Volumes/Federico/Programmi/C++/af6502/src/memory.cpp

CMakeFiles/AF6502.dir/memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AF6502.dir/memory.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Federico/Programmi/C++/af6502/src/memory.cpp > CMakeFiles/AF6502.dir/memory.cpp.i

CMakeFiles/AF6502.dir/memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AF6502.dir/memory.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Federico/Programmi/C++/af6502/src/memory.cpp -o CMakeFiles/AF6502.dir/memory.cpp.s

CMakeFiles/AF6502.dir/CPU.cpp.o: CMakeFiles/AF6502.dir/flags.make
CMakeFiles/AF6502.dir/CPU.cpp.o: /Volumes/Federico/Programmi/C++/af6502/src/CPU.cpp
CMakeFiles/AF6502.dir/CPU.cpp.o: CMakeFiles/AF6502.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/Federico/Programmi/C++/af6502/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/AF6502.dir/CPU.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AF6502.dir/CPU.cpp.o -MF CMakeFiles/AF6502.dir/CPU.cpp.o.d -o CMakeFiles/AF6502.dir/CPU.cpp.o -c /Volumes/Federico/Programmi/C++/af6502/src/CPU.cpp

CMakeFiles/AF6502.dir/CPU.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AF6502.dir/CPU.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Volumes/Federico/Programmi/C++/af6502/src/CPU.cpp > CMakeFiles/AF6502.dir/CPU.cpp.i

CMakeFiles/AF6502.dir/CPU.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AF6502.dir/CPU.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Volumes/Federico/Programmi/C++/af6502/src/CPU.cpp -o CMakeFiles/AF6502.dir/CPU.cpp.s

# Object files for target AF6502
AF6502_OBJECTS = \
"CMakeFiles/AF6502.dir/main.cpp.o" \
"CMakeFiles/AF6502.dir/memory.cpp.o" \
"CMakeFiles/AF6502.dir/CPU.cpp.o"

# External object files for target AF6502
AF6502_EXTERNAL_OBJECTS =

libAF6502.a: CMakeFiles/AF6502.dir/main.cpp.o
libAF6502.a: CMakeFiles/AF6502.dir/memory.cpp.o
libAF6502.a: CMakeFiles/AF6502.dir/CPU.cpp.o
libAF6502.a: CMakeFiles/AF6502.dir/build.make
libAF6502.a: CMakeFiles/AF6502.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Volumes/Federico/Programmi/C++/af6502/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libAF6502.a"
	$(CMAKE_COMMAND) -P CMakeFiles/AF6502.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AF6502.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AF6502.dir/build: libAF6502.a
.PHONY : CMakeFiles/AF6502.dir/build

CMakeFiles/AF6502.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AF6502.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AF6502.dir/clean

CMakeFiles/AF6502.dir/depend:
	cd /Volumes/Federico/Programmi/C++/af6502/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/Federico/Programmi/C++/af6502/src /Volumes/Federico/Programmi/C++/af6502/src /Volumes/Federico/Programmi/C++/af6502/build /Volumes/Federico/Programmi/C++/af6502/build /Volumes/Federico/Programmi/C++/af6502/build/CMakeFiles/AF6502.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AF6502.dir/depend

