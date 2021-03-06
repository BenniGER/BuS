#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stud_type_ {                  /* Struktur des Datensatzes:    */
    int matnum;                              /* Matrikelnummer, Vor-         */
    char vorname[20];                        /* und Nachname sind Eintraege. */
    char nachname[20];                       /* Die Datenbank ist eine       */
    struct stud_type_ *next_student;         /* einfach verkettete Liste     */
} stud_type;


/* Ist die Datenbank leer?      */
bool is_empty(stud_type** liste) {
    return *liste == NULL;
}

/* Einfuegen eines Elementes    
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt MatNr, Vorname und Nachname des Studenten der Eingef�gt werden soll
 *
 */
void enqueue(stud_type** studenten_liste, int matnum, char vorname[20], char nachname[20])
{
    /* Deklariere ben�tigte Variablen */
	stud_type* new_student;
	stud_type* pcur;
    	stud_type* previous;
    /* Hol Speicher auf dem Heap an f�r den neuen Listen Eintrag */
	new_student = malloc(sizeof(stud_type));
    /* Bef�ll den Speicher */
	new_student->matnum = matnum;
	strcpy(new_student->vorname, vorname);
	strcpy(new_student->nachname, nachname);
    /* F�g den neuen Eintrag in die Liste ein */
    /* Ist die Liste leer ? */
	if (is_empty(studenten_liste)){
		*studenten_liste = new_student;
     }
    /* Sortier den Studenten aufsteigend nach Matrikelnummer ein */
	else {
		pcur = *studenten_liste;
        while(pcur != NULL && pcur->matnum < matnum) {
            previous = pcur;
            pcur = pcur->next_student;
		}
		previous->next_student = new_student;
        new_student->next_student = pcur;
	}
}

stud_type* sort_liste(void (*functionPtr)(stud_type* list, stud_type** sorted_list), stud_type* list) {
	stud_type* sorted_list = NULL;	
	(*functionPtr)(list, &sorted_list);
	return sorted_list;
}

void sort_vorname(stud_type* list, stud_type** sorted_list) {
	
	stud_type* pcur;
	stud_type* scur;
    	stud_type* previous;
	char name_p[20];
	char name_s[20];

	pcur = list;
        while(pcur != NULL) {		
		if(is_empty(sorted_list)) {			
			*sorted_list = pcur;
			pcur = pcur->next_student;		
			continue;
		}
			
		scur = *sorted_list;	
				
		strcpy(name_p, pcur->vorname);
		strcpy(name_s, scur->vorname);
		while(scur != NULL && name_p[0] >= name_s[0]) {
		    previous = scur;
		    scur = scur->next_student;
		}
	
		previous->next_student = pcur;
        	previous->next_student->next_student = scur;
			
		pcur = pcur->next_student;
	}
}	

void sort_nachname(stud_type* list, stud_type** sorted_list) {
	stud_type* pcur;
	stud_type* scur;
    	stud_type* previous;
	char name_p[20];
	char name_s[20];

	pcur = list;
        while(pcur != NULL) {		
		if(is_empty(sorted_list)) {			
			*sorted_list = pcur;
			pcur = pcur->next_student;			
			continue;
		}
			
		scur = *sorted_list;	
				
		strcpy(name_p, pcur->nachname);
		strcpy(name_s, scur->nachname);

		while(scur != NULL && name_p[0] >= name_s[0]) {
		    previous = scur;
		    scur = scur->next_student;
		}
	
		previous->next_student = pcur;
        	previous->next_student->next_student = scur;
			
		pcur = pcur->next_student;
	}
}

/* L�schen eines Elementes. 
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt die MatNr des Studenten der zu l�schen ist
 *
 * Gibt 0 f�r einen Fehler zur�ck
 * Gibt 1 f�r Erfolg zur�ck
 */
int dequeue(stud_type** studenten_liste, int matnum)
{
    /* Deklariere ben�tigte Variablen -> m*/
    stud_type* pcur;
    stud_type* previous;
    /* Pr�fe Randbedingungen */
	if(is_empty(studenten_liste)) return 0;
    /* Finde den Studenten */
    /* Was muss passieren wenn das 1. Element gel�scht wird? */
	pcur = *studenten_liste;
	if((*studenten_liste)->matnum == matnum){
		(*studenten_liste) = (*studenten_liste)->next_student;
		free(pcur);
		return 1;
	}
	
	while(pcur != NULL && pcur->matnum != matnum){
		previous = pcur;
        pcur = pcur->next_student;
	}
	
	if(pcur == NULL) {
        return 0;
    }
    /* L�sche den Studenten und gibt den Speicher frei */
	if(pcur->matnum == matnum){
		previous->next_student = pcur->next_student;
		free(pcur);
		return 1;
	}
    
    /* Was ist wenn es nicht in der Liste ist? */
	else return 0;
    /* ... */
    
}

/* Auslesen eines Elementes 
 *
 * Bekommt zeiger auf den Listenstart
 * Bekommt matnr des Studenten der ausgelesen werden soll
 *
 * Schreibt Vorname und Nachname in vorname und nachname
 */
int get_student(stud_type* studenten_liste, int matnum, char vorname[20], char nachname[20])
{
    stud_type *curr;

    /* Durchmustern der DB */
    curr = studenten_liste;
    while ((curr != NULL) && (curr->matnum < matnum)) {
        curr = curr->next_student;
    }

    if ((curr == NULL) || (curr->matnum != matnum)) {
        /* R�ckgabewert: Fehler */
        return 0;
    } else {
       strncpy(vorname, curr->vorname, 20);
       strncpy(nachname, curr->nachname, 20);
        /* R�ckgabewert: OK */
       return 1;
   }
}


/* Test der Listenfunktionen  */
int main(void)                                   
{
    /* Initialisierung der Datenbank */
    stud_type *studenten_liste = NULL;
    /* platz f�r vorname */
    char vorname[20];
    /* platz f�r nachname */
    char nachname[20];
    /* zeiger f�r iteration */
    stud_type *curr;

    printf(">>> Fuege neuen Studenten in die Liste ein: Eddard Stark [1234] ...\n");
    enqueue(&studenten_liste, 1234, "Eddard", "Stark");
    printf(">>> Fuege neuen Studenten in die Liste ein: Jon Snow [5678] ...\n");
    enqueue(&studenten_liste, 5678, "Jon", "Snow");
    printf(">>> Fuege neuen Studenten in die Liste ein: Tyrion Lannister [9999] ...\n");
    enqueue(&studenten_liste, 9999, "Tyrion", "Lannister");
    printf(">>> Test, ob die Matrikelnummer 1289 bereits erfasst wurde ...\n");

    if(get_student(studenten_liste, 815, vorname, nachname)) {
        printf("    Matrikelnummer %4i: %s %s\n", 1289, vorname, nachname);
    } else {
        printf("    Matrikelnummer %4i ist unbekannt\n", 1289);
    }

    printf(">>> Fuege neuen Studenten in die Liste ein: Daenerys Targaryen [1289] ...\n");
    enqueue(&studenten_liste, 1289, "Daenerys", "Targaryen");
    printf(">>> Loesche die Matrikelnummer 1234 ...\n");

    if(dequeue(&studenten_liste, 1234)) {
        printf("    Matrikelnummer %4i geloescht\n", 1234);
    } else {
        printf("    Matrikelnummer %4i war nicht erfasst\n", 1234);
    }

    printf(">>> Test ob die Studentenliste leer ist ...\n");

    if(is_empty(&studenten_liste)) {
        printf("    Die Studentenliste ist leer \n");
    } else {
        printf("    Die Studentenliste ist nicht leer \n");
    }

    printf(">>> Test, ob die Matrikelnummer 5678 bereits erfasst wurde ...\n");

    if(get_student(studenten_liste, 5678, vorname, nachname)) {
        printf("    Matrikelnummer %4i: %s %s\n", 5678, vorname, nachname);
    } else {
        printf("    Matrikelnummer %4i ist unbekannt\n", 5678);
    }
 
    printf(">>> Loesche die Matrikelnummer 9998 ...\n");
 
    if(dequeue(&studenten_liste, 9998)) {
        printf("    Matrikelnummer %4i geloescht\n", 9998);
    } else {
        printf("    Matrikelnummer %4i war nicht erfasst\n", 9998);
    }

    printf(">>> Loesche die Matrikelnummer 5678 ...\n");

    if(dequeue(&studenten_liste, 5678)) {
        printf("    Matrikelnummer %4i geloescht\n", 5678);
    } else {
        printf("    Matrikelnummer %4i war nicht erfasst\n", 5678);
    }

    printf(">>> Gebe alle erfassten Studenten aus ...\n");
    curr = studenten_liste;

    while(curr != NULL) {
        printf("    Matrikelnummer %4i: %s %s\n", curr->matnum, curr->vorname, curr->nachname);
        curr = curr->next_student;
    }
    
//Sort Parts
    void (*sortPtr)(stud_type* list, stud_type** sorted_list);
    stud_type* sortierte_liste = NULL;
    sortPtr = &sort_vorname;
    sortierte_liste = sort_liste(sortPtr, studenten_liste);
    
    printf(">>> Gebe alle erfassten Studenten aus ...\n");
    curr = sortierte_liste;

    while(curr != NULL) {
        printf("    Matrikelnummer %4i: %s %s\n", curr->matnum, curr->vorname, curr->nachname);
        curr = curr->next_student;
    }
    return 0;
}
