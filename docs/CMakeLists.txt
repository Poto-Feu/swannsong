if(UNIX)
    # Install man pages if the system is UNIX-like
    add_custom_target(man ALL)

    set(MAN_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/swannsongadv.6")
    set(COMP_MAN_FILE_NAME "swannsongadv.6.gz")
    set(MAN_DIR "share/man/man6/")
    add_custom_command(TARGET man POST_BUILD
        COMMAND gzip -c
        ${MAN_FILE_NAME}
        > ${CMAKE_CURRENT_BINARY_DIR}/${COMP_MAN_FILE_NAME})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${COMP_MAN_FILE_NAME}
        DESTINATION ${MAN_DIR}
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
endif()
