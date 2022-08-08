add_executable(e2_threadpool
                Main.cpp)

target_include_directories(e2_threadpool
                            PRIVATE ${E2_THREADPOOL_INCLUDE_DIRS})

# compile options
target_compile_options(e2_threadpool PUBLIC ${COMPILE_OPTIONS})

# compile definitions
target_compile_definitions(e2_threadpool PUBLIC ${COMPILE_DEFINITIONS})
