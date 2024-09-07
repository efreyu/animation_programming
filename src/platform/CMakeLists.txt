set(MODULE_SOURCE)

# scan only the current directory
file(GLOB MODULE_SOURCE
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.[hc]
        ${CMAKE_CURRENT_SOURCE_DIR}/*.[hc]pp)

if (USE_VULKAN)
    file(GLOB_RECURSE PLATFORM_SOURCE
            CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/*.[hc]
            ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/*.[hc]pp)
    set(MODULE_SOURCE ${MODULE_SOURCE} ${PLATFORM_SOURCE})
else()
    file(GLOB_RECURSE PLATFORM_SOURCE
            CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.[hc]
            ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.[hc]pp)
    set(MODULE_SOURCE ${MODULE_SOURCE} ${PLATFORM_SOURCE})
endif()
MakeReadableListOfSources(MODULE_SOURCE)

set(GAME_SOURCE ${GAME_SOURCE} ${MODULE_SOURCE} PARENT_SCOPE)
