file(REMOVE_RECURSE "${WORK_ROOT}")
file(MAKE_DIRECTORY "${WORK_ROOT}/problems/foundation/stack")
file(COPY "${PROJECT_ROOT}/problems/foundation/stack/F03-stack-array"
     DESTINATION "${WORK_ROOT}/problems/foundation/stack")
file(MAKE_DIRECTORY "${WORK_ROOT}/submissions")
file(COPY
    "${PROJECT_ROOT}/tests/fixtures/submissions/stack_correct.cpp"
    "${PROJECT_ROOT}/tests/fixtures/submissions/stack_wrong.cpp"
    DESTINATION "${WORK_ROOT}/submissions")

execute_process(
    COMMAND "${ALGO}" test F03-stack-array
        "${WORK_ROOT}/submissions/stack_correct.cpp" --root "${WORK_ROOT}"
    RESULT_VARIABLE ac_result OUTPUT_QUIET ERROR_VARIABLE ac_error
)
if(NOT ac_result EQUAL 0)
    message(FATAL_ERROR "AC attempt failed: ${ac_error}")
endif()
execute_process(
    COMMAND "${ALGO}" test F03-stack-array
        "${WORK_ROOT}/submissions/stack_wrong.cpp" --root "${WORK_ROOT}"
    RESULT_VARIABLE wa_result OUTPUT_QUIET ERROR_VARIABLE wa_error
)
if(NOT wa_result EQUAL 1)
    message(FATAL_ERROR "WA attempt returned ${wa_result}: ${wa_error}")
endif()
execute_process(
    COMMAND "${ALGO}" test F03-stack-array
        "${WORK_ROOT}/submissions/stack_correct.cpp" --no-progress
        --root "${WORK_ROOT}"
    RESULT_VARIABLE opt_out_result OUTPUT_QUIET ERROR_VARIABLE opt_out_error
)
if(NOT opt_out_result EQUAL 0)
    message(FATAL_ERROR "opt-out attempt failed: ${opt_out_error}")
endif()
execute_process(
    COMMAND "${ALGO}" progress F03-stack-array --root "${WORK_ROOT}"
    RESULT_VARIABLE progress_result OUTPUT_VARIABLE progress_output
    ERROR_VARIABLE progress_error
)
if(NOT progress_result EQUAL 0)
    message(FATAL_ERROR "progress query failed: ${progress_error}")
endif()
if(NOT progress_output MATCHES
   "F03-stack-array \\| attempts 2 \\| accepted 1 \\| best [0-9]+ ms \\| last WA")
    message(FATAL_ERROR "unexpected progress output: ${progress_output}")
endif()
file(READ "${WORK_ROOT}/.judge/progress.json" progress_json)
if(NOT progress_json MATCHES "\"schema_version\": 1")
    message(FATAL_ERROR "missing schema version: ${progress_json}")
endif()
file(REMOVE_RECURSE "${WORK_ROOT}")
