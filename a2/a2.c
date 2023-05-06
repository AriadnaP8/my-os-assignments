#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "a2_helper.h"

#define n 40

typedef struct {
    int id;
    int proces;
}threaduri;

sem_t semafor[n];   // vector de semafoare pentru sincronizarea thread-urilor

void *thread_function(void *arg)
{
    
    threaduri* thread = (threaduri*) arg;

    if(thread->proces == 9)     // veridicam daca procesul in care ne aflam e 9
    {
        if(thread->id == 3) // verif daca thread-ul curent e 3
        {
            info(BEGIN, thread->proces, thread->id);   
            sem_post(&semafor[5]);  // crestem val sem
            sem_wait(&semafor[4]);  // asteptam ca 4 
            info(END, thread->proces, thread->id);
        }
        else 
            if(thread->id == 5)
            {
                sem_wait(&semafor[5]);  // asteapta semnalul de la thread-ul 3 ca sa inceapa sa execute
                info(BEGIN, thread->proces, thread->id);   
                info(END, thread->proces, thread->id);
                sem_post(&semafor[4]);  // deblocam semaforul 4
            }
        else 
            {
                // asteptam semnalul de la thread-ul 5 pentru a incepe executia
                info(BEGIN, thread->proces, thread->id);   
                info(END, thread->proces, thread->id);
                //sem_wait(&semafor[4]);  // asteapta semnaal de la thread-ul 5 pentru a incepe executia
            }
    }
    else
    {
        // restul proceselor care vor fi executate
        info(BEGIN, thread->proces, thread->id);   
        info(END, thread->proces, thread->id);
    }

    return NULL;
}


int main(int argc, char **argv)
{
    init();

    info(BEGIN, 1, 0);  // incepe procesul 1
    
    pid_t pid2;
    pid_t pid3;
    pid_t pid4;
    pid_t pid5;
    pid_t pid6;
    pid_t pid7;
    pid_t pid8;
    pid_t pid9;
    
    pthread_t tid[n];
    threaduri thread_vect[n + 1];   // pentru a acoperit toate thread-urile

    // initiallizam semafoarele
    for(int i = 0; i < n; i++)
    {
        sem_init(&semafor[i], 0, 0); // vom initializa cu 0 semafoarele(param3), initial fiind blocat
        // (param2) va fi 0 tocmai pentru a rezolva cerinta si anume pentru a sincroniza thread-urile in acelasi proces
    }

    pid2 = fork();
    if(pid2 == 0) {
        info(BEGIN, 2, 0);  // incepe procesul 2

        info(END, 2, 0);    // se termina procesul 2
        wait(NULL);
        exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
    }

    pid3 = fork();
    if(pid3 == 0) {
        info(BEGIN, 3, 0);  // incepe procesul 3
        // creez cele 6 thread-uri din cerinta 5
        for(int i = 1; i <= 6; i++)
        {
            thread_vect[i].id = i;
            thread_vect[i].proces = 3;
            pthread_create(&tid[i], NULL, thread_function, &thread_vect[i]);
        }
        for(int i = 1; i <= 6; i++)
        {
            pthread_join(tid[i], NULL);
        }
        pid4 = fork();
        if(pid4 == 0) {
            info(BEGIN, 4, 0);  // incepe procesul 4
            // creez cele 35 de thread-uri din cerinta 4
            for(int i = 1; i <= 35; i++)
            {
                thread_vect[i].id = i;
                thread_vect[i].proces = 4;
                pthread_create(&tid[i], NULL, thread_function, &thread_vect[i]);
            }
            for(int i = 1; i <= 35; i++)
            {
                pthread_join(tid[i], NULL);
            }
            pid7 = fork();
            if(pid7 == 0) {
                info(BEGIN, 7, 0);  // incepe procesul 7

                info(END, 7, 0);    // se termina procesul 7
                wait(NULL);
                exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
            }

            
            waitpid(pid4, NULL, 0);     // asteapta dupa procesul 8
            info(END, 4, 0);    // se termina procesul 4
            exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
        }

        pid8 = fork();
        if(pid8 == 0) {
            info(BEGIN, 8, 0);  // incepe procesul 8

            wait(NULL);
            info(END, 8, 0);    // se termina procesul 8
            exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
        }

        waitpid(pid4, NULL, 0);     // asteapta dupa procesul 4
        waitpid(pid8, NULL, 0);     // asteapta dupa procesul 8
        info(END, 3, 0);    // se termina procesul 3
        exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
    }

    pid5 = fork();
    if(pid5 == 0) {
        info(BEGIN, 5, 0);  // incepe procesul 5

        wait(NULL);
        info(END, 5, 0);    // se termina procesul 5
        exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
    }
    
    pid6 = fork();
    if(pid6 == 0) {
        info(BEGIN, 6, 0);  // incepe procesul 6

        wait(NULL);
        info(END, 6, 0);    // se termina procesul 6
        exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
    }

    pid9 = fork();
    if(pid9 == 0) {
        info(BEGIN, 9, 0);  // incepe procesul 9
        // creez cele 5 thread-uri din cerinta 3
        for(int i = 1; i <= 5; i++)
        {
            thread_vect[i].id = i;
            thread_vect[i].proces = 9;
            pthread_create(&tid[i], NULL, thread_function, &thread_vect[i]);
        }
        for(int i = 1; i <= 5; i++)
        {
            pthread_join(tid[i], NULL);
        }

        wait(NULL);
        info(END, 9, 0);    // se termina procesul 9
        exit(0); // ca sa iasa din proces, sa nu se continue in urmatorul
    }

    waitpid(pid2, NULL, 0);     // asteapta dupa procesul 2
    waitpid(pid3, NULL, 0);     // asteapta dupa procesul 5
    waitpid(pid5, NULL, 0);     // asteapta dupa procesul 5
    waitpid(pid6, NULL, 0);     // asteapta dupa procesul 6
    waitpid(pid9, NULL, 0);     // asteapta dupa procesul 9

    sem_destroy(&semafor[4]); // distrugem semaforul corespunzator procesului 1
    sem_destroy(&semafor[5]); // distrugem semaforul corespunzator procesului 1

    info(END, 1, 0);    // se termina procesul 1
    return 0;
}