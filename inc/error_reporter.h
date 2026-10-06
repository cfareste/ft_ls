#pragma once

void report_opening_directory_error(const char *directory_path);
void report_reading_directory_error(const char *directory_path);
void report_closing_directory_error(const char *directory_path);
void report_access_file_error(const char *file_path);
void report_read_symbolic_link_error(const char *file_path);
