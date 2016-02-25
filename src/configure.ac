AC_INIT([qx-simulator],[0.1.3],[nader.khammassi@gmail.com],[qx-simulator])
AC_PREREQ([2.69])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
# author: Nader Khammassi
# bugreport to : nader.khammassi@gmail.com

AC_CONFIG_MACRO_DIR([m4]) # autotools suggestion
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(pthread, pthread_join)
AC_CHECK_LIB(pthread, pthread_exit)
AC_CHECK_LIB(pthread, pthread_barrierattr_init)
AC_CHECK_LIB(pthread, pthread_barrier_init)
AC_CHECK_LIB(pthread, pthread_mutex_init)
AC_CHECK_LIB(pthread, pthread_mutex_destroy)
AC_CHECK_LIB(pthread, pthread_mutex_lock)
AC_CHECK_LIB(pthread, pthread_mutex_unlock)
AC_CHECK_LIB(pthread, pthread_cond_init)
AC_CHECK_LIB(pthread, pthread_cond_destroy)
AC_CHECK_LIB(pthread, pthread_cond_destroy)
AC_CHECK_LIB(pthread, pthread_cond_broadcast)
AC_CHECK_LIB(pthread, pthread_cond_wait)
AC_CHECK_LIB(pthread, pthread_cond_signal)

AC_CHECK_FUNCS([pow]) 
AC_CHECK_FUNCS([sqrt])

AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([mach/mach.h])
AC_CHECK_HEADERS([stdint.h]) 
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([unistd.h])

AC_CHECK_HEADER_STDBOOL 
AC_C_INLINE 
AC_FUNC_ERROR_AT_LINE
AC_TYPE_INT32_T 
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AC_SUBST([QX_VERSION], [0:1:3])
AC_SUBST([QCODE_VERSION], [0.1.0])

AC_CONFIG_FILES([Makefile qx.pc:qx.pc.in])

AC_PROG_CC

AC_OUTPUT

