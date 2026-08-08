add_library(qr_code_gen qr_code_gen.cpp)
add_library(qr_code_gen::qr_code_gen ALIAS qr_code_gen)

target_compile_features(qr_code_gen PUBLIC cxx_std_17)

target_include_directories(qr_code_gen
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(qr_code_gen PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS qr_code_gen
    EXPORT qr_code_genTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT qr_code_genTargets
    FILE qr_code_genTargets.cmake
    NAMESPACE qr_code_gen::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qr_code_gen
)