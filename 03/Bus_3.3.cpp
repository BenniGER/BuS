#include <stdio.h>

int MAXQUANTUM = 13;
int MINQUANTUM = 1;

struct prozess {
	int bearbeitungszeit;
	int endtime;
};

int main()
{
	int dur[8] = {6, 13, 7, 3, 4, 9, 10, 11};
	struct prozess list[8];

	float avg;
	int counter;
	int time;
	int quantum;

	printf("Quantum  P1  P2  P3  P4  P5  P6  P7  P8  AVG\n");
	printf("--------------------------------------------\n");
	
	for(int q=MINQUANTUM; q<MAXQUANTUM+1; q++) {

		//Initaliserung der Laufzeitvariabeln
		avg = 0;
		
		for(int i=0; i<8; i++) {
			list[i].endtime = 0;
			list[i].bearbeitungszeit = dur[i];
			avg -= dur[i];	
		}

		counter = 0;
		time = 0;
		quantum = q;

		//berechnen der aktuellen Prozesszeit
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
		
		//Ausgabe
		printf("%i        ", quantum);
		
		for(int i=0; i<8; i++) {
			avg += list[i].endtime;
			printf("%i  ", list[i].endtime);
		}
		avg = avg/8;
		printf("%f  ", avg);
		printf("\n");
	}

	return 0;
}
