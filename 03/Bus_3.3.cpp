// Bus_3.3.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#define MAXQUANTUM 13;
#define MINQUANTUM 1;

struct prozess {
	int bearbeitungszeit;
	int endtime;
};

int main()
{
	int dur[8] = {6, 13, 7, 3, 4, 9, 10, 11};
	struct prozess list[8];

	int counter;
	int time;
	int quantum;

	printf("Quantum  P1  P2  P3  P4  P5  P6  P7  P8\n");
	printf("---------------------------------------\n");
	
	for(int q=MINQUANTUM; q<MAXQUANTUM+1; q++) {

		for(int i=0; i<8; i++) {
			list[i].endtime = 0;
			list[i].bearbeitungszeit = dur[i];
		}

		counter = 0;
		time = 0;
		quantum = q;

		while(counter != 8) {
			for(int j=0; j<8; j++)
			{
				for(int k=0; k<quantum; k++) {
					if(list[j].bearbeitungszeit > 0) {
						list[j].bearbeitungszeit--;
						time++;				
					}

					if(list[j].endtime == 0 && list[j].bearbeitungszeit == 0) {
							list[j].endtime = time;
							counter++;
					}
				}
			}
		}

		printf("%i        ", quantum);
		
		for(int i=0; i<8; i++) {	
			 printf("%i  ", list[i].endtime);
		}
		printf("\n");
	}

	getchar();
	return 0;
}

