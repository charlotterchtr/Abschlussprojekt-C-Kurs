#include "header.h"

 typedef struct {
        int  position;
        int  number;
        } AutoCorr;

//Autokorrelation -> in Funktion
AutoCorr * Corr(FILE * datei, int textlength, char * textzeile) {
    AutoCorr * array;                               //Ergebnis der Autokorrelation in struct gespeichert mit Übereinstimmung und Position
    array = malloc(textlength*sizeof(*array)); 
    
    while(!feof(datei)) {
        fgets(textzeile, textlength, datei); //eine Zeile aus Datei
        //textzeile = "Das ist ein test string das ist ein test string das ist ein test string";

        for(int j = 1; j<strlen(textzeile); j++) {  
            array[j].position = j;                       //j ist Anzahl der Positionen um die verschoben wird
            for( int i = j; i<strlen(textzeile); i++) {        //i ist Position im array
                if (textzeile[i] != '_' && textzeile[i-j] == textzeile[i]) {
                    array[j].number += 1;
                }           
            }
        }
    }
    array[32].number = 0;           //Zeilenlänge vermutlich oft 32 deshalb keine Aussage über Schlüsselwort
    return array;
}

AutoCorr * getMax(AutoCorr * array, int N) {    //die N größten Zahlen
    AutoCorr * max;
    max = malloc(N*sizeof(*max));  //struct in das maxima gespeichert werden sollen, die 20 größten

    for(int j = 0; j<N; j++) {
        max[j].number = 0;          //maximum ist in max[j].number zwischengespeichert
        for(int i = 0; i<200; i++) {      
                if (array[i].number > max[j].number) {
                    max[j].number = array[i].number;
                    max[j].position = array[i].position;
            }
        }
        for(int i = 0; i<200; i++) {
                if (array[i].position == max[j].position) {
                    array[i].number = 0;
            }
        }
    }

    return max;
}


int main() {

    char* dateiname = "Crypt.txt";
    FILE * urdatei = fopen(dateiname, "r");

    int textlength = 200;                       //maximale string Länge, hoffentlich ist keine Zeile länger als 200 Zeichen
    char * urtext = malloc(textlength); 

    //Autokorrelation von verschlüsseltem Text
    AutoCorr * array;                               //Ergebnis der Autokorrelation in struct gespeichert mit Übereinstimmung und Position
    array = Corr(urdatei, textlength, urtext);
    
    //die Positionen mit den N=10 größten Übereinstimmungen herauslesen aus Autokorrelations struct (array)
    AutoCorr * max;
    int N = 10;
    max = getMax(array, N);

    //print
    for(int j = 0; j<N; j++) {
        printf("Verschiebung um %d: %d\n", max[j].position, max[j].number);
    }

    //Teiler der Verschiebungsanzahlen -> evtl. am höchsten Priorisierte Zahl unter 15 nehmen und nach weiteren Teilern schauen?
    int x = 0;
    int * teiler = malloc(N*10*sizeof(int));

    for (int i=0; i < N; i++) {
        for (int j=2; j < 10; j++) {
            if(max[i].number % j == 0) {
                teiler[x] = j;
                x+=1;
            }
        }
    }

    //print
    x=0;
    while (teiler[x] != 0) {
        printf("%d\n", teiler[x]);
        x++;
    }

    AutoCorr * length = malloc(10*sizeof(*length));

    //Häufigkeit der Teiler -> nicht wirklich zielführend
    x=0;

    for(int j = 0; j<10; j++) {
        length[j].number = 0;
    }

    while (teiler[x] != 0) {
        for(int i=0; i<10; i++) {
            length[i].position = i;
                if(teiler[x] == i){
                length[i].number += 1;
                }
        }
        x++;
    }

    //print
    for(int j = 0; j<10; j++) {
        printf("Teiler %d; Häufigkeit %d\n", length[j].position, length[j].number);
    }

    AutoCorr * solution;
    solution = getMax(length, 3);

    printf("Das Schlüsselwort ist vermutlich %d, %d oder %d Zeichen lang\n", solution[0].position, solution[1].position, solution[2].position);

}