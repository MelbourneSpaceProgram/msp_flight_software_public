# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake.exe

# The command to remove a file.
RM = /usr/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build

# Include any dependencies generated for this target.
include example/CMakeFiles/allocate_module_test.dir/depend.make

# Include the progress variables for this target.
include example/CMakeFiles/allocate_module_test.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/allocate_module_test.dir/flags.make

example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o: example/CMakeFiles/allocate_module_test.dir/flags.make
example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o: ../example/allocate_module.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) -DUNIT_TESTING=1 $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allocate_module_test.dir/allocate_module.c.o   -c /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example/allocate_module.c

example/CMakeFiles/allocate_module_test.dir/allocate_module.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allocate_module_test.dir/allocate_module.c.i"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) -DUNIT_TESTING=1 $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example/allocate_module.c > CMakeFiles/allocate_module_test.dir/allocate_module.c.i

example/CMakeFiles/allocate_module_test.dir/allocate_module.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allocate_module_test.dir/allocate_module.c.s"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) -DUNIT_TESTING=1 $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example/allocate_module.c -o CMakeFiles/allocate_module_test.dir/allocate_module.c.s

example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.requires:

.PHONY : example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.requires

example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.provides: example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.requires
	$(MAKE) -f example/CMakeFiles/allocate_module_test.dir/build.make example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.provides.build
.PHONY : example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.provides

example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.provides.build: example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o


example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o: example/CMakeFiles/allocate_module_test.dir/flags.make
example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o: ../example/allocate_module_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o   -c /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example/allocate_module_test.c

example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/allocate_module_test.dir/allocate_module_test.c.i"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example/allocate_module_test.c > CMakeFiles/allocate_module_test.dir/allocate_module_test.c.i

example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/allocate_module_test.dir/allocate_module_test.c.s"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example/allocate_module_test.c -o CMakeFiles/allocate_module_test.dir/allocate_module_test.c.s

example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.requires:

.PHONY : example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.requires

example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.provides: example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.requires
	$(MAKE) -f example/CMakeFiles/allocate_module_test.dir/build.make example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.provides.build
.PHONY : example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.provides

example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.provides.build: example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o


# Object files for target allocate_module_test
allocate_module_test_OBJECTS = \
"CMakeFiles/allocate_module_test.dir/allocate_module.c.o" \
"CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o"

# External object files for target allocate_module_test
allocate_module_test_EXTERNAL_OBJECTS =

example/allocate_module_test.exe: example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o
example/allocate_module_test.exe: example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o
example/allocate_module_test.exe: example/CMakeFiles/allocate_module_test.dir/build.make
example/allocate_module_test.exe: src/libcmocka.dll.a
example/allocate_module_test.exe: /usr/lib/librt.a
example/allocate_module_test.exe: example/CMakeFiles/allocate_module_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable allocate_module_test.exe"
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/allocate_module_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/allocate_module_test.dir/build: example/allocate_module_test.exe

.PHONY : example/CMakeFiles/allocate_module_test.dir/build

example/CMakeFiles/allocate_module_test.dir/requires: example/CMakeFiles/allocate_module_test.dir/allocate_module.c.o.requires
example/CMakeFiles/allocate_module_test.dir/requires: example/CMakeFiles/allocate_module_test.dir/allocate_module_test.c.o.requires

.PHONY : example/CMakeFiles/allocate_module_test.dir/requires

example/CMakeFiles/allocate_module_test.dir/clean:
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example && $(CMAKE_COMMAND) -P CMakeFiles/allocate_module_test.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/allocate_module_test.dir/clean

example/CMakeFiles/allocate_module_test.dir/depend:
	cd /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/example /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example /cygdrive/c/Users/Anthony/OwnCloud/UMSP/SVN/CDH_software/test/cmocka/build/example/CMakeFiles/allocate_module_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/allocate_module_test.dir/depend

