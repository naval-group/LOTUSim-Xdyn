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
        SET(GRPC_GRPCPP_UNSECURE
            ${GRPC_ROOT_DIR_LIB}/libgrpc++_unsecure.a
            ${GRPC_ROOT_DIR_LIB}/libgrpc.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_strings.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_strings_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_str_format_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_int128.a
            # ${GRPC_ROOT_DIR_LIB}/libabsl_flags.a
            # ${GRPC_ROOT_DIR_LIB}/libabsl_flags_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_raw_logging_internal.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_throw_delegate.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_bad_optional_access.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_time.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_time_zone.a
            ${GRPC_ROOT_DIR_LIB}/libabsl_base.a
            ${GRPC_ROOT_DIR_LIB}/libcares.a
            ${GRPC_ROOT_DIR_LIB}/libssl.a
            ${GRPC_ROOT_DIR_LIB}/libcrypto.a
            ${GRPC_ROOT_DIR_LIB}/libgpr.a
            ${GRPC_ROOT_DIR_LIB}/libupb.a
            ${GRPC_ROOT_DIR_LIB}/libaddress_sorting.a
            z
            m
            dl)
        # SET(GRPC_CPP_PLUGIN_EXECUTABLE /usr/bin/grpc_cpp_plugin)
        # SET(GRPC_INCLUDE_DIR /usr/include)
        SET(PROTOBUF_PROTOC /opt/grpc/bin/protoc)
        SET(GRPC_CPP_PLUGIN_EXECUTABLE /opt/grpc/bin/grpc_cpp_plugin)
        SET(GRPC_INCLUDE_DIR /opt/grpc/include)
    ENDIF()
ENDIF()
