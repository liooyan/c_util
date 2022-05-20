cmake_minimum_required(VERSION 3.20)
project(util C)


set(CMAKE_C_STANDARD 99)

add_executable(main main.c util/execute_pool.c util/queue.c)
add_executable(queue_test test/queue_test.c util/queue.c)
add_executable(thead_test test/thead_test.c util/execute_pool.c util/queue.c)
add_executable(dict_test test/dict_test.c util/dict.c )
add_executable(murmur_hash test/murmur_hash_test.c util/murmur_hash.c )
add_executable(hash_dict_test test/hash_dict_test.c util/dict_type.c  util/dict.c util/murmur_hash.c )
add_executable(socker_test test/socker.c )
add_executable(click_test test/click.c )

add_executable(except_test test/except_test.c except/except.c )
add_executable(except_test2 test/except_test2.c  )




if(WIN32)
    target_link_libraries(socker_test wsock32 ws2_32)
    target_link_libraries(click_test wsock32 ws2_32)
endif()
