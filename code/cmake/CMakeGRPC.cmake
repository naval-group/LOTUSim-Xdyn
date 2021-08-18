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
        SET(GRPC_GRPCPP_UNSECURE
            /usr/lib/x86_64-linux-gnu/libgrpc++_unsecure.a
            /usr/lib/x86_64-linux-gnu/libgrpc.a
            /usr/lib/x86_64-linux-gnu/libcares.a
            /usr/lib/x86_64-linux-gnu/libssl.a
            /usr/lib/x86_64-linux-gnu/libcrypto.a
            z
            dl)
    ELSE()
        SET(GRPC_GRPCPP_UNSECURE
            /usr/lib/x86_64-linux-gnu/libgrpc++_unsecure.a
            /usr/lib/x86_64-linux-gnu/libgrpc.a
            /usr/lib/x86_64-linux-gnu/libabsl_strings.a
            /usr/lib/x86_64-linux-gnu/libabsl_strings_internal.a
            /usr/lib/x86_64-linux-gnu/libabsl_str_format_internal.a
            /usr/lib/x86_64-linux-gnu/libabsl_int128.a
            /usr/lib/x86_64-linux-gnu/libabsl_flags.a
            /usr/lib/x86_64-linux-gnu/libabsl_flags_internal.a
            /usr/lib/x86_64-linux-gnu/libabsl_raw_logging_internal.a
            /usr/lib/x86_64-linux-gnu/libabsl_throw_delegate.a
            /usr/lib/x86_64-linux-gnu/libabsl_bad_optional_access.a
            /usr/lib/x86_64-linux-gnu/libabsl_time.a
            /usr/lib/x86_64-linux-gnu/libabsl_time_zone.a
            /usr/lib/x86_64-linux-gnu/libabsl_base.a
            /usr/lib/x86_64-linux-gnu/libcares.a
            /usr/lib/x86_64-linux-gnu/libssl.a
            /usr/lib/x86_64-linux-gnu/libcrypto.a
            z
            m
            dl)
    ENDIF()
    SET(GRPC_CPP_PLUGIN_EXECUTABLE /usr/bin/grpc_cpp_plugin)
    SET(GRPC_INCLUDE_DIR /usr/include)
ENDIF()
