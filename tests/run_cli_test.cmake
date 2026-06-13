if(NOT DEFINED ALGO OR NOT DEFINED PROJECT_ROOT OR NOT DEFINED PROBLEM_ID OR
   NOT DEFINED SOURCE_FILE)
    message(FATAL_ERROR
        "ALGO, PROJECT_ROOT, PROBLEM_ID, and SOURCE_FILE are required"
    )
endif()

execute_process(
    COMMAND
        "${ALGO}" test "${PROBLEM_ID}" "${SOURCE_FILE}" --root "${PROJECT_ROOT}"
    RESULT_VARIABLE actual_exit
    OUTPUT_VARIABLE standard_output
    ERROR_VARIABLE standard_error
    TIMEOUT 30
)

set(combined_output "${standard_output}\n${standard_error}")

if(NOT actual_exit EQUAL EXPECTED_EXIT)
    message(FATAL_ERROR
        "Expected exit ${EXPECTED_EXIT}, got ${actual_exit}\n${combined_output}"
    )
endif()

string(FIND "${combined_output}" "${EXPECTED_TEXT}" match_index)
if(match_index EQUAL -1)
    message(FATAL_ERROR
        "Expected output to contain '${EXPECTED_TEXT}'\n${combined_output}"
    )
endif()
