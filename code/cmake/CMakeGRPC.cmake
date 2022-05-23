IF(MINGW)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x600)
    SET(GRPC_GRPCPP_UNSECURE
        /opt/GRPC/lib/libgrpc++_unsecure.a
        /opt/GRPC/lib/libgrpc.a
        /opt/GRPC/lib/libgpr.a
        /opt/GRPC/lib/libaddress_sorting.a
        /opt/CAres/lib/libcares.a
        /usr/src/mxe/usr/x86_64-w64-mingw32.static.posix/lib/libssl.a
        /usr/src/mxe/usr/x86_64-w64-mingw32.static.posix/lib/libcrypto.a
        /usr/src/mxe/usr/x86_64-w64-mingw32.static.posix/lib/libz.a)
    SET(GRPC_CPP_PLUGIN_EXECUTABLE /opt/GRPC/bin/grpc_cpp_plugin.exe)
    SET(GRPC_INCLUDE_DIR /opt/GRPC/include)
ELSE()
    IF(CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
        MESSAGE(STATUS "Using gRPC library coming from distribution")
        SET(GRPC_GRPCPP_UNSECURE
            /usr/lib/x86_64-linux-gnu/libgrpc++_unsecure.a
            /usr/lib/x86_64-linux-gnu/libgrpc.a
            /usr/lib/x86_64-linux-gnu/libcares.a
            /usr/lib/x86_64-linux-gnu/libssl.a
            /usr/lib/x86_64-linux-gnu/libcrypto.a
            z
            dl)
        SET(GRPC_CPP_PLUGIN_EXECUTABLE /usr/bin/grpc_cpp_plugin)
        SET(GRPC_INCLUDE_DIR /usr/include)
    ELSE()
        MESSAGE(STATUS "Using gRPC library compiled with -fPIC flag")
        # SET(GRPC_ROOT_DIR_LIB /usr/lib/x86_64-linux-gnu)
        SET(GRPC_ROOT_DIR_LIB /opt/grpc/lib)
        # Declare the list of static gRPC files needed for version 1.32 and 1.46
        # Only file found in path will be used.
        #
        # All this needs to be refactored, to use CMake FindPackage to looked
        # for static files compiled with -fPIC option to create shared library...
        SET(GRPC_GRPCPP_UNSECURE_TO_SEARCH
            ${GRPC_ROOT_DIR_LIB}/libgrpc++_unsecure.a
            ${GRPC_ROOT_DIR_LIB}/libgrpc.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_strings.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_strings_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_str_format_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_int128.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_raw_logging_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_throw_delegate.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_bad_optional_access.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_time.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_time_zone.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_synchronization.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_spinlock_wait.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_randen_hwaes.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_randen_hwaes_impl.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_randen.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_pool_urbg.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_seed_gen_exception.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_randen_slow.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_platform.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_random_internal_seed_material.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_time.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_status.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_cord.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_cord_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_cordz_functions.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_cordz_handle.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_cordz_info.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_statusor.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_bad_variant_access.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_hash.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_malloc_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_stacktrace.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_symbolize.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_demangle_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_debugging_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_low_level_hash.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_exponential_biased.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_city.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_base.a
            ${GRPC_ROOT_DIR_LIB}/libre2.a
            ${GRPC_ROOT_DIR_LIB}/libcares.a
            ${GRPC_ROOT_DIR_LIB}/libssl.a
            ${GRPC_ROOT_DIR_LIB}/libcrypto.a
            ${GRPC_ROOT_DIR_LIB}/libgpr.a
            ${GRPC_ROOT_DIR_LIB}/libupb.a
            ${GRPC_ROOT_DIR_LIB}/libaddress_sorting.a)
        SET(GRPC_GRPCPP_UNSECURE)
        FOREACH(GRPC_FILE ${GRPC_GRPCPP_UNSECURE_TO_SEARCH})
            IF(EXISTS ${GRPC_FILE})
                LIST(APPEND GRPC_GRPCPP_UNSECURE ${GRPC_FILE})
            ENDIF()
        ENDFOREACH()
        LIST(APPEND GRPC_GRPCPP_UNSECURE z)
        LIST(APPEND GRPC_GRPCPP_UNSECURE m)
        LIST(APPEND GRPC_GRPCPP_UNSECURE dl)
        # SET(GRPC_CPP_PLUGIN_EXECUTABLE /usr/bin/grpc_cpp_plugin)
        # SET(GRPC_INCLUDE_DIR /usr/include)
        SET(PROTOBUF_PROTOC /opt/grpc/bin/protoc)
        SET(GRPC_CPP_PLUGIN_EXECUTABLE /opt/grpc/bin/grpc_cpp_plugin)
        SET(GRPC_INCLUDE_DIR /opt/grpc/include)
    ENDIF()
ENDIF()
