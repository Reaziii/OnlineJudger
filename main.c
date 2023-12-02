#include <stdio.h>
#include "utils/utils.h"
#include <signal.h>
#include <unistd.h>
#include "utils/config.h"
#include <stdlib.h>
#include "runner.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "time.h"

struct Config config;
struct Result result;

void resetResult(){
    result.result = result.signal = result.error = result.time = result.memory = 0;
}




int main(int argc, char *argv[]) {
    resetResult();
    config.exe_path = "/Users/reaz/CLionProjects/Judgerv2/test/test";
    config.memory_limit = 13000;
    config.time_limit = 1000;
    config.error_path = "/Users/reaz/CLionProjects/Judgerv2/test/error.txt";
    config.input_path = "/Users/reaz/CLionProjects/Judgerv2/test/input.txt";
    config.output_path = "/Users/reaz/CLionProjects/Judgerv2/test/output.txt";
    config.type = "cpp";
    pid_t pid = fork();

    struct timeval start, end;
    gettimeofday(&start, NULL);
    if(pid==-1){
        //failed
        exit(SYSTEM_ERROR);
    }
    else if(pid==0){
        run(&config);
        exit(SYSTEM_ERROR);
    }
    else{
        int status;
        struct rusage resource;
        if(wait4(pid, &status, 0x00000008, &resource) == -1){
            exit(SYSTEM_ERROR);
        }
        gettimeofday(&end, NULL);

        // checking termination without complete
        if(WIFSIGNALED(status)){
            result.signal = WTERMSIG(status);
        }
        //checking if the error is for me
        if(result.signal==SIGUSR1){
            result.result = SYSTEM_ERROR;
        }
        else{
            printf("status = %d\n", status);
            result.exit_code = WEXITSTATUS(status);
            result.time = (int)(resource.ru_utime.tv_sec * 1000 + resource.ru_utime.tv_usec/1000);
            int realtime = (int)(end.tv_sec * 1000 + end.tv_usec/1000 - start.tv_sec*1000 - start.tv_usec/1000);
            result.memory = (int) resource.ru_maxrss;
            if(result.exit_code!=0){
                result.result = RUNTIME_ERROR;
            }

            if(result.signal == SIGSEGV){
                // memory limit or runtime error
                if(result.memory > config.memory_limit){
                    result.result = MEMORY_LIMIT_EXCEEDED;
                }
                else{
                    result.result = RUNTIME_ERROR;
                }
            }
            else{
                // NORMAL TEST
                if(result.memory>config.memory_limit){
                    result.result = MEMORY_LIMIT_EXCEEDED;
                }
                else if(result.time > config.time_limit){
                    result.result = TIME_LIMIT_EXCEEDED;
                }
                else if(result.signal != 0){
                    result.result = RUNTIME_ERROR;
                }

            }

        }




    }



    printf("{\n"
           "    \"time\": %d,\n"
           "    \"memory\": %d,\n"
           "    \"signal\": %d,\n"
           "    \"exit_code\": %d,\n"
           "    \"error\": %d,\n"
           "    \"result\": %d,\n"
           "}",
           result.time,
           result.memory,
           result.signal,
           result.exit_code,
           result.error,
           result.result);





    return 0;
}
