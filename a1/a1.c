#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>


void listRec(const char *path, int size, char * name, int are_num, int *OK)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        printf("ERROR\ninvalid directory path\n");
        return;
    }
    
    if(*OK == 1) 
    {
        *OK = 2;
        printf("SUCCESS\n");
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                // daca s-a introdus un size afisam
                if((statbuf.st_size > size && size != -8))
                {
                    // daca avem numa introdus, comparam ca sa vedem daca numele fisierului incepe cu ceea ce s a introdus
                    if(strncmp(entry->d_name, name, strlen(name)) == 0 && are_num == 0)
                        printf("%s\n", fullPath);
                    else 
                    if( are_num == 1)   // daca nu s a introdus nume, el respecta conditia de size deci afisam
                        printf("%s\n", fullPath);
                }
                else 
                if( size == -8) // daca nu s a introdus size ul
                {
                    // comparam iar numele si afisam la fel ca mai sus
                    if(strncmp(entry->d_name, name, strlen(name)) == 0 && are_num == 0)
                        printf("%s\n", fullPath);
                    else 
                    if( are_num == 1)
                        printf("%s\n", fullPath);
                }


                if (S_ISDIR(statbuf.st_mode))
                {
                    listRec(fullPath, size, name, are_num, OK);
                }
                
            }
        }
    }
    closedir(dir);
}

void listNeRec(const char *path, int size, char * name, int are_num)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        printf("ERROR\ninvalid directory path\n");
        return;
    }

    printf("SUCCESS\n");
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {

                // daca s-a introdus un size afisam
                if((statbuf.st_size > size && size != -8))
                {
                    // daca avem numa introdus, comparam ca sa vedem daca numele fisierului incepe cu ceea ce s a introdus
                    if(strncmp(entry->d_name, name, strlen(name)) == 0 && are_num == 0)
                        printf("%s\n", fullPath);
                    else 
                    if( are_num == 1)   // daca nu s a introdus nume, el respecta conditia de size deci afisam
                        printf("%s\n", fullPath);
                }
                else 
                if( size == -8) // daca nu s a introdus size ul
                {
                    // comparam iar numele si afisam la fel ca mai sus
                    if(strncmp(entry->d_name, name, strlen(name)) == 0 && are_num == 0)
                        printf("%s\n", fullPath);
                    else 
                    if( are_num == 1)
                        printf("%s\n", fullPath);
                }
            }
        }
    }
    closedir(dir);
}

void parsat(char *path)
{
    off_t fd;
    fd = open(path,O_RDONLY);
    if(fd == -1)
    {
        perror("Nu s-a putut deschide fisierul.");
        close(fd);
        return;
    }
    lseek(fd, 0, SEEK_SET);
    // numarul magic
    char magic = 0;
    if(read(fd, &magic, 1) != 1 || magic != '8')
    {
        printf("ERROR\nwrong magic\n");
        close(fd);
        return;
    }
    int headerSize = 0;
    read(fd, &headerSize, 2);
    // versiunea
    int version = 0;
    if(read(fd, &version, 1) != 1 || (version < 109 || version > 213))
    {
        printf("ERROR\nwrong version\n");
        close(fd);
        return;
    }
    // numarul de sectiuni
    int nbOfSections = 0;
    if(read(fd, &nbOfSections, 1) != 1 || (nbOfSections < 3 || nbOfSections > 18))
    {
        printf("ERROR\nwrong sect_nr\n");
        close(fd);
        return;
    }

    struct s
    {
        char name[21];
        int type, offset, size;
    };
    struct s v[nbOfSections+1];
    for(int i = 1; i <= nbOfSections; i++)  //initializam vectorul de tipul structurii s cu 0
    {
        v[i].type=0;
        v[i].offset=0;
        v[i].size=0;
    }
    for(int i = 1; i <= nbOfSections; i++)  //parcurgem si citim pe rand in functie de conditii
    {
        read(fd, v[i].name, 20);
        v[i].name[20]='\0'; // am pus asta ca sa nu citeasca caractere random
        read(fd, &v[i].type, 1);
        if(v[i].type != 61 && v[i].type != 65 && v[i].type != 69 && v[i].type != 58)
        {
            printf("ERROR\nwrong sect_types\n");
            close(fd);
            return;
        }
        read(fd, &v[i].offset, 4);
        read(fd, &v[i].size, 4);
    }
    // afisam sub formatul cerut 
    printf("SUCCESS\nversion=%d\nnr_sections=%d\n", version, nbOfSections);
    for(int i = 1; i <= nbOfSections; i++)
    {
        printf("section%d: %s %d %d\n", i, v[i].name, v[i].type, v[i].size);
    }

    close(fd);
}

int main(int argc, char **argv)
{
    int ok = 0, ok1 = 0, size = -8, are_num = 1, ok_parsare = 0;
    char s[101], name[101];
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
        // verificam care dintre optiuni au fost scrise in terminal
        if(strcmp(argv[i], "list") == 0)
        {
            ok = 1;
        }
        if(strcmp(argv[i], "recursive") == 0)
        {
            ok1 = 1;
        }
        if(strncmp(argv[i], "path=", 5) == 0)
        {
            strcpy(s, argv[i] + 5);
        }
        if(strncmp(argv[i], "size_greater=", 13) == 0) 
            sscanf(argv[i] + 13, "%d", &size);
        
        if(strncmp(argv[i], "name_starts_with=", 17) == 0) 
        {
            are_num = 0;
            sscanf(argv[i] + 17, "%s", name);
        }

        if(strcmp(argv[i], "parse") == 0)
        {
            ok_parsare = 1;

        }
    }

    int OK = 1; // asta e pentru a afisa SUCCES o singura data in functia recursiva
    if(ok == 1)
    {
        if(ok1 == 1) 
        {
            listRec(s, size, name, are_num, &OK);   // o transmitem aici ca parametru la functie
        }
        else
        {
            listNeRec(s, size, name, are_num);
        }
    }
    // daca s a introdus in terminal conditia de parsare, apelam functia pentru afisarile necesare
    if(ok_parsare != 0) parsat(s);
    return 0;
}