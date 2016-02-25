## libxpu: shared library

## place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} 

## define a libtool archive target "libexample-@XPU_API_VERSION@.la", with
## @XPU_API_VERSION@ substituted into the generated Makefile at configure
## time.
## The libtool archive file (.la) will be installed into the directory named
## by the predefined variable $(bindir), along with the actual shared library
## file (.so).
lib_LTLIBRARIES = libxpu-@XPU_API_VERSION@.la

## define the source file list for the "libxpu-@XPU_API_VERSION@.la"
## target.  note that @XPU_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## the file extension .cc is recognized by Automake, and makes it produce
## rules which invoke the C++ compiler to produce a libtool object file (.lo)
## from each source file.  Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.
libxpu_@XPU_API_VERSION@_la_SOURCES = xpu.cc 

## instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libxpu_@XPU_API_VERSION@_la_LDFLAGS = -version-info $(XPU_SO_VERSION)

## define the list of public header files and their install location.  the
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  this only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
xpu_includedir = $(includedir)/xpu-$(XPU_API_VERSION)
nobase_xpu_include_HEADERS = xpu.h \
						   xpu/types.h \
						   xpu/dynamic_callback.h \
						   xpu/sequential_tasks.h \
						   xpu/timer.h \
						   xpu/parallel_for.h \
						   xpu/exception.h \
						   xpu/lockable.h \
						   xpu/utils.h \
						   xpu/pointers.h \
						   xpu/exception.cc \
						   xpu/pipeline.h \
						   xpu/stdout.h \
						   xpu/core/lockable_factory.h \
						   xpu/core/queue.h \
						   xpu/core/event_notifier.h \
						   xpu/core/generic_worker.h \
						   xpu/core/work.h \
						   xpu/core/event_relay.h \
						   xpu/core/list.h \
						   xpu/core/lockable_factory.cc \
						   xpu/core/object_pool.h \
						   xpu/core/os/spinlock.h \
						   xpu/core/os/barrier.h \
						   xpu/core/os/mutex.h \
						   xpu/core/os/event.h \
						   xpu/core/os/mutex.cc \
						   xpu/core/os/thread.cc \
						   xpu/core/os/thread.h \
						   xpu/core/os/cpuset.h \
						   xpu/core/os/spinlock.cc \
						   xpu/core/os/barrier.cc \
						   xpu/core/os/control.h \
						   xpu/core/event_listener.h \
						   xpu/vec3f.h \
						   xpu/vec4f.h \
						   xpu/processor.h \
						   xpu/range.h \
						   xpu/pointers.cc \
						   xpu/task_group.h \
						   xpu/task.h \
						   xpu/xxmmintrin.h \
						   xpu/static_callback.h \
						   xpu/parallel_tasks.h 

## The generated configuration header is installed in its own subdirectory of
## $(libdir).  The reason for this is that the configuration information put
## into this header file describes the target platform the installed library
## has been built for.  Thus the file must not be installed into a location
## intended for architecture-independent files, as defined by the Filesystem
## Hierarchy Standard (FHS).
## The nodist_ prefix instructs Automake to not generate rules for including
## the listed files in the distribution on 'make dist'.  Files that are listed
## in _HEADERS variables are normally included in the distribution, but the
## configuration header file is generated at configure time and should not be
## shipped with the source tarball.
xpu_libincludedir = $(libdir)/xpu-$(XPU_API_VERSION)/include
nodist_xpu_libinclude_HEADERS = xpu-config.h

## unit tests
XPU_BUILT_TESTS = tests/hybrid tests/parallel tests/init-clean tests/pipeline \
                  tests/parallel_for tests/shared_memory_detection

			    
TESTS = $(XPU_BUILT_TESTS) 
check_PROGRAMS = $(XPU_BUILT_TESTS)

tests_init_clean_SOURCES = tests/init-clean-test.cc
tests_init_clean_LDADD = libxpu-@XPU_API_VERSION@.la
tests_init_clean_LDFLAGS = -lpthread

tests_hybrid_SOURCES = tests/hybrid-test.cc
tests_hybrid_LDADD = libxpu-@XPU_API_VERSION@.la
tests_hybrid_LDFLAGS = -lpthread

tests_parallel_SOURCES = tests/parallel-test.cc
tests_parallel_LDADD = libxpu-@XPU_API_VERSION@.la
tests_parallel_LDFLAGS = -lpthread

tests_parallel_for_SOURCES = tests/parallel_for-test.cc
tests_parallel_for_LDADD = libxpu-@XPU_API_VERSION@.la
tests_parallel_for_LDFLAGS = -lpthread

tests_pipeline_SOURCES = tests/pipeline-test.cc
tests_pipeline_LDADD = libxpu-@XPU_API_VERSION@.la
tests_pipeline_LDFLAGS = -lpthread

#tests_pipeline_image_processing_SOURCES = tests/pipeline-image-processing.cc
#tests_pipeline_image_processing_LDADD = libxpu-@XPU_API_VERSION@.la
#tests_pipeline_image_processing_LDFLAGS = -lpthread -lrt

tests_shared_memory_detection_SOURCES = tests/shared-memory-detection-test.cc
tests_shared_memory_detection_LDADD = libxpu-@XPU_API_VERSION@.la
tests_shared_memory_detection_LDFLAGS = -lpthread

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xpu-$(XPU_API_VERSION).pc

## Define an independent executable script for inclusion in the distribution
## archive.  However, it will not be installed on an end user's system due to
## the noinst_ prefix.
dist_noinst_SCRIPTS = autogen.sh
