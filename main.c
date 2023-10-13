/*
C-Blockkurs WS 23/24

Matrikelnummer: 2160621
Name: Hedwig Charlotte Richter

benotet in IT-Ausbildung

Projektname: Vigenère-Chiffre

Compileraufruf

gcc main.c header.c

Ausführen zum

    a) chiffrieren
            ./a.out c Praktische_Physik.txt 
        (chiffrierter Text wird in Datei Crypt.txt geschrieben)

    b) dechiffrieren
            ./a.out d Crypt.txt
        (dechiffrierter Text wird in Datei Clear.txt geschrieben)

    c) angreifen
            ./a.out a Crypt.txt
        (Länge des Schlüsselwortes wird versucht herauszufinden aus Crypt.txt)
    
    Schlüsselwort kann bliebig vergeben werden, ohne Umlaute, sollte natürlich zum ver- und entschlüsseln dasselbe sein
*/

#include "header.h"


int main(int argc, char **argv) {

        //zwei Eingaben akzeptiert (argc = 3)
        if(argc < 2 || argc > 3) { //argc = 0 immer Betriebssystem, arc = 1 ist erste Eingabe
        printf("Ungültige Eingabe \nBitte c zum Chiffrieren, d zum Dechiffrieren oder a für Angriff eingeben, \ndanach Dateiname\n");
        exit(1);
        }

//Verschlüsseln
if (strcmp(argv[1], "c") == 0) {
    
    //open urdatei die verschlüsselt werden soll
    char* dateiname = argv[2];
    FILE * urdatei = fopen(dateiname, "r");

    //open cryptdatei in die verschlüsselter text geschrieben werden soll
    FILE * cryptdatei = fopen("Crypt.txt", "w");

    //speicher reservieren für herausgelesene Zeilen aus urdatei
    int textlength = 200;               //maximale string Länge, hoffentlich ist keine Zeile länger als 200 Zeichen
    char * urtext = malloc(textlength); //Speicherplatz reservieren, urtext ist pointer auf array Beginn

    //Schlüsselwort (key) vergeben
    char * key = malloc(textlength); //Speicherplatz reservieren, urtext ist pointer auf array Beginn
    printf("Schlüsselwort (ohne Leerzeichen) eingeben: \n");
    scanf("%100s", key);    //string abbruch bei Zeilenumbruch, maximal 100 character

    while(!feof(urdatei)) {
        fgets(urtext, textlength, urdatei);  //Zeile in urtext-array geschrieben

        char * crypt = malloc(strlen(urtext));   
        crypt = code(urtext, key);

        printFile(crypt, cryptdatei);
        free(crypt);
    }

    free(urtext);
    free(key);
    fclose(urdatei);
    fclose(cryptdatei);
}

//Dechiffrieren
else if (strcmp(argv[1], "d") == 0) {

    //open datei die entschlüsselt werden soll
    char* dateiname = argv[2];
    FILE * cryptdatei = fopen(dateiname, "r");

    //open enddatei in die entschlüsselter text geschrieben werden soll
    FILE * neudatei = fopen("Clear.txt", "w");

    //speicher reservieren für herausgelesene Zeilen aus urdatei
    int textlength = 200;               //maximale string Länge, hoffentlich ist keine Zeile länger als 200 Zeichen
    char * crypt = malloc(textlength); //Speicherplatz reservieren, urtext ist pointer auf array Beginn

    //Schlüsselwort (key) eingeben
    char * key = malloc(textlength); //Speicherplatz reservieren, urtext ist pointer auf array Beginn
    printf("Schlüsselwort (ohne Leerzeichen) eingeben: \n");
    scanf("%100s", key);    //string abbruch bei Zeilenumbruch, maximal 100 character

    while(!feof(cryptdatei)) {
        fgets(crypt, textlength, cryptdatei);  //Zeile in urtext-array geschrieben

        char * neutext = malloc(strlen(crypt));   
        neutext = decode(crypt, key);

        printFile(neutext, neudatei);
        free(neutext);
    }
    
    free(crypt);
    free(key);
    fclose(neudatei);
    fclose(cryptdatei);

}

//Statistischer Angriff -> Länge des Schlüsselworts herausfinden
else if (strcmp(argv[1], "a") == 0) {

    char* dateiname = argv[2];
    FILE * urdatei = fopen(dateiname, "r");

    int textlength = 200;                       //maximale string Länge, hoffentlich ist keine Zeile länger als 200 Zeichen
    char * urtext = malloc(textlength); 

    //Autokorrelation von verschlüsseltem Text
    AutoCorr * array;                               //Ergebnis der Autokorrelation in struct gespeichert mit Übereinstimmung und Position
    array = Corr(urdatei, textlength, urtext);
    
    //die Positionen mit den N=10 größten Übereinstimmungen herauslesen aus Autokorrelations struct (array)
    //denn nur Positionen mit größten Übereinstimmungen sind relevant
    AutoCorr * max;
    int N = 10;
    max = getMax(array, N);

    //print
    printf("\nErgebnis der Autokorrelation (Anzahl der Übereinstimmungen bei Verschiebung um Positionen x): \n");
    for(int j = 0; j<N; j++) {
        printf("Verschiebung um %d: %d\n", max[j].position, max[j].number);
    }
    printf("\n");

    //Größter gemeinsamer Teiler der Positionsnummern -> in teiler-array gespeichert
    int x = 0;
    int * teiler = malloc(N*10*sizeof(int));
    while(max[x+1].number > 70) {     //>70 random gewählt aber soll nur die signifikantesten Ergebnisse nehmen, für maximal 10 ist Speicher reserviert
    teiler[x] = gcd(max[x].position, max[x+1].position);
    x++;
    }

    //Häufigkeit des größten gemeinsamen Teilers aus teiler-array in length-array
    AutoCorr * length = malloc(15*sizeof(*length));
    for(int j = 0; j<15; j++) {
        length[j].number = 0;       //auf 0 setzen um Speichereffekte zu vermeiden
    }
    x=0;
    //Zählen
    while (teiler[x] != 0) {
        for(int i=2; i<15; i++) {
            length[i-2].position = i;
                if(teiler[x] == i){
                length[i-2].number += 1;
                }
        }
        x++;
    }

    //Die zwei Teiler mit dem höchsten Vorkommen herauslesen und ausgeben
    AutoCorr * solution;
    solution = getMax(length, 2);

    printf("Das Schlüsselwort könnte %d oder %d Zeichen lang sein\n", solution[0].position, solution[1].position);

    fclose(urdatei);
    free(urtext);
    free(teiler);
    free(length);

}
    

else {
    printf("Ungültige Eingabe \nBitte c zum Chiffrieren, d zum Dechiffrieren oder a für Angriff eingeben, \ndanach Dateiname\n");
}

}




