#include "header.h"

//Struct mit Speicher für zwei int
typedef struct {
        int  position;
        int  number;
        } AutoCorr;

//Autokorrelation
AutoCorr * Corr(FILE * datei, int textlength, char * textzeile) {
    AutoCorr * array;                               //Ergebnis der Autokorrelation in struct gespeichert mit Übereinstimmung und Position
    array = malloc(textlength*sizeof(*array)); 
    
    while(!feof(datei)) {
        fgets(textzeile, textlength, datei); //eine Zeile aus Datei

        for(int j = 1; j<strlen(textzeile); j++) {  
            array[j].position = j;                              //j ist Anzahl der Positionen um die verschoben wird
            for( int i = j; i<strlen(textzeile); i++) {         //i ist Position im array
                if (textzeile[i] != '_' && textzeile[i-j] == textzeile[i]) {
                    array[j].number += 1;
                }           
            }
        }
    }
    array[32].number = 0;           //Zeilenlänge vermutlich oft 32 deshalb keine Aussage über Schlüsselwort, wird ausgeschlossen
    return array;
}

//Maxima einer Menge an Zahlen
AutoCorr * getMax(AutoCorr * array, int N) {    //die N größten Zahlen
    AutoCorr * max;
    max = malloc(N*sizeof(*max));               //struct in das maxima gespeichert werden sollen, die N größten

    for(int j = 0; j<N; j++) {
        max[j].number = 0;                      //maximum ist in max[j].number zwischengespeichert
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

//Größter gemeinsamer Teiler
        int gcd(int a, int b) {
        if (b == 0) return a;
        return gcd(b, a % b);
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
    
    int x = 0;
    int * teiler = malloc(N*10*sizeof(int));

    while(max[x+1].number > 70) {     //>70 random gewählt aber soll nur die signifikantesten Ergebnisse nehmen
    //Größter gemeinsamer Teiler
    teiler[x] = gcd(max[x].position, max[x+1].position);
    x++;
    }

    //print
    x=0;
    while (teiler[x] != 0) {
        printf("%d\n", teiler[x]);
        x++;
    }


    AutoCorr * length = malloc(10*sizeof(*length));

    //Häufigkeit des größten gemeinsamen Teilers aus teiler-array
    
   
    for(int j = 0; j<10; j++) {
        length[j].number = 0;
    }

    x=0;
    while (teiler[x] != 0) {
        for(int i=0; i<10; i++) {
            length[i].position = i;
                if(teiler[x] == i){
                length[i].number += 1;
                }
        }
        x++;
    }

    /*
    //print
    for(int j = 0; j<10; j++) {
        printf("Teiler %d; Häufigkeit %d\n", length[j].position, length[j].number);
    }
    */

    //Die zwei Teiler mit dem höchsten Vorkommen herauslesen und ausgeben
    AutoCorr * solution;
    solution = getMax(length, 2);

    printf("Das Schlüsselwort ist vermutlich %d oder %d Zeichen lang\n", solution[0].position, solution[1].position);

}