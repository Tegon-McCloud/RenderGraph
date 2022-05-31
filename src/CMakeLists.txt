
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC ${Vulkan_LIBRARIES}
)

