if(NOT DEFINED ALGO OR NOT DEFINED PROJECT_ROOT OR NOT DEFINED PROBLEM_ID OR
   NOT DEFINED SOURCE_FILE OR NOT DEFINED SEED OR NOT DEFINED CASE_COUNT)
    message(FATAL_ERROR
        "ALGO, PROJECT_ROOT, PROBLEM_ID, SOURCE_FILE, SEED, and CASE_COUNT are required"
    )
endif()

execute_process(
    COMMAND
        "${ALGO}" stress "${PROBLEM_ID}" "${SOURCE_FILE}"
        --seed "${SEED}"
        --cases "${CASE_COUNT}"
        --root "${PROJECT_ROOT}"
    RESULT_VARIABLE actual_exit
    OUTPUT_VARIABLE standard_output
    ERROR_VARIABLE standard_error
    TIMEOUT 60
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

string(FIND "${combined_output}" "Seed: ${SEED}" seed_index)
if(seed_index EQUAL -1)
    message(FATAL_ERROR
        "Expected output to contain reproducible seed ${SEED}\n${combined_output}"
    )
endif()

if(EXPECT_FAILURE_CONTEXT)
    foreach(required_text "Case seed:" "Replay: algo stress" "Failing input:")
        string(FIND "${combined_output}" "${required_text}" required_index)
        if(required_index EQUAL -1)
            message(FATAL_ERROR
                "Expected failure output to contain '${required_text}'\n${combined_output}"
            )
        endif()
    endforeach()
endif()
