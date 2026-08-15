#!/usr/bin/env bash

set -uo pipefail

RESET='\033[0m'
RED='\033[1;31m'
RED_THIN='\033[0;31m'
GREEN='\033[1;32m'
GREEN_THIN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
BOLD='\033[1m'
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="${SCRIPT_DIR}/e2e"
TESTS_FILE="${TESTS_DIR}/tests.txt"
TESTDIR_PATH="./testdir"
DELIMITER="::"

trim() {
    local value="$1"
    value="${value#"${value%%[![:space:]]*}"}"
    value="${value%"${value##*[![:space:]]}"}"
    printf '%s' "$value"
}

print_pass() {
    printf '%b[✔] %b%s%b\n' "${GREEN}" "${GREEN_THIN}" "$1" "${RESET}"
}

print_fail() {
    printf '%b[✘] %s%b\n' "${RED}" "$1" "${RESET}"
}

print_info() {
    printf '%b%s%b\n' "${RESET}" "$1" "${RESET}"
}

print_summary() {
    printf '%bSummary: %d tests executed, %d passed, %d failed%b\n' "${YELLOW}" "$1" "$2" "$3" "${RESET}"
}

prepare_test_environment() {
    rm -rf "${TESTDIR_PATH}"
    mkdir -p "${TESTDIR_PATH}/ssfaf"
    mkdir -p "${TESTDIR_PATH}/testttttatatat"

    touch "${TESTDIR_PATH}/'my"
    touch "${TESTDIR_PATH}/'my file'"
    touch "${TESTDIR_PATH}/\"file\""
    touch "${TESTDIR_PATH}/file"
    touch "${TESTDIR_PATH}/file'"
    touch "${TESTDIR_PATH}/file2"
    touch "${TESTDIR_PATH}/my file"
    touch "${TESTDIR_PATH}/my-pdf.pdf"
    touch "${TESTDIR_PATH}/myfiñe"
    touch "${TESTDIR_PATH}/nwl"
    touch "${TESTDIR_PATH}/test@pdf"
    touch "${TESTDIR_PATH}/tab\tfile"
    touch "${TESTDIR_PATH}/a"
    touch "${TESTDIR_PATH}/file "
    touch "${TESTDIR_PATH}/ "

    ln -s "ssfaf" "${TESTDIR_PATH}/link_to_ssfaf"
    printf 'hello' > "${TESTDIR_PATH}/file"
    printf 'world' > "${TESTDIR_PATH}/file2"
    printf 'sample' > "${TESTDIR_PATH}/a"
    printf 'data' > "${TESTDIR_PATH}/nwl"
}

remove_test_environment() {
    rm -rf "${TESTDIR_PATH}"
}

parse_test_entry() {
    local line="$1"
    local ft_cmd=""
    local model_cmd=""

    if [[ "$line" != *"${DELIMITER}"* ]]; then
        printf '%b[ERROR] Invalid test entry: %s%b\n' "${RED}" "$line" "${RESET}" >&2
        return 1
    fi

    ft_cmd="$(trim "${line%%"${DELIMITER}"*}")"
    model_cmd="$(trim "${line#*"${DELIMITER}"}")"

    if [[ -z "${ft_cmd}" || -z "${model_cmd}" ]]; then
        printf '%b[ERROR] Missing command or model in test entry: %s%b\n' "${RED}" "$line" "${RESET}" >&2
        return 1
    fi

    printf '%s\n%s\n' "${ft_cmd}" "${model_cmd}"
}

print_output_block() {
    local label="$1"
    local content="$2"

    printf '%b-> %s:%b\n' "${RED_THIN}" "${label}" "${RESET}"
    printf '%b--------------------------%b\n' "${RED_THIN}" "${RESET}"
    printf '%s\n' "${content}"
    printf '%b--------------------------%b\n' "${RED_THIN}" "${RESET}"
}

run_single_test() {
    local ft_cmd="$1"
    local model_cmd="$2"
    local ft_output="$(mktemp)"
    local model_output="$(mktemp)"
    local ft_status=0
    local model_status=0

    bash -lc "${ft_cmd}" >"${ft_output}" 2>&1 || ft_status=$?
    bash -lc "${model_cmd}" >"${model_output}" 2>&1 || model_status=$?

    if [[ "${ft_status}" -eq "${model_status}" ]] && diff -u "${model_output}" "${ft_output}" >/dev/null 2>&1; then
        print_pass "${ft_cmd}"
        passed=$((passed + 1))
    else
        print_fail "${ft_cmd}"
        printf '%b-> ft_ls status: %s | ls status: %s%b\n' "${RED_THIN}" "${ft_status}" "${model_status}" "${RESET}"
        printf '%b-> model command: %s%b\n' "${RED_THIN}" "${model_cmd}" "${RESET}"

        if [[ -s "${model_output}" ]]; then
            print_output_block "ls output" "$(cat "${model_output}")"
        else
            print_info "-> ls output: <empty>"
        fi

        if [[ -s "${ft_output}" ]]; then
            print_output_block "ft_ls output" "$(cat "${ft_output}")"
        else
            print_info "-> ft_ls output: <empty>"
        fi

        if ! diff -u "${model_output}" "${ft_output}" >/dev/null 2>&1; then
            printf '%b-> diff:%b\n' "${RED_THIN}" "${RESET}"
            printf '%b--------------------------%b\n' "${RED_THIN}" "${RESET}"
            diff -u "${model_output}" "${ft_output}" || true
            printf '%b--------------------------%b\n\n' "${RED_THIN}" "${RESET}"
        fi

        failed=$((failed + 1))
    fi

    total=$((total + 1))
    rm -f "${ft_output}" "${model_output}"
}

main() {
    if [[ ! -f "${TESTS_FILE}" ]]; then
        printf '%b[ERROR] Test file not found: %s%b\n' "${RED}" "${TESTS_FILE}" "${RESET}" >&2
        exit 1
    fi

    export LANG=C
    export LC_ALL=C

    prepare_test_environment

    local line=""
    local parsed=""
    local ft_cmd=""
    local model_cmd=""

    passed=0
    failed=0
    total=0

    while IFS= read -r line || [[ -n "${line}" ]]; do
        line="$(trim "${line}")"

        if [[ -z "${line}" || "${line}" =~ ^# ]]; then
            continue
        fi

        parsed="$(parse_test_entry "${line}")" || continue
        ft_cmd="$(printf '%s\n' "${parsed}" | sed -n '1p')"
        model_cmd="$(printf '%s\n' "${parsed}" | sed -n '2p')"
        run_single_test "${ft_cmd}" "${model_cmd}"
    done < "${TESTS_FILE}"

    echo
    print_summary "${total}" "${passed}" "${failed}"

    remove_test_environment

    if [[ "${failed}" -ne 0 ]]; then
        exit 1
    fi

    exit 0
}

main "$@"
