# Packages
find_package(GTest REQUIRED)

# Test executable
add_executable(${PROJECT_NAME}_test)

# Test sources
target_sources(${PROJECT_NAME}_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/dense_unitary_matrix.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/error_models.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/integration_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/operands_helper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/quantum_state.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/random.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/register_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sparse_array.cpp"
)

target_compile_features(${PROJECT_NAME}_test PRIVATE
    cxx_std_23
)

# Target options
target_link_libraries(${PROJECT_NAME}_test
    PRIVATE qx
    PRIVATE GTest::gmock
    PRIVATE GTest::gtest
)
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
    )
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        -Wall -Wextra -Werror -Wfatal-errors
        -Wno-error=unused-but-set-variable
        -Wno-error=unused-function
        -Wno-error=unused-local-typedef
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        /MP /EHsc /bigobj
    )
else()
    message(SEND_ERROR "Unknown compiler!")
endif()

gtest_discover_tests(${PROJECT_NAME}_test
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
