target_sources(todolib 
    PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/Database.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Date.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DatabaseCommand.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DatabaseCommandVisitor.cpp"
)
