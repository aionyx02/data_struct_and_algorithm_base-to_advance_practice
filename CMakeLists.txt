cmake_minimum_required(VERSION 3.25)

project(data_structures_practice VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(judge_core
    src/catalog.cpp
    src/judge_service.cpp
    src/stress_service.cpp
)

if(WIN32)
    target_sources(judge_core PRIVATE src/process_runner_windows.cpp)
else()
    target_sources(judge_core PRIVATE src/process_runner_unsupported.cpp)
endif()

target_include_directories(judge_core PUBLIC include)
target_compile_features(judge_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(judge_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(judge_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(algo src/main.cpp)
target_link_libraries(algo PRIVATE judge_core)
if(WIN32)
    target_link_libraries(algo PRIVATE Shell32)
endif()
target_compile_definitions(algo PRIVATE
    DATA_STRUCTURES_PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

include(CTest)

if(BUILD_TESTING AND WIN32)
    function(add_catalog_test name expected_id)
        add_test(
            NAME ${name}
            COMMAND algo list --root "${CMAKE_SOURCE_DIR}"
        )
        set_tests_properties(${name} PROPERTIES
            PASS_REGULAR_EXPRESSION "${expected_id}"
        )
    endfunction()

    function(add_cli_judge_test
        name
        problem_id
        source_file
        expected_exit
        expected_text
    )
        add_test(
            NAME ${name}
            COMMAND ${CMAKE_COMMAND}
                "-DALGO=$<TARGET_FILE:algo>"
                "-DPROJECT_ROOT=${CMAKE_SOURCE_DIR}"
                "-DPROBLEM_ID=${problem_id}"
                "-DSOURCE_FILE=${CMAKE_SOURCE_DIR}/${source_file}"
                "-DEXPECTED_EXIT=${expected_exit}"
                "-DEXPECTED_TEXT=${expected_text}"
                -P "${CMAKE_SOURCE_DIR}/tests/run_cli_test.cmake"
        )
    endfunction()

    function(add_cli_stress_test
        name
        problem_id
        source_file
        expected_exit
        expected_text
        expect_failure_context
    )
        add_test(
            NAME ${name}
            COMMAND ${CMAKE_COMMAND}
                "-DALGO=$<TARGET_FILE:algo>"
                "-DPROJECT_ROOT=${CMAKE_SOURCE_DIR}"
                "-DPROBLEM_ID=${problem_id}"
                "-DSOURCE_FILE=${CMAKE_SOURCE_DIR}/${source_file}"
                "-DSEED=424242"
                "-DCASE_COUNT=4"
                "-DEXPECTED_EXIT=${expected_exit}"
                "-DEXPECTED_TEXT=${expected_text}"
                "-DEXPECT_FAILURE_CONTEXT=${expect_failure_context}"
                -P "${CMAKE_SOURCE_DIR}/tests/run_stress_test.cmake"
        )
    endfunction()

    add_catalog_test(catalog_lists_f01 F01-array-adt)
    add_catalog_test(catalog_lists_f02 F02-dynamic-array)
    add_catalog_test(catalog_lists_f03 F03-stack-array)
    add_catalog_test(catalog_lists_f04 F04-circular-queue)
    add_catalog_test(catalog_lists_f05 F05-array-deque)
    add_catalog_test(catalog_lists_f06 F06-singly-linked-list)
    add_catalog_test(catalog_lists_f07 F07-linked-stack)
    add_catalog_test(catalog_lists_f08 F08-linked-queue)
    add_catalog_test(catalog_lists_f09 F09-doubly-linked-list)
    add_catalog_test(catalog_lists_f10 F10-circular-linked-list)

    add_cli_judge_test(
        judge_accepts_array_adt
        F01-array-adt
        tests/fixtures/submissions/array_adt_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_dynamic_array
        F02-dynamic-array
        tests/fixtures/submissions/dynamic_array_correct.cpp
        0
        "Verdict: AC"
    )

    add_cli_judge_test(
        judge_accepts_correct
        F03-stack-array
        tests/fixtures/submissions/stack_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_unicode_space_path
        F03-stack-array
        "tests/fixtures/路徑 with spaces/stack correct.cpp"
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_wrong_answer
        F03-stack-array
        tests/fixtures/submissions/stack_wrong.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_stops_timeout
        F03-stack-array
        tests/fixtures/submissions/stack_timeout.cpp
        1
        "Verdict: TLE"
    )
    add_cli_judge_test(
        judge_reports_runtime_error
        F03-stack-array
        tests/fixtures/submissions/stack_runtime_error.cpp
        1
        "Verdict: RE"
    )
    add_cli_judge_test(
        judge_reports_compile_error
        F03-stack-array
        tests/fixtures/submissions/stack_compile_error.cpp
        1
        "Verdict: CE"
    )
    add_cli_judge_test(
        judge_blocks_forbidden_api
        F03-stack-array
        tests/fixtures/submissions/stack_forbidden_api.cpp
        1
        "Verdict: API"
    )
    add_cli_judge_test(
        judge_bounds_output
        F03-stack-array
        tests/fixtures/submissions/stack_output_flood.cpp
        1
        "output limit exceeded"
    )
    add_cli_judge_test(
        judge_accepts_circular_queue
        F04-circular-queue
        tests/fixtures/submissions/circular_queue_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_non_circular_queue
        F04-circular-queue
        tests/fixtures/submissions/circular_queue_wrong.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_array_deque
        F05-array-deque
        tests/fixtures/submissions/array_deque_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_singly_linked_list
        F06-singly-linked-list
        tests/fixtures/submissions/singly_linked_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_linked_stack
        F07-linked-stack
        tests/fixtures/submissions/linked_stack_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_linked_queue
        F08-linked-queue
        tests/fixtures/submissions/linked_queue_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_stale_queue_tail
        F08-linked-queue
        tests/fixtures/submissions/linked_queue_stale_tail.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_doubly_linked_list
        F09-doubly-linked-list
        tests/fixtures/submissions/doubly_linked_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_circular_linked_list
        F10-circular-linked-list
        tests/fixtures/submissions/circular_linked_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_missing_circular_rotation
        F10-circular-linked-list
        tests/fixtures/submissions/circular_linked_list_no_rotate.cpp
        1
        "Verdict: WA"
    )

    add_cli_stress_test(
        stress_accepts_array_adt
        F01-array-adt
        tests/fixtures/submissions/array_adt_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_dynamic_array
        F02-dynamic-array
        tests/fixtures/submissions/dynamic_array_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_stack
        F03-stack-array
        tests/fixtures/submissions/stack_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_circular_queue
        F04-circular-queue
        tests/fixtures/submissions/circular_queue_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_non_circular_queue
        F04-circular-queue
        tests/fixtures/submissions/circular_queue_wrong.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_array_deque
        F05-array-deque
        tests/fixtures/submissions/array_deque_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_singly_linked_list
        F06-singly-linked-list
        tests/fixtures/submissions/singly_linked_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_linked_stack
        F07-linked-stack
        tests/fixtures/submissions/linked_stack_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_linked_queue
        F08-linked-queue
        tests/fixtures/submissions/linked_queue_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_stale_queue_tail
        F08-linked-queue
        tests/fixtures/submissions/linked_queue_stale_tail.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_doubly_linked_list
        F09-doubly-linked-list
        tests/fixtures/submissions/doubly_linked_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_circular_linked_list
        F10-circular-linked-list
        tests/fixtures/submissions/circular_linked_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_missing_circular_rotation
        F10-circular-linked-list
        tests/fixtures/submissions/circular_linked_list_no_rotate.cpp
        1
        "Verdict: WA"
        TRUE
    )
endif()
