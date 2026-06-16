cmake_minimum_required(VERSION 3.25)

project(data_structures_practice VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(judge_core
    src/catalog.cpp
    src/judge_service.cpp
    src/stress_service.cpp
    src/case_generator.cpp
    src/generators/linear_generators.cpp
    src/generators/tree_generators.cpp
    src/generators/bst_generators.cpp
    src/generators/forest_dsu_generators.cpp
    src/generators/graph_generators.cpp
    src/generators/hashing_generators.cpp
    src/generators/advanced_generators.cpp
    src/generators/segment_tree_generators.cpp
    src/generators/static_range_generators.cpp
)

target_include_directories(judge_core PRIVATE src)

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

# 靜態連結 GCC / C++ 執行庫，讓 algo.exe 自包含，
# 不再依賴外部 libstdc++-6.dll / libgcc_s / libwinpthread（避免版本不符的「找不到輸入點」）。
if(MINGW)
    target_link_options(algo PRIVATE -static -static-libgcc -static-libstdc++)
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
    add_catalog_test(catalog_lists_f41 F41-parent-array-forest)
    add_catalog_test(catalog_lists_f42 F42-child-sibling-forest)
    add_catalog_test(catalog_lists_f43 F43-forest-traversal-views)
    add_catalog_test(catalog_lists_f44 F44-quick-union-disjoint-set)
    add_catalog_test(catalog_lists_f45 F45-weighted-union-disjoint-set)
    add_catalog_test(catalog_lists_f46 F46-path-compression-trace)
    add_catalog_test(catalog_lists_f47 F47-undirected-adjacency-matrix)
    add_catalog_test(catalog_lists_f48 F48-directed-adjacency-matrix)
    add_catalog_test(catalog_lists_f49 F49-undirected-adjacency-list)
    add_catalog_test(catalog_lists_f50 F50-directed-adjacency-list)
    add_catalog_test(catalog_lists_f51 F51-fixed-edge-table-graph)
    add_catalog_test(catalog_lists_f52 F52-modulo-hash-function)
    add_catalog_test(catalog_lists_f53 F53-linear-probing-hash-table)
    add_catalog_test(catalog_lists_f54 F54-quadratic-probing-hash-table)
    add_catalog_test(catalog_lists_f55 F55-double-hashing-table)
    add_catalog_test(catalog_lists_f56 F56-tombstone-hash-table)
    add_catalog_test(catalog_lists_f57 F57-node-pool-chained-hash-table)
    add_catalog_test(catalog_lists_f58 F58-rehashing-linear-probing-table)
    add_catalog_test(catalog_lists_f59 F59-avl-single-rotation-trace)
    add_catalog_test(catalog_lists_f60 F60-avl-double-rotation-trace)
    add_catalog_test(catalog_lists_f61 F61-avl-insertion-tree)
    add_catalog_test(catalog_lists_f62 F62-avl-deletion-tree)
    add_catalog_test(catalog_lists_f63 F63-avl-structural-validation)
    add_catalog_test(catalog_lists_f64 F64-order-four-search-tree-validation)
    add_catalog_test(catalog_lists_f65 F65-red-black-insertion-tree)
    add_catalog_test(catalog_lists_f66 F66-sequence-shift-cost-trace)
    add_catalog_test(catalog_lists_f67 F67-switchable-stack-representation)
    add_catalog_test(catalog_lists_f68 F68-amortized-growth-ledger)
    add_catalog_test(catalog_lists_a01 A01-fenwick-point-prefix)
    add_catalog_test(catalog_lists_a02 A02-fenwick-range-update-point-query)
    add_catalog_test(catalog_lists_a03 A03-fenwick-dual-range-range)
    add_catalog_test(catalog_lists_a04 A04-fenwick-prefix-lower-bound)
    add_catalog_test(catalog_lists_a05 A05-fenwick-frequency-order-statistics)
    add_catalog_test(catalog_lists_a06 A06-fenwick-coordinate-indexed)
    add_catalog_test(catalog_lists_a07 A07-fenwick-two-dimensional)
    add_catalog_test(catalog_lists_a08 A08-fenwick-sparse)
    add_catalog_test(catalog_lists_a09 A09-segment-tree-recursive-sum)
    add_catalog_test(catalog_lists_a10 A10-segment-tree-iterative-sum)
    add_catalog_test(catalog_lists_a11 A11-segment-tree-min-max-monoid)
    add_catalog_test(catalog_lists_a12 A12-segment-tree-gcd-monoid)
    add_catalog_test(catalog_lists_a13 A13-segment-tree-lazy-range-add)
    add_catalog_test(catalog_lists_a14 A14-segment-tree-lazy-range-assign)
    add_catalog_test(catalog_lists_a15 A15-segment-tree-lazy-mixed-tags)
    add_catalog_test(catalog_lists_a16 A16-segment-tree-positional-search)
    add_catalog_test(catalog_lists_a17 A17-segment-tree-dynamic-sparse)
    add_catalog_test(catalog_lists_a18 A18-segment-tree-persistent-versions)
add_catalog_test(catalog_lists_a19 A19-segment-tree-order-statistic)
add_catalog_test(catalog_lists_a20 A20-segment-tree-beats)
add_catalog_test(catalog_lists_a21 A21-sparse-table-range-query)
add_catalog_test(catalog_lists_a22 A22-disjoint-sparse-table-sum)
add_catalog_test(catalog_lists_a23 A23-sqrt-decomposition-range-sum)
add_catalog_test(catalog_lists_a24 A24-block-decomposition-lazy)

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
    add_cli_judge_test(
        judge_accepts_parent_array_forest
        F41-parent-array-forest
        tests/fixtures/submissions/parent_array_forest_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_one_hop_forest_root
        F41-parent-array-forest
        tests/fixtures/submissions/parent_array_forest_one_hop_root.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_child_sibling_forest
        F42-child-sibling-forest
        tests/fixtures/submissions/child_sibling_forest_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_first_child_only_forest
        F42-child-sibling-forest
        tests/fixtures/submissions/child_sibling_forest_first_child_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_forest_traversal_views
        F43-forest-traversal-views
        tests/fixtures/submissions/forest_traversal_views_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_first_root_only_traversal
        F43-forest-traversal-views
        tests/fixtures/submissions/forest_traversal_views_first_root_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_quick_union_disjoint_set
        F44-quick-union-disjoint-set
        tests/fixtures/submissions/quick_union_disjoint_set_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_non_root_union_attach
        F44-quick-union-disjoint-set
        tests/fixtures/submissions/quick_union_non_root_attach.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_weighted_union_disjoint_set
        F45-weighted-union-disjoint-set
        tests/fixtures/submissions/weighted_union_disjoint_set_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_reversed_union_weight
        F45-weighted-union-disjoint-set
        tests/fixtures/submissions/weighted_union_reversed_size.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_path_compression_trace
        F46-path-compression-trace
        tests/fixtures/submissions/path_compression_trace_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_missing_path_compression
        F46-path-compression-trace
        tests/fixtures/submissions/path_compression_trace_no_compression.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_undirected_adjacency_matrix
        F47-undirected-adjacency-matrix
        tests/fixtures/submissions/undirected_adjacency_matrix_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_asymmetric_undirected_matrix
        F47-undirected-adjacency-matrix
        tests/fixtures/submissions/undirected_adjacency_matrix_asymmetric.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_directed_adjacency_matrix
        F48-directed-adjacency-matrix
        tests/fixtures/submissions/directed_adjacency_matrix_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_duplicate_arc_count
        F48-directed-adjacency-matrix
        tests/fixtures/submissions/directed_adjacency_matrix_counts_duplicates.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_undirected_adjacency_list
        F49-undirected-adjacency-list
        tests/fixtures/submissions/undirected_adjacency_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_unsorted_undirected_neighbors
        F49-undirected-adjacency-list
        tests/fixtures/submissions/undirected_adjacency_list_unsorted.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_directed_adjacency_list
        F50-directed-adjacency-list
        tests/fixtures/submissions/directed_adjacency_list_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_stale_directed_indegree
        F50-directed-adjacency-list
        tests/fixtures/submissions/directed_adjacency_list_stale_indegree.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fixed_edge_table_graph
        F51-fixed-edge-table-graph
        tests/fixtures/submissions/fixed_edge_table_graph_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_edge_table_without_reuse
        F51-fixed-edge-table-graph
        tests/fixtures/submissions/fixed_edge_table_graph_no_reuse.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_modulo_hash_function
        F52-modulo-hash-function
        tests/fixtures/submissions/modulo_hash_function_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_negative_hash_remainder
        F52-modulo-hash-function
        tests/fixtures/submissions/modulo_hash_function_negative_remainder.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_linear_probing_hash_table
        F53-linear-probing-hash-table
        tests/fixtures/submissions/linear_probing_hash_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_linear_probing_without_wrap
        F53-linear-probing-hash-table
        tests/fixtures/submissions/linear_probing_hash_table_no_wrap.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_quadratic_probing_hash_table
        F54-quadratic-probing-hash-table
        tests/fixtures/submissions/quadratic_probing_hash_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_linear_as_quadratic_probing
        F54-quadratic-probing-hash-table
        tests/fixtures/submissions/quadratic_probing_hash_table_linear.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_double_hashing_table
        F55-double-hashing-table
        tests/fixtures/submissions/double_hashing_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fixed_step_double_hashing
        F55-double-hashing-table
        tests/fixtures/submissions/double_hashing_table_fixed_step.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_tombstone_hash_table
        F56-tombstone-hash-table
        tests/fixtures/submissions/tombstone_hash_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_lookup_stopping_at_tombstone
        F56-tombstone-hash-table
        tests/fixtures/submissions/tombstone_hash_table_stops_at_deleted.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_node_pool_chained_hash_table
        F57-node-pool-chained-hash-table
        tests/fixtures/submissions/node_pool_chained_hash_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_chaining_collision_overwrite
        F57-node-pool-chained-hash-table
        tests/fixtures/submissions/node_pool_chained_hash_table_overwrites_collision.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_rehashing_linear_probing_table
        F58-rehashing-linear-probing-table
        tests/fixtures/submissions/rehashing_linear_probing_table_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_stale_slots_after_rehash
        F58-rehashing-linear-probing-table
        tests/fixtures/submissions/rehashing_linear_probing_table_stale_slots.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_avl_single_rotation_trace
        F59-avl-single-rotation-trace
        tests/fixtures/submissions/avl_single_rotation_trace_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_wrong_avl_single_rotation
        F59-avl-single-rotation-trace
        tests/fixtures/submissions/avl_single_rotation_trace_wrong_direction.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_avl_double_rotation_trace
        F60-avl-double-rotation-trace
        tests/fixtures/submissions/avl_double_rotation_trace_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_single_only_avl_rotation
        F60-avl-double-rotation-trace
        tests/fixtures/submissions/avl_double_rotation_trace_single_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_avl_insertion_tree
        F61-avl-insertion-tree
        tests/fixtures/submissions/avl_tree_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_avl_without_rebalancing
        F61-avl-insertion-tree
        tests/fixtures/submissions/avl_tree_without_rebalancing.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_avl_deletion_tree
        F62-avl-deletion-tree
        tests/fixtures/submissions/avl_tree_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_avl_without_delete_rebalancing
        F62-avl-deletion-tree
        tests/fixtures/submissions/avl_tree_without_rebalancing.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_avl_structural_validation
        F63-avl-structural-validation
        tests/fixtures/submissions/avl_structural_validation_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_local_only_avl_validation
        F63-avl-structural-validation
        tests/fixtures/submissions/avl_structural_validation_local_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_order_four_search_tree_validation
        F64-order-four-search-tree-validation
        tests/fixtures/submissions/order_four_search_tree_validation_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_missing_multiway_occupancy_check
        F64-order-four-search-tree-validation
        tests/fixtures/submissions/order_four_search_tree_validation_skips_occupancy.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_red_black_insertion_tree
        F65-red-black-insertion-tree
        tests/fixtures/submissions/red_black_insertion_tree_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_plain_bst_as_red_black_tree
        F65-red-black-insertion-tree
        tests/fixtures/submissions/red_black_insertion_tree_plain_bst.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_sequence_shift_cost_trace
        F66-sequence-shift-cost-trace
        tests/fixtures/submissions/sequence_shift_cost_trace_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_counting_inserted_value_as_shift
        F66-sequence-shift-cost-trace
        tests/fixtures/submissions/sequence_shift_cost_trace_counts_inserted_value.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_switchable_stack_representation
        F67-switchable-stack-representation
        tests/fixtures/submissions/switchable_stack_representation_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_stack_reversal_during_representation_switch
        F67-switchable-stack-representation
        tests/fixtures/submissions/switchable_stack_representation_reverses_on_switch.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_amortized_growth_ledger
        F68-amortized-growth-ledger
        tests/fixtures/submissions/amortized_growth_ledger_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_counting_new_value_as_copy
        F68-amortized-growth-ledger
        tests/fixtures/submissions/amortized_growth_ledger_counts_new_element.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_point_prefix
        A01-fenwick-point-prefix
        tests/fixtures/submissions/fenwick_point_prefix_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_off_by_one_range
        A01-fenwick-point-prefix
        tests/fixtures/submissions/fenwick_point_prefix_wrong_range.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_range_point
        A02-fenwick-range-update-point-query
        tests/fixtures/submissions/fenwick_range_point_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_endpoint_only_range_add
        A02-fenwick-range-update-point-query
        tests/fixtures/submissions/fenwick_range_point_endpoint_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_dual_range
        A03-fenwick-dual-range-range
        tests/fixtures/submissions/fenwick_dual_range_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_single_tree_range
        A03-fenwick-dual-range-range
        tests/fixtures/submissions/fenwick_dual_single_tree.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_prefix_lower_bound
        A04-fenwick-prefix-lower-bound
        tests/fixtures/submissions/fenwick_prefix_lower_bound_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_upper_bound_walk
        A04-fenwick-prefix-lower-bound
        tests/fixtures/submissions/fenwick_prefix_lower_bound_upper.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_order_statistics
        A05-fenwick-frequency-order-statistics
        tests/fixtures/submissions/fenwick_order_statistics_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_rank_off_by_one
        A05-fenwick-frequency-order-statistics
        tests/fixtures/submissions/fenwick_order_statistics_off_by_one.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_coordinate_indexed
        A06-fenwick-coordinate-indexed
        tests/fixtures/submissions/fenwick_coordinate_indexed_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_reversed_coordinate_mapping
        A06-fenwick-coordinate-indexed
        tests/fixtures/submissions/fenwick_coordinate_indexed_reversed.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_two_dimensional
        A07-fenwick-two-dimensional
        tests/fixtures/submissions/fenwick_two_dimensional_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_two_dimensional_wrong_y_direction
        A07-fenwick-two-dimensional
        tests/fixtures/submissions/fenwick_two_dimensional_wrong_y_direction.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_fenwick_sparse
        A08-fenwick-sparse
        tests/fixtures/submissions/fenwick_sparse_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_fenwick_sparse_point_only_storage
        A08-fenwick-sparse
        tests/fixtures/submissions/fenwick_sparse_point_only.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_recursive_sum
        A09-segment-tree-recursive-sum
        tests/fixtures/submissions/segment_tree_recursive_sum_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_exclusive_right
        A09-segment-tree-recursive-sum
        tests/fixtures/submissions/segment_tree_recursive_sum_exclusive_right.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_iterative_sum
        A10-segment-tree-iterative-sum
        tests/fixtures/submissions/segment_tree_iterative_sum_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_stale_parent
        A10-segment-tree-iterative-sum
        tests/fixtures/submissions/segment_tree_iterative_sum_stale_parent.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_min_max
        A11-segment-tree-min-max-monoid
        tests/fixtures/submissions/segment_tree_min_max_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_min_max_zero_identity
        A11-segment-tree-min-max-monoid
        tests/fixtures/submissions/segment_tree_min_max_zero_identity.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_gcd
        A12-segment-tree-gcd-monoid
        tests/fixtures/submissions/segment_tree_gcd_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_gcd_identity_one
        A12-segment-tree-gcd-monoid
        tests/fixtures/submissions/segment_tree_gcd_identity_one.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_lazy_add
        A13-segment-tree-lazy-range-add
        tests/fixtures/submissions/segment_tree_lazy_add_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_lazy_add_without_push
        A13-segment-tree-lazy-range-add
        tests/fixtures/submissions/segment_tree_lazy_add_no_push.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_lazy_assign
        A14-segment-tree-lazy-range-assign
        tests/fixtures/submissions/segment_tree_lazy_assign_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_lazy_assign_without_push
        A14-segment-tree-lazy-range-assign
        tests/fixtures/submissions/segment_tree_lazy_assign_no_push.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_lazy_mixed_tags
        A15-segment-tree-lazy-mixed-tags
        tests/fixtures/submissions/segment_tree_lazy_mixed_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_lazy_mixed_reversed_order
        A15-segment-tree-lazy-mixed-tags
        tests/fixtures/submissions/segment_tree_lazy_mixed_reversed_order.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_positional_search
        A16-segment-tree-positional-search
        tests/fixtures/submissions/segment_tree_positional_search_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_positional_search_ignores_bounds
        A16-segment-tree-positional-search
        tests/fixtures/submissions/segment_tree_positional_search_ignores_bounds.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_dynamic_sparse
        A17-segment-tree-dynamic-sparse
        tests/fixtures/submissions/segment_tree_dynamic_sparse_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_dynamic_sparse_eager_children
        A17-segment-tree-dynamic-sparse
        tests/fixtures/submissions/segment_tree_dynamic_sparse_eager_children.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_persistent_versions
        A18-segment-tree-persistent-versions
        tests/fixtures/submissions/segment_tree_persistent_versions_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_persistent_versions_in_place
        A18-segment-tree-persistent-versions
        tests/fixtures/submissions/segment_tree_persistent_versions_in_place.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_order_statistic
        A19-segment-tree-order-statistic
        tests/fixtures/submissions/segment_tree_order_statistic_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_order_statistic_kth_wrong_branch
        A19-segment-tree-order-statistic
        tests/fixtures/submissions/segment_tree_order_statistic_kth_wrong_branch.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_order_statistic_less_inclusive
        A19-segment-tree-order-statistic
        tests/fixtures/submissions/segment_tree_order_statistic_less_inclusive.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_segment_tree_beats
        A20-segment-tree-beats
        tests/fixtures/submissions/segment_tree_beats_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_beats_no_recurse
        A20-segment-tree-beats
        tests/fixtures/submissions/segment_tree_beats_no_recurse.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_rejects_segment_tree_beats_full_count
        A20-segment-tree-beats
        tests/fixtures/submissions/segment_tree_beats_full_count.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_sparse_table_range_query
        A21-sparse-table-range-query
        tests/fixtures/submissions/sparse_table_range_query_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_sparse_table_range_query_drops_endpoint
        A21-sparse-table-range-query
        tests/fixtures/submissions/sparse_table_range_query_drops_endpoint.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_rejects_sparse_table_range_query_single_block
        A21-sparse-table-range-query
        tests/fixtures/submissions/sparse_table_range_query_single_block.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_disjoint_sparse_table_sum
        A22-disjoint-sparse-table-sum
        tests/fixtures/submissions/disjoint_sparse_table_sum_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_disjoint_sparse_table_sum_overlapping_blocks
        A22-disjoint-sparse-table-sum
        tests/fixtures/submissions/disjoint_sparse_table_sum_overlapping_blocks.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_rejects_disjoint_sparse_table_sum_missing_single_case
        A22-disjoint-sparse-table-sum
        tests/fixtures/submissions/disjoint_sparse_table_sum_missing_single_case.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_sqrt_decomposition_range_sum
        A23-sqrt-decomposition-range-sum
        tests/fixtures/submissions/sqrt_decomposition_range_sum_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_sqrt_decomposition_range_sum_stale_block_sum
        A23-sqrt-decomposition-range-sum
        tests/fixtures/submissions/sqrt_decomposition_range_sum_stale_block_sum.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_rejects_sqrt_decomposition_range_sum_whole_block_sums
        A23-sqrt-decomposition-range-sum
        tests/fixtures/submissions/sqrt_decomposition_range_sum_whole_block_sums.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_accepts_block_decomposition_lazy
        A24-block-decomposition-lazy
        tests/fixtures/submissions/block_decomposition_lazy_correct.cpp
        0
        "Verdict: AC"
    )
    add_cli_judge_test(
        judge_rejects_block_decomposition_lazy_partial_ignores_lazy
        A24-block-decomposition-lazy
        tests/fixtures/submissions/block_decomposition_lazy_partial_ignores_lazy.cpp
        1
        "Verdict: WA"
    )
    add_cli_judge_test(
        judge_rejects_block_decomposition_lazy_full_add_skips_sum
        A24-block-decomposition-lazy
        tests/fixtures/submissions/block_decomposition_lazy_full_add_skips_sum.cpp
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
    add_cli_stress_test(
        stress_accepts_parent_array_forest
        F41-parent-array-forest
        tests/fixtures/submissions/parent_array_forest_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_one_hop_forest_root
        F41-parent-array-forest
        tests/fixtures/submissions/parent_array_forest_one_hop_root.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_child_sibling_forest
        F42-child-sibling-forest
        tests/fixtures/submissions/child_sibling_forest_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_first_child_only_forest
        F42-child-sibling-forest
        tests/fixtures/submissions/child_sibling_forest_first_child_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_forest_traversal_views
        F43-forest-traversal-views
        tests/fixtures/submissions/forest_traversal_views_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_first_root_only_traversal
        F43-forest-traversal-views
        tests/fixtures/submissions/forest_traversal_views_first_root_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_quick_union_disjoint_set
        F44-quick-union-disjoint-set
        tests/fixtures/submissions/quick_union_disjoint_set_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_non_root_union_attach
        F44-quick-union-disjoint-set
        tests/fixtures/submissions/quick_union_non_root_attach.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_weighted_union_disjoint_set
        F45-weighted-union-disjoint-set
        tests/fixtures/submissions/weighted_union_disjoint_set_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_reversed_union_weight
        F45-weighted-union-disjoint-set
        tests/fixtures/submissions/weighted_union_reversed_size.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_path_compression_trace
        F46-path-compression-trace
        tests/fixtures/submissions/path_compression_trace_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_missing_path_compression
        F46-path-compression-trace
        tests/fixtures/submissions/path_compression_trace_no_compression.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_undirected_adjacency_matrix
        F47-undirected-adjacency-matrix
        tests/fixtures/submissions/undirected_adjacency_matrix_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_asymmetric_undirected_matrix
        F47-undirected-adjacency-matrix
        tests/fixtures/submissions/undirected_adjacency_matrix_asymmetric.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_directed_adjacency_matrix
        F48-directed-adjacency-matrix
        tests/fixtures/submissions/directed_adjacency_matrix_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_duplicate_arc_count
        F48-directed-adjacency-matrix
        tests/fixtures/submissions/directed_adjacency_matrix_counts_duplicates.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_undirected_adjacency_list
        F49-undirected-adjacency-list
        tests/fixtures/submissions/undirected_adjacency_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_unsorted_undirected_neighbors
        F49-undirected-adjacency-list
        tests/fixtures/submissions/undirected_adjacency_list_unsorted.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_directed_adjacency_list
        F50-directed-adjacency-list
        tests/fixtures/submissions/directed_adjacency_list_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_stale_directed_indegree
        F50-directed-adjacency-list
        tests/fixtures/submissions/directed_adjacency_list_stale_indegree.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fixed_edge_table_graph
        F51-fixed-edge-table-graph
        tests/fixtures/submissions/fixed_edge_table_graph_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_edge_table_without_reuse
        F51-fixed-edge-table-graph
        tests/fixtures/submissions/fixed_edge_table_graph_no_reuse.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_modulo_hash_function
        F52-modulo-hash-function
        tests/fixtures/submissions/modulo_hash_function_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_negative_hash_remainder
        F52-modulo-hash-function
        tests/fixtures/submissions/modulo_hash_function_negative_remainder.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_linear_probing_hash_table
        F53-linear-probing-hash-table
        tests/fixtures/submissions/linear_probing_hash_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_linear_probing_without_wrap
        F53-linear-probing-hash-table
        tests/fixtures/submissions/linear_probing_hash_table_no_wrap.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_quadratic_probing_hash_table
        F54-quadratic-probing-hash-table
        tests/fixtures/submissions/quadratic_probing_hash_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_linear_as_quadratic_probing
        F54-quadratic-probing-hash-table
        tests/fixtures/submissions/quadratic_probing_hash_table_linear.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_double_hashing_table
        F55-double-hashing-table
        tests/fixtures/submissions/double_hashing_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fixed_step_double_hashing
        F55-double-hashing-table
        tests/fixtures/submissions/double_hashing_table_fixed_step.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_tombstone_hash_table
        F56-tombstone-hash-table
        tests/fixtures/submissions/tombstone_hash_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_lookup_stopping_at_tombstone
        F56-tombstone-hash-table
        tests/fixtures/submissions/tombstone_hash_table_stops_at_deleted.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_node_pool_chained_hash_table
        F57-node-pool-chained-hash-table
        tests/fixtures/submissions/node_pool_chained_hash_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_chaining_collision_overwrite
        F57-node-pool-chained-hash-table
        tests/fixtures/submissions/node_pool_chained_hash_table_overwrites_collision.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_rehashing_linear_probing_table
        F58-rehashing-linear-probing-table
        tests/fixtures/submissions/rehashing_linear_probing_table_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_stale_slots_after_rehash
        F58-rehashing-linear-probing-table
        tests/fixtures/submissions/rehashing_linear_probing_table_stale_slots.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_avl_single_rotation_trace
        F59-avl-single-rotation-trace
        tests/fixtures/submissions/avl_single_rotation_trace_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_wrong_avl_single_rotation
        F59-avl-single-rotation-trace
        tests/fixtures/submissions/avl_single_rotation_trace_wrong_direction.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_avl_double_rotation_trace
        F60-avl-double-rotation-trace
        tests/fixtures/submissions/avl_double_rotation_trace_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_single_only_avl_rotation
        F60-avl-double-rotation-trace
        tests/fixtures/submissions/avl_double_rotation_trace_single_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_avl_insertion_tree
        F61-avl-insertion-tree
        tests/fixtures/submissions/avl_tree_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_avl_without_rebalancing
        F61-avl-insertion-tree
        tests/fixtures/submissions/avl_tree_without_rebalancing.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_avl_deletion_tree
        F62-avl-deletion-tree
        tests/fixtures/submissions/avl_tree_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_avl_without_delete_rebalancing
        F62-avl-deletion-tree
        tests/fixtures/submissions/avl_tree_without_rebalancing.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_avl_structural_validation
        F63-avl-structural-validation
        tests/fixtures/submissions/avl_structural_validation_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_local_only_avl_validation
        F63-avl-structural-validation
        tests/fixtures/submissions/avl_structural_validation_local_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_order_four_search_tree_validation
        F64-order-four-search-tree-validation
        tests/fixtures/submissions/order_four_search_tree_validation_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_missing_multiway_occupancy_check
        F64-order-four-search-tree-validation
        tests/fixtures/submissions/order_four_search_tree_validation_skips_occupancy.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_red_black_insertion_tree
        F65-red-black-insertion-tree
        tests/fixtures/submissions/red_black_insertion_tree_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_plain_bst_as_red_black_tree
        F65-red-black-insertion-tree
        tests/fixtures/submissions/red_black_insertion_tree_plain_bst.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_sequence_shift_cost_trace
        F66-sequence-shift-cost-trace
        tests/fixtures/submissions/sequence_shift_cost_trace_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_counting_inserted_value_as_shift
        F66-sequence-shift-cost-trace
        tests/fixtures/submissions/sequence_shift_cost_trace_counts_inserted_value.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_switchable_stack_representation
        F67-switchable-stack-representation
        tests/fixtures/submissions/switchable_stack_representation_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_stack_reversal_during_representation_switch
        F67-switchable-stack-representation
        tests/fixtures/submissions/switchable_stack_representation_reverses_on_switch.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_amortized_growth_ledger
        F68-amortized-growth-ledger
        tests/fixtures/submissions/amortized_growth_ledger_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_counting_new_value_as_copy
        F68-amortized-growth-ledger
        tests/fixtures/submissions/amortized_growth_ledger_counts_new_element.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_point_prefix
        A01-fenwick-point-prefix
        tests/fixtures/submissions/fenwick_point_prefix_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_off_by_one_range
        A01-fenwick-point-prefix
        tests/fixtures/submissions/fenwick_point_prefix_wrong_range.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_range_point
        A02-fenwick-range-update-point-query
        tests/fixtures/submissions/fenwick_range_point_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_endpoint_only_range_add
        A02-fenwick-range-update-point-query
        tests/fixtures/submissions/fenwick_range_point_endpoint_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_dual_range
        A03-fenwick-dual-range-range
        tests/fixtures/submissions/fenwick_dual_range_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_single_tree_range
        A03-fenwick-dual-range-range
        tests/fixtures/submissions/fenwick_dual_single_tree.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_prefix_lower_bound
        A04-fenwick-prefix-lower-bound
        tests/fixtures/submissions/fenwick_prefix_lower_bound_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_upper_bound_walk
        A04-fenwick-prefix-lower-bound
        tests/fixtures/submissions/fenwick_prefix_lower_bound_upper.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_order_statistics
        A05-fenwick-frequency-order-statistics
        tests/fixtures/submissions/fenwick_order_statistics_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_rank_off_by_one
        A05-fenwick-frequency-order-statistics
        tests/fixtures/submissions/fenwick_order_statistics_off_by_one.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_coordinate_indexed
        A06-fenwick-coordinate-indexed
        tests/fixtures/submissions/fenwick_coordinate_indexed_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_reversed_coordinate_mapping
        A06-fenwick-coordinate-indexed
        tests/fixtures/submissions/fenwick_coordinate_indexed_reversed.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_two_dimensional
        A07-fenwick-two-dimensional
        tests/fixtures/submissions/fenwick_two_dimensional_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_two_dimensional_wrong_y_direction
        A07-fenwick-two-dimensional
        tests/fixtures/submissions/fenwick_two_dimensional_wrong_y_direction.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_fenwick_sparse
        A08-fenwick-sparse
        tests/fixtures/submissions/fenwick_sparse_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_fenwick_sparse_point_only_storage
        A08-fenwick-sparse
        tests/fixtures/submissions/fenwick_sparse_point_only.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_recursive_sum
        A09-segment-tree-recursive-sum
        tests/fixtures/submissions/segment_tree_recursive_sum_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_exclusive_right
        A09-segment-tree-recursive-sum
        tests/fixtures/submissions/segment_tree_recursive_sum_exclusive_right.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_iterative_sum
        A10-segment-tree-iterative-sum
        tests/fixtures/submissions/segment_tree_iterative_sum_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_stale_parent
        A10-segment-tree-iterative-sum
        tests/fixtures/submissions/segment_tree_iterative_sum_stale_parent.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_min_max
        A11-segment-tree-min-max-monoid
        tests/fixtures/submissions/segment_tree_min_max_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_min_max_zero_identity
        A11-segment-tree-min-max-monoid
        tests/fixtures/submissions/segment_tree_min_max_zero_identity.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_gcd
        A12-segment-tree-gcd-monoid
        tests/fixtures/submissions/segment_tree_gcd_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_gcd_identity_one
        A12-segment-tree-gcd-monoid
        tests/fixtures/submissions/segment_tree_gcd_identity_one.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_lazy_add
        A13-segment-tree-lazy-range-add
        tests/fixtures/submissions/segment_tree_lazy_add_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_lazy_add_without_push
        A13-segment-tree-lazy-range-add
        tests/fixtures/submissions/segment_tree_lazy_add_no_push.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_lazy_assign
        A14-segment-tree-lazy-range-assign
        tests/fixtures/submissions/segment_tree_lazy_assign_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_lazy_assign_without_push
        A14-segment-tree-lazy-range-assign
        tests/fixtures/submissions/segment_tree_lazy_assign_no_push.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_lazy_mixed_tags
        A15-segment-tree-lazy-mixed-tags
        tests/fixtures/submissions/segment_tree_lazy_mixed_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_lazy_mixed_reversed_order
        A15-segment-tree-lazy-mixed-tags
        tests/fixtures/submissions/segment_tree_lazy_mixed_reversed_order.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_positional_search
        A16-segment-tree-positional-search
        tests/fixtures/submissions/segment_tree_positional_search_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_positional_search_ignores_bounds
        A16-segment-tree-positional-search
        tests/fixtures/submissions/segment_tree_positional_search_ignores_bounds.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_dynamic_sparse
        A17-segment-tree-dynamic-sparse
        tests/fixtures/submissions/segment_tree_dynamic_sparse_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_dynamic_sparse_eager_children
        A17-segment-tree-dynamic-sparse
        tests/fixtures/submissions/segment_tree_dynamic_sparse_eager_children.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_persistent_versions
        A18-segment-tree-persistent-versions
        tests/fixtures/submissions/segment_tree_persistent_versions_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_persistent_versions_in_place
        A18-segment-tree-persistent-versions
        tests/fixtures/submissions/segment_tree_persistent_versions_in_place.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_order_statistic
        A19-segment-tree-order-statistic
        tests/fixtures/submissions/segment_tree_order_statistic_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_order_statistic_kth_wrong_branch
        A19-segment-tree-order-statistic
        tests/fixtures/submissions/segment_tree_order_statistic_kth_wrong_branch.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_order_statistic_less_inclusive
        A19-segment-tree-order-statistic
        tests/fixtures/submissions/segment_tree_order_statistic_less_inclusive.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_segment_tree_beats
        A20-segment-tree-beats
        tests/fixtures/submissions/segment_tree_beats_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_beats_no_recurse
        A20-segment-tree-beats
        tests/fixtures/submissions/segment_tree_beats_no_recurse.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_rejects_segment_tree_beats_full_count
        A20-segment-tree-beats
        tests/fixtures/submissions/segment_tree_beats_full_count.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_sparse_table_range_query
        A21-sparse-table-range-query
        tests/fixtures/submissions/sparse_table_range_query_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_sparse_table_range_query_drops_endpoint
        A21-sparse-table-range-query
        tests/fixtures/submissions/sparse_table_range_query_drops_endpoint.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_rejects_sparse_table_range_query_single_block
        A21-sparse-table-range-query
        tests/fixtures/submissions/sparse_table_range_query_single_block.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_disjoint_sparse_table_sum
        A22-disjoint-sparse-table-sum
        tests/fixtures/submissions/disjoint_sparse_table_sum_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_disjoint_sparse_table_sum_overlapping_blocks
        A22-disjoint-sparse-table-sum
        tests/fixtures/submissions/disjoint_sparse_table_sum_overlapping_blocks.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_rejects_disjoint_sparse_table_sum_missing_single_case
        A22-disjoint-sparse-table-sum
        tests/fixtures/submissions/disjoint_sparse_table_sum_missing_single_case.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_sqrt_decomposition_range_sum
        A23-sqrt-decomposition-range-sum
        tests/fixtures/submissions/sqrt_decomposition_range_sum_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_sqrt_decomposition_range_sum_stale_block_sum
        A23-sqrt-decomposition-range-sum
        tests/fixtures/submissions/sqrt_decomposition_range_sum_stale_block_sum.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_rejects_sqrt_decomposition_range_sum_whole_block_sums
        A23-sqrt-decomposition-range-sum
        tests/fixtures/submissions/sqrt_decomposition_range_sum_whole_block_sums.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_accepts_block_decomposition_lazy
        A24-block-decomposition-lazy
        tests/fixtures/submissions/block_decomposition_lazy_correct.cpp
        0
        "Verdict: AC"
        FALSE
    )
    add_cli_stress_test(
        stress_rejects_block_decomposition_lazy_partial_ignores_lazy
        A24-block-decomposition-lazy
        tests/fixtures/submissions/block_decomposition_lazy_partial_ignores_lazy.cpp
        1
        "Verdict: WA"
        TRUE
    )
    add_cli_stress_test(
        stress_rejects_block_decomposition_lazy_full_add_skips_sum
        A24-block-decomposition-lazy
        tests/fixtures/submissions/block_decomposition_lazy_full_add_skips_sum.cpp
        1
        "Verdict: WA"
        TRUE
    )
endif()
