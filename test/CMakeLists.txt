target_sources(todotest 
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DatabaseTest.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DateTest.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DatabaseCommandTest.cpp"
)
