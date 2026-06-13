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
    add_catalog_test(catalog_lists_f11 F11-singly-list-reverse)
    add_catalog_test(catalog_lists_f12 F12-singly-list-concatenate)
    add_catalog_test(catalog_lists_f13 F13-singly-list-split)
    add_catalog_test(catalog_lists_f14 F14-doubly-list-splice)
    add_catalog_test(catalog_lists_f15 F15-fixed-node-pool-list)
    add_catalog_test(catalog_lists_f16 F16-sentinel-doubly-list)
    add_catalog_test(catalog_lists_f17 F17-cursor-doubly-list)
    add_catalog_test(catalog_lists_f18 F18-record-table)
    add_catalog_test(catalog_lists_f19 F19-sparse-polynomial)
    add_catalog_test(catalog_lists_f20 F20-triplet-sparse-matrix)
    add_catalog_test(catalog_lists_f21 F21-csr-sparse-matrix)
    add_catalog_test(catalog_lists_f22 F22-row-major-matrix)
    add_catalog_test(catalog_lists_f23 F23-row-major-tensor)
    add_catalog_test(catalog_lists_f24 F24-two-stacks-one-array)
    add_catalog_test(catalog_lists_f25 F25-circular-linked-queue)
    add_catalog_test(catalog_lists_f26 F26-linked-deque)
    add_catalog_test(catalog_lists_f27 F27-array-binary-tree)
    add_catalog_test(catalog_lists_f28 F28-linked-binary-tree)
    add_catalog_test(catalog_lists_f29 F29-inorder-threaded-tree)
    add_catalog_test(catalog_lists_f30 F30-binary-min-heap)
    add_catalog_test(catalog_lists_f31 F31-binary-tree-traversal-views)
    add_catalog_test(catalog_lists_f32 F32-level-order-tree-view)
    add_catalog_test(catalog_lists_f33 F33-bottom-up-min-heap-build)
    add_catalog_test(catalog_lists_f34 F34-min-heap-removal-trace)
    add_catalog_test(catalog_lists_f35 F35-bst-insert-search)
    add_catalog_test(catalog_lists_f36 F36-bst-deletion-cases)
    add_catalog_test(catalog_lists_f37 F37-bst-predecessor-successor)
    add_catalog_test(catalog_lists_f38 F38-bst-range-view)
    add_catalog_test(catalog_lists_f39 F39-bst-structural-validation)
    add_catalog_test(catalog_lists_f40 F40-bst-replacement-trace)

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
    add_cli_judge_test(
        judge_accepts_singly_list_reverse
        F11-singly-list-reverse
        tests/fixtures/submissions/singly_list_reverse_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_lost_reverse_tail
        F11-singly-list-reverse
        tests/fixtures/submissions/singly_list_reverse_lost_tail.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_singly_list_concatenate
        F12-singly-list-concatenate
        tests/fixtures/submissions/singly_list_concatenate_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_singly_list_split
        F13-singly-list-split
        tests/fixtures/submissions/singly_list_split_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_accepts_doubly_list_splice
        F14-doubly-list-splice
        tests/fixtures/submissions/doubly_list_splice_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_broken_splice_backlink
        F14-doubly-list-splice
        tests/fixtures/submissions/doubly_list_splice_broken_backlink.cpp
        1
        "Verdict: RE"
    )
    add_cli_judge_test(
        judge_accepts_fixed_node_pool
        F15-fixed-node-pool-list
        tests/fixtures/submissions/fixed_node_pool_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_pool_without_reuse
        F15-fixed-node-pool-list
        tests/fixtures/submissions/fixed_node_pool_list_no_reuse.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_sentinel_doubly_list
        F16-sentinel-doubly-list
        tests/fixtures/submissions/sentinel_doubly_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_broken_sentinel_backlink
        F16-sentinel-doubly-list
        tests/fixtures/submissions/sentinel_doubly_list_broken_backlink.cpp
        1
        "Verdict:"
    )
    add_cli_judge_test(
        judge_accepts_cursor_doubly_list
        F17-cursor-doubly-list
        tests/fixtures/submissions/cursor_doubly_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_cursor_without_reuse
        F17-cursor-doubly-list
        tests/fixtures/submissions/cursor_doubly_list_no_reuse.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_record_table
        F18-record-table
        tests/fixtures/submissions/record_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_record_table_without_reuse
        F18-record-table
        tests/fixtures/submissions/record_table_no_reuse.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_sparse_polynomial
        F19-sparse-polynomial
        tests/fixtures/submissions/sparse_polynomial_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_ascending_polynomial_terms
        F19-sparse-polynomial
        tests/fixtures/submissions/sparse_polynomial_ascending.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_triplet_sparse_matrix
        F20-triplet-sparse-matrix
        tests/fixtures/submissions/triplet_sparse_matrix_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_unsorted_triplets
        F20-triplet-sparse-matrix
        tests/fixtures/submissions/triplet_sparse_matrix_unsorted.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_csr_sparse_matrix
        F21-csr-sparse-matrix
        tests/fixtures/submissions/csr_sparse_matrix_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_shifted_csr_offsets
        F21-csr-sparse-matrix
        tests/fixtures/submissions/csr_sparse_matrix_shifted_offsets.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_row_major_matrix
        F22-row-major-matrix
        tests/fixtures/submissions/row_major_matrix_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_column_major_matrix
        F22-row-major-matrix
        tests/fixtures/submissions/row_major_matrix_column_major.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_row_major_tensor
        F23-row-major-tensor
        tests/fixtures/submissions/row_major_tensor_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_wrong_tensor_stride
        F23-row-major-tensor
        tests/fixtures/submissions/row_major_tensor_wrong_stride.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_two_stacks_one_array
        F24-two-stacks-one-array
        tests/fixtures/submissions/two_stacks_one_array_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fixed_stack_partition
        F24-two-stacks-one-array
        tests/fixtures/submissions/two_stacks_fixed_partition.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_circular_linked_queue
        F25-circular-linked-queue
        tests/fixtures/submissions/circular_linked_queue_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_queue_without_rotate
        F25-circular-linked-queue
        tests/fixtures/submissions/circular_linked_queue_no_rotate.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_linked_deque
        F26-linked-deque
        tests/fixtures/submissions/linked_deque_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_stale_deque_tail
        F26-linked-deque
        tests/fixtures/submissions/linked_deque_stale_tail.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_array_binary_tree
        F27-array-binary-tree
        tests/fixtures/submissions/array_binary_tree_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_one_based_tree_indices
        F27-array-binary-tree
        tests/fixtures/submissions/array_binary_tree_one_based.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_linked_binary_tree
        F28-linked-binary-tree
        tests/fixtures/submissions/linked_binary_tree_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_missing_tree_parent_links
        F28-linked-binary-tree
        tests/fixtures/submissions/linked_binary_tree_missing_parent.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_inorder_threaded_tree
        F29-inorder-threaded-tree
        tests/fixtures/submissions/inorder_threaded_tree_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_id_neighbor_threads
        F29-inorder-threaded-tree
        tests/fixtures/submissions/inorder_threaded_tree_id_neighbors.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_binary_min_heap
        F30-binary-min-heap
        tests/fixtures/submissions/binary_min_heap_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_heap_without_change_sift_down
        F30-binary-min-heap
        tests/fixtures/submissions/binary_min_heap_no_sift_down_change.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_binary_tree_traversal_views
        F31-binary-tree-traversal-views
        tests/fixtures/submissions/binary_tree_traversal_views_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_wrong_preorder_view
        F31-binary-tree-traversal-views
        tests/fixtures/submissions/binary_tree_traversal_views_wrong_preorder.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_level_order_tree_view
        F32-level-order-tree-view
        tests/fixtures/submissions/level_order_tree_view_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_dfs_as_level_order
        F32-level-order-tree-view
        tests/fixtures/submissions/level_order_tree_view_dfs.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bottom_up_heap_build
        F33-bottom-up-min-heap-build
        tests/fixtures/submissions/bottom_up_min_heap_build_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_repeated_push_heap_build
        F33-bottom-up-min-heap-build
        tests/fixtures/submissions/bottom_up_min_heap_build_repeated_push.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_min_heap_removal_trace
        F34-min-heap-removal-trace
        tests/fixtures/submissions/min_heap_removal_trace_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_removal_without_sift_down
        F34-min-heap-removal-trace
        tests/fixtures/submissions/min_heap_removal_trace_no_sift_down.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bst_insert_search
        F35-bst-insert-search
        tests/fixtures/submissions/bst_insert_search_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_bst_duplicate_overwrite
        F35-bst-insert-search
        tests/fixtures/submissions/bst_insert_search_overwrites_duplicate.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bst_deletion_cases
        F36-bst-deletion-cases
        tests/fixtures/submissions/bst_deletion_cases_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_bst_predecessor_replacement
        F36-bst-deletion-cases
        tests/fixtures/submissions/bst_deletion_uses_predecessor.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bst_predecessor_successor
        F37-bst-predecessor-successor
        tests/fixtures/submissions/bst_predecessor_successor_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_bst_parent_only_neighbors
        F37-bst-predecessor-successor
        tests/fixtures/submissions/bst_predecessor_successor_parent_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bst_range_view
        F38-bst-range-view
        tests/fixtures/submissions/bst_range_view_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_bst_exclusive_range
        F38-bst-range-view
        tests/fixtures/submissions/bst_range_view_excludes_endpoints.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bst_structural_validation
        F39-bst-structural-validation
        tests/fixtures/submissions/bst_structural_validation_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_bst_local_only_validation
        F39-bst-structural-validation
        tests/fixtures/submissions/bst_structural_validation_local_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_bst_replacement_trace
        F40-bst-replacement-trace
        tests/fixtures/submissions/bst_replacement_trace_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_bst_key_copy_replacement
        F40-bst-replacement-trace
        tests/fixtures/submissions/bst_replacement_trace_copies_key.cpp
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
    add_cli_stress_test(
        stress_accepts_singly_list_reverse
        F11-singly-list-reverse
        tests/fixtures/submissions/singly_list_reverse_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_lost_reverse_tail
        F11-singly-list-reverse
        tests/fixtures/submissions/singly_list_reverse_lost_tail.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_singly_list_concatenate
        F12-singly-list-concatenate
        tests/fixtures/submissions/singly_list_concatenate_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_singly_list_split
        F13-singly-list-split
        tests/fixtures/submissions/singly_list_split_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_accepts_doubly_list_splice
        F14-doubly-list-splice
        tests/fixtures/submissions/doubly_list_splice_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_broken_splice_backlink
        F14-doubly-list-splice
        tests/fixtures/submissions/doubly_list_splice_broken_backlink.cpp
        1
        "Verdict: RE"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fixed_node_pool
        F15-fixed-node-pool-list
        tests/fixtures/submissions/fixed_node_pool_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_pool_without_reuse
        F15-fixed-node-pool-list
        tests/fixtures/submissions/fixed_node_pool_list_no_reuse.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_sentinel_doubly_list
        F16-sentinel-doubly-list
        tests/fixtures/submissions/sentinel_doubly_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_broken_sentinel_backlink
        F16-sentinel-doubly-list
        tests/fixtures/submissions/sentinel_doubly_list_broken_backlink.cpp
        1
        "Verdict:"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_cursor_doubly_list
        F17-cursor-doubly-list
        tests/fixtures/submissions/cursor_doubly_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_cursor_without_reuse
        F17-cursor-doubly-list
        tests/fixtures/submissions/cursor_doubly_list_no_reuse.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_record_table
        F18-record-table
        tests/fixtures/submissions/record_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_record_table_without_reuse
        F18-record-table
        tests/fixtures/submissions/record_table_no_reuse.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_sparse_polynomial
        F19-sparse-polynomial
        tests/fixtures/submissions/sparse_polynomial_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_ascending_polynomial_terms
        F19-sparse-polynomial
        tests/fixtures/submissions/sparse_polynomial_ascending.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_triplet_sparse_matrix
        F20-triplet-sparse-matrix
        tests/fixtures/submissions/triplet_sparse_matrix_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_unsorted_triplets
        F20-triplet-sparse-matrix
        tests/fixtures/submissions/triplet_sparse_matrix_unsorted.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_csr_sparse_matrix
        F21-csr-sparse-matrix
        tests/fixtures/submissions/csr_sparse_matrix_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_shifted_csr_offsets
        F21-csr-sparse-matrix
        tests/fixtures/submissions/csr_sparse_matrix_shifted_offsets.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_row_major_matrix
        F22-row-major-matrix
        tests/fixtures/submissions/row_major_matrix_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_column_major_matrix
        F22-row-major-matrix
        tests/fixtures/submissions/row_major_matrix_column_major.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_row_major_tensor
        F23-row-major-tensor
        tests/fixtures/submissions/row_major_tensor_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_wrong_tensor_stride
        F23-row-major-tensor
        tests/fixtures/submissions/row_major_tensor_wrong_stride.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_two_stacks_one_array
        F24-two-stacks-one-array
        tests/fixtures/submissions/two_stacks_one_array_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fixed_stack_partition
        F24-two-stacks-one-array
        tests/fixtures/submissions/two_stacks_fixed_partition.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_circular_linked_queue
        F25-circular-linked-queue
        tests/fixtures/submissions/circular_linked_queue_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_queue_without_rotate
        F25-circular-linked-queue
        tests/fixtures/submissions/circular_linked_queue_no_rotate.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_linked_deque
        F26-linked-deque
        tests/fixtures/submissions/linked_deque_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_stale_deque_tail
        F26-linked-deque
        tests/fixtures/submissions/linked_deque_stale_tail.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_array_binary_tree
        F27-array-binary-tree
        tests/fixtures/submissions/array_binary_tree_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_one_based_tree_indices
        F27-array-binary-tree
        tests/fixtures/submissions/array_binary_tree_one_based.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_linked_binary_tree
        F28-linked-binary-tree
        tests/fixtures/submissions/linked_binary_tree_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_missing_tree_parent_links
        F28-linked-binary-tree
        tests/fixtures/submissions/linked_binary_tree_missing_parent.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_inorder_threaded_tree
        F29-inorder-threaded-tree
        tests/fixtures/submissions/inorder_threaded_tree_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_id_neighbor_threads
        F29-inorder-threaded-tree
        tests/fixtures/submissions/inorder_threaded_tree_id_neighbors.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_binary_min_heap
        F30-binary-min-heap
        tests/fixtures/submissions/binary_min_heap_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_heap_without_change_sift_down
        F30-binary-min-heap
        tests/fixtures/submissions/binary_min_heap_no_sift_down_change.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_binary_tree_traversal_views
        F31-binary-tree-traversal-views
        tests/fixtures/submissions/binary_tree_traversal_views_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_wrong_preorder_view
        F31-binary-tree-traversal-views
        tests/fixtures/submissions/binary_tree_traversal_views_wrong_preorder.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_level_order_tree_view
        F32-level-order-tree-view
        tests/fixtures/submissions/level_order_tree_view_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_dfs_as_level_order
        F32-level-order-tree-view
        tests/fixtures/submissions/level_order_tree_view_dfs.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bottom_up_heap_build
        F33-bottom-up-min-heap-build
        tests/fixtures/submissions/bottom_up_min_heap_build_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_repeated_push_heap_build
        F33-bottom-up-min-heap-build
        tests/fixtures/submissions/bottom_up_min_heap_build_repeated_push.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_min_heap_removal_trace
        F34-min-heap-removal-trace
        tests/fixtures/submissions/min_heap_removal_trace_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_removal_without_sift_down
        F34-min-heap-removal-trace
        tests/fixtures/submissions/min_heap_removal_trace_no_sift_down.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bst_insert_search
        F35-bst-insert-search
        tests/fixtures/submissions/bst_insert_search_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_bst_duplicate_overwrite
        F35-bst-insert-search
        tests/fixtures/submissions/bst_insert_search_overwrites_duplicate.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bst_deletion_cases
        F36-bst-deletion-cases
        tests/fixtures/submissions/bst_deletion_cases_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_bst_predecessor_replacement
        F36-bst-deletion-cases
        tests/fixtures/submissions/bst_deletion_uses_predecessor.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bst_predecessor_successor
        F37-bst-predecessor-successor
        tests/fixtures/submissions/bst_predecessor_successor_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_bst_parent_only_neighbors
        F37-bst-predecessor-successor
        tests/fixtures/submissions/bst_predecessor_successor_parent_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bst_range_view
        F38-bst-range-view
        tests/fixtures/submissions/bst_range_view_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_bst_exclusive_range
        F38-bst-range-view
        tests/fixtures/submissions/bst_range_view_excludes_endpoints.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bst_structural_validation
        F39-bst-structural-validation
        tests/fixtures/submissions/bst_structural_validation_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_bst_local_only_validation
        F39-bst-structural-validation
        tests/fixtures/submissions/bst_structural_validation_local_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_bst_replacement_trace
        F40-bst-replacement-trace
        tests/fixtures/submissions/bst_replacement_trace_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_bst_key_copy_replacement
        F40-bst-replacement-trace
        tests/fixtures/submissions/bst_replacement_trace_copies_key.cpp
        1
        "Verdict: WA"
        TRUE
    )
endif()
