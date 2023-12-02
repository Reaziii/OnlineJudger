#pragma once

struct Config{
    int time_limit;
    int memory_limit;
    char *exe_path;
    char *type;
    char *input_path;
    char *output_path;
    char *error_path;
};

enum {
    TIME_LIMIT_EXCEEDED = 1,
    MEMORY_LIMIT_EXCEEDED = 2,
    RUNTIME_ERROR = 3,
    COMPILATION_ERROR = 4,
    WRONG_ANSWER = 6,
    SYSTEM_ERROR = -1
};

struct Result{
    int time;
    int memory;
    int signal;
    int exit_code;
    int error;
    int result;
};