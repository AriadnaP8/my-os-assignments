#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void listRec(const char *path, int size, char * name)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                if(statbuf.st_size > size)
                {
                    printf("%s\n", fullPath);
                }

                if(strcmp(entry->d_name, name) == 0)
                {
                    printf("%s\n", fullPath);
                }

                if (S_ISDIR(statbuf.st_mode))
                {
                    listRec(fullPath, size, name);
                }
                
            }
        }
    }
    closedir(dir);
}

void listNeRec(const char *path, int size, char * name)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                if(statbuf.st_size > size)
                {
                    printf("%s\n", fullPath);
                }

                if(strcmp(entry->d_name, name) == 0) 
                {
                   printf("%s\n", fullPath);
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    int ok = 0, ok1 = 0, size = 0;//, ok2 = 0, ok3 = 0, ok4 = 0;
    char s[101], name[101];
    //name = NULL;
    //strcpy(name,"");
    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("85866\n");
        }
    }
    // incepem parcurgerea de la 1 pentru ca pe pozitia 0 este numele fisierului
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "list") == 0)
        {
            ok = 1;
        }
        if(strcmp(argv[i], "recursive") == 0)
        {
            ok1 = 1;
        }
        if(strcmp(argv[i], "filtering_options") == 0)
        {
            //ok2 = 1;
        }
        if(strncmp(argv[i], "path=", 5) == 0)
        {
            //ok3 = 1;
            strcpy(s, argv[i] + 5);
        }
        if(strncmp(argv[i], "size_greater=", 13) == 0) 
            sscanf(argv[i] + 13, "%d", &size);
        
        if(strncmp(argv[i], "name_starts_with=", 17) == 0) 
        {
            sscanf(argv[i] + 17, "%s", name);
        }
        
    }
    if(ok == 1)
    {
        if(ok1 == 1) 
        {
            printf("SUCCESS\n");
            listRec(s, size, name);
        }
        else
        {
            printf("SUCCESS\n");
            listNeRec(s, size, name);
        }
    }
    return 0;
}