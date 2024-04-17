# Packages
find_package(GTest REQUIRED)

# Test executable
add_executable(${PROJECT_NAME}_test)

# Test sources
target_sources(${PROJECT_NAME}_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BitsetTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DenseUnitaryMatrixTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ErrorModelsTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/IntegrationTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/QuantumStateTest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SparseArrayTest.cpp"
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
        -Wno-error=restrict
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
