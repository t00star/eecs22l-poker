/*********************************************************************/
/* Poker Project, for EECS 22L, Spring 2024                   		 */
/* unit_test_client.c: Unit test for client			                 */
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT_NUMBER_LOWER_BOUND 1024

void getPortNum(int argc, char *const *argv, int* portNum);

void test_getPortNum_valid_port() {
    int portNum;
    char *args[] = {"program", "hostname", "10080"};
    getPortNum(3, args, &portNum);
    assert(portNum == 10080);
    printf("test_getPortNum_valid_port passed.\n");
}

void test_getPortNum_invalid_port() {
    int portNum;
    char *args[] = {"program", "hostname", "1023"};
    if (fork() == 0) {
        /* In child process */
        getPortNum(3, args, &portNum);
        /* If exit is not called, test failed */
        assert(0);
    } else {
        /* In parent process, wait for child */
        int status;
        wait(&status);
        /* Check if child exited with EXIT_FAILURE */
        assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);
        printf("test_getPortNum_invalid_port passed.\n");
    }
}

void test_getPortNum_missing_args() {
    int portNum;
    char *args[] = {"program", "hostname"};
    if (fork() == 0) {
        /* In child process */
        getPortNum(2, args, &portNum);
        /* If exit is not called, test failed */
        assert(0);
    } else {
        /* In parent process, wait for child */
        int status;
        wait(&status);
        /* Check if child exited with EXIT_FAILURE */
        assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);
        printf("test_getPortNum_missing_args passed.\n");
    }
}

int main() {
    test_getPortNum_valid_port();
    test_getPortNum_invalid_port();
    test_getPortNum_missing_args();

    printf("All tests passed.\n");
    return 0;
}
