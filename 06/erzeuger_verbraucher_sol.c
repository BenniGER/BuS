/* Erzeuger-Verbraucher-Problem mit Ringpuffer & Semaphoren */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 100		/* 100 Zeichen Puffer		 	   */

char 	*ring;			/* Zeiger auf Ringpuffer		   */
int 	*in,*out;		/* Zeiger auf in/out-Indizes		   */
int 	semset;			/* Semaphorenset-Identifier		   */


void sem_init(int semnr, int value) {
    /* Init-Operation auf Semaphore semnr             			   */
    int erg;

    erg = semctl(semset, semnr, SETVAL, value);
}

void sem_signal(int semnr) {
    /* Signal-Operation auf Semaphore semnr				    */
    struct sembuf semops;
    int erg;

    semops.sem_num = semnr;	/* Semaphorennummer			    */
    semops.sem_op = 1;		/* Wert um 1 erhöhen			    */
    semops.sem_flg = 0;		/* keine besonderen Flags		    */

    erg = semop(semset, &semops, 1);	/* Operation ausführen			    */   
}

void sem_wait(int semnr) {
/* Wait-Operation auf Semaphore semnr im Semaphorenset semsetID		    */
    struct sembuf semops;
    int erg;

    semops.sem_num = semnr;	/* Semaphorennummer			    */
    semops.sem_op = -1;		/* Versuch den Wert um 1 zu verringern	    */
    semops.sem_flg = 0;		/* keine speziellen Flags		    */

    erg = semop(semset, &semops, 1);	/* Operation ausführen 			    */   
}


void write_element(char element, int semnr) {
	sem_wait(semnr);	/* auf anderen Erzeuger warten wenn dieser gerade aktiv ist, ansonsten selbst aktiv werden */
	
    while ((*in + 1) % BUFSIZE == *out); /* busy waiting				    */
    ring[*in] = element;	  	 /* Element auslesen			    */
    *in = (*in + 1) % BUFSIZE;	 /* in aktualisieren	*/

	sem_signal(semnr);	/* Erzeugenprozess beendet --> kritischen Bereich freigeben für nächsten Erzeuger */
}

char read_element(int semnr) {
    char element;

    sem_wait(semnr);		  /* WAIT 				    */

    while (*in == *out);		  /* busy waiting			    */
    element = ring[*out];		  /* Element auslesen			    */
    *out = (*out + 1) % BUFSIZE;	  /* out aktualisieren			    */

    sem_signal(semnr);		  /* SIGNAL 				    */	
    return element;
}


void erzeuger(int anzahl) {	  /* "produziert" anzahl Elemente 	    */
    int i;

    for(i = 1; i <= anzahl; i++) {	  /* Elemente "produzieren" 		    */
        write_element('A', 1);	/* erweitert um die zweite Semaphorennummer, zum synchronisieren der beiden Erzeuger */
        printf("Erzeuger     : %d Elemente produziert ...\n", i);
        fflush(stdout);
    }
}

void verbraucher(int nr) { 
/* "verbraucht" moeglichst viele Elemente und zaehlt diese  		    */
    int i;
    char c;

    i = 0;
   
    while (1) {			  /* Elemente "verbrauchen" 		    */
        c = read_element(0);
        i++;
        printf("Verbraucher %d: %d Elemente konsumiert ...\n",nr,i);
        fflush(stdout);
    }
}


void main(int argc, char *argv[]) {
    int	child;			  /* PID des Sohnprozesses		    */
    int shmID;			  /* shared memory ID 			    */

                                      /* shared memory anfordern 		    */
    shmID = shmget(IPC_PRIVATE, BUFSIZE + 2 * sizeof(int), IPC_CREAT | 0x1ff);

   				  /* Zeiger auf Ringpuffer initialisieren   */
    ring = (char *)shmat(shmID, NULL, 0); 
    in = (int *)(ring + 100);	  /* Pointer auf in-Index initialisieren    */ 
    out = in + 1;			  /* Pointer auf out-Index initialisieren   */

   				  /* Sem.set mit einer Semaphore anfordern  */
    semset = semget(IPC_PRIVATE, 1, IPC_CREAT | 0x1ff);

    sem_init(0,1);	 	  /* Semaphore 0 mit 1 initialisieren 	    */
	sem_init(1,1);
    *in = 0;			  /* Indizes initialisieren 		    */
    *out = 0;

    child = fork();		  /* Sohnprozess starten 		    */
    if (child == 0)	{	  /* Bin ich der Sohnprozess? 		    */
        erzeuger(120000);
							/* jeder Erzeuger hat einen verbraucher */
		verbraucher(2);		
    } else {
		erzeuger(4000);
        verbraucher(1);           
        
	}
}
