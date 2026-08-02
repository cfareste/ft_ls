#!/usr/bin/env bash
set -euo pipefail
export LC_ALL=C

REPORTS_DIR="./reports"
OUTPUT_FILE="./$REPORTS_DIR/reports.txt"

list_gcov_files() {
    find "$REPORTS_DIR" -maxdepth 1 -type f -name '*.gcov' | sort
}

extract_function_lines() {
    local file="$1"
    grep -E '^function ' "$file" || true
}

blocks_percentage_from_line() {
    local line="$1"
    echo "$line" | grep -oE 'blocks executed [0-9]+([.][0-9]+)?%' | awk '{print $3}' | tr -d '%'
}

write_file_summary() {
    local file="$1"
    local filename

    filename="$(basename "$file")"
    printf '%s:\n' "$filename" >> "$OUTPUT_FILE"

    while IFS= read -r line; do
        [ -n "$line" ] || continue
        printf ' - %s\n' "$line" >> "$OUTPUT_FILE"
    done < <(extract_function_lines "$file")

    printf '\n' >> "$OUTPUT_FILE"
}

compute_mean_percentage() {
    local total=0
    local count=0
    local file
    local line
    local value

    while IFS= read -r file; do
        while IFS= read -r line; do
            [ -n "$line" ] || continue
            value="$(blocks_percentage_from_line "$line")"
            if [ -n "$value" ]; then
                total="$(awk -v sum="$total" -v current="$value" 'BEGIN { printf "%.2f", sum + current }')"
                count=$((count + 1))
            fi
        done < <(extract_function_lines "$file")
    done < <(list_gcov_files)

    if [ "$count" -eq 0 ]; then
        echo "0.00"
        return
    fi

    awk -v total="$total" -v count="$count" 'BEGIN { printf "%.2f", total / count }'
}

main() {
    if [ ! -d "$REPORTS_DIR" ]; then
        echo "Reports directory not found: $REPORTS_DIR" >&2
        exit 1
    fi

    local gcov_files
    gcov_files="$(list_gcov_files)"
    if [ -z "$gcov_files" ]; then
        echo "No .gcov files found in $REPORTS_DIR" >&2
        exit 1
    fi

    : > "$OUTPUT_FILE"

    while IFS= read -r file; do
        write_file_summary "$file"
    done < <(list_gcov_files)

    local mean
    mean="$(compute_mean_percentage)"
    printf 'Mean %%: %s\n' "$mean" >> "$OUTPUT_FILE"

    if awk -v mean="$mean" 'BEGIN { exit !(mean >= 90) }'; then
        exit 0
    fi

    exit 1
}

main "$@"
