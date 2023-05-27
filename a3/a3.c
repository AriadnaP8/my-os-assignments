#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define pipe2 "RESP_PIPE_85866"
#define pipe1 "REQ_PIPE_85866"

int main(int argc, char **argv)
{
    unlink("RESP_PIPE_85866"); 
    int fd1 = -1, fd2 = -1;
    //creez pipe2
    if(mkfifo(pipe2, 0600) != 0){
        perror("ERROR\ncannot create the response pipe\n");
        return -1;
    }

    // deschidem in citire primul pipe
    fd1 = open(pipe1, O_RDONLY);
    fd2 = open(pipe2, O_WRONLY);

    char cuvant[10] = "BEGIN", lungime = 5;
    write(fd2, &lungime, 1);    // tre sa declar lungimea pentru string fields
    write(fd2, &cuvant, lungime);

   /// read(fd1, &cuvant, lungime);
    printf("SUCCESS\n");
    //else printf("ERROR\ncannot create the response pipe | cannot open the request pipe\n");
    
    char lungimeEcho = 4, lungimeVariant = 7;
    char request[10];
    while (1) {
        read(fd1,&lungime,1);
        read(fd1, &request, lungimeEcho);
        //for(int i =0; i<99; i++) request[i] = 0;

        if (strcmp(request, "ECHO") == 0) {
            char mesaj[30] = "ECHO";
            int lg = 85866;
            char mesaj2 [30]= "VARIANT";
            write(fd2, &lungimeEcho, 1);  
            write(fd2, &mesaj, lungimeEcho);
            write(fd2, &lg, 4);
            write(fd2, &lungimeVariant, 1);  
            write(fd2, &mesaj2, lungimeVariant);
        }
        else
        if(strcmp(request, "EXIT") == 0)
    	{
    		close(fd1);
    		close(fd2);
    		unlink("RESP_PIPE_85866");
    		return 0;
    		break;
    	}
        else 
        if (strcmp(request, "CREATE_SHM") == 0) {
            int lungimeCreate = 10;
            char mesaj[30] = "CREATE_SHM";
            write(fd2, &lungimeCreate, 1);  
            write(fd2, &mesaj, lungimeCreate);
        }else break;
    }

    close(fd1);
    close(fd2);

    unlink("RESP_PIPE_85866"); 
    return 0;
}