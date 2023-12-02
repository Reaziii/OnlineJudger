#include<stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "utils/config.h"
#include <stdio.h>
#include <sys/resource.h>
void run(struct Config *config){
    FILE *file;
    char *args[] = {"/bin/ls", "-l", NULL};
    char *envs[] = {NULL};
    struct rlimit limit;

    struct rlimit max_memory;
    max_memory.rlim_cur = max_memory.rlim_max = (rlim_t) (config->memory_limit) * 2000;
    if (setrlimit(RLIMIT_AS, &max_memory) != 0 && setrlimit(RLIMIT_STACK, &max_memory) != 0) {
        perror("SETLIMIT [MEMORY]");
        exit(SYSTEM_ERROR);
    }


    limit.rlim_cur = limit.rlim_max = (rlim_t) (config->time_limit + 1000)/1000;
    if(setrlimit(RLIMIT_CPU, &limit)!=0){
        perror("SETLIMIT ERROR [TIME]");
        exit(SYSTEM_ERROR);
    }
    if(config->input_path!=NULL){
        file = freopen(config->input_path, "r", stdin);
        if(file==NULL){
            exit(SYSTEM_ERROR);
        }
    }
    if(config->input_path!=NULL){
        file = freopen(config->output_path, "w", stdout);
        if(file==NULL){
            exit(SYSTEM_ERROR);
        }
    }
    if(config->input_path!=NULL){
        file = freopen(config->error_path, "w", stderr);
        if(file==NULL){
            exit(SYSTEM_ERROR);
        }
    }
    execve(config->exe_path, args, envs);
    exit(SYSTEM_ERROR);

}