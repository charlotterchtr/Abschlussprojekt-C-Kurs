/*
Matrikelnummer 2160621
Name Hedwig Charlotte Richter
M. Sc. Computational Science

(benotet in IT-Ausbildung)
*/

#include "header.h"

//Codierfunktion, Eingabe von Urtext und Schlüsselwort
char * code(char*text, char*key) {

    //Schlüsselwort soll so lang wie Urtext sein -> iteration des Schlüsselwortes
    if(strlen(key) < strlen(text)) {
        int a = strlen(key);
        for(int i = strlen(key); i < strlen(text); i += 1) { 
            key[i] = key[i-a];
        }
    }

    //string crypt für verschlüsselten text
    char * crypt = malloc(strlen(text));  
    //Speicher in Länge von Urtext reservieren
    //0 an string ende lassen (+1 speicherplatz reservieren) dann funktioniert print %s

    //Verschlüsseln
    for(int i = 0; i < strlen(text); i++) {
        int x = 0; //Werte egal

        //key umwandeln, enthält keine Leerzeichen
        if(key[i] >= 'a' && key[i] <= 'z') {
            x = key[i] - 'a';
        }
        else if (key[i] >= 'A' && key[i] <= 'Z') {
            x = key[i] - 'A';
        }
        else {printf("Ungültige Eingabe an Position %d\n", i);} //Schlüsselwort soll keine Sonderzeichen enthalten

        //Buchstaben an gleicher Position in Zahl zwischen 0 und 26 umwandeln, zum Umwandeln unterscheiden zwischen groß- und kleinschreibung
        //Urtext umwandeln, Leerzeichen bei ascii32 lassen
        if(text[i] >= 'a' && text[i] <= 'z') {
            if (text[i] + x > 122) {
                crypt[i] = (text[i] + x) - 26;
            } else {
            crypt[i] = text[i] + x;
            }
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            if (text[i] + x > 90) {
                crypt[i] = (text[i] + x) - 26;
            } else {
            crypt[i] = text[i] + x;
            }
        }
        else if (text[i] == 32) {                   //falls Leerzeichen dann lassen
            crypt[i] = '_';
        } else {
            crypt[i] = text[i];                    //falls Sonderzeichen dann übernehmen
        }                         
    }

    return crypt;           
}

//Funktion decode
char * decode(char*text, char*key) {
    
    //Schlüsselwort soll so lang wie Urtext sein -> iteration
    if(strlen(key) < strlen(text)) {
        int a = strlen(key);
        for(int i = strlen(key); i < strlen(text); i += 1) { 
            key[i] = key[i-a];
        }
    }

    //Neutext als array
    char * neutext = malloc(strlen(text));

    for(int i = 0; i < strlen(text); i++) {
        int x = 0; //Werte egal
        int y = 0;

        //key umwandeln in Nummern, enthält keine Leerzeichen, je nach groß oder klein
        if(key[i] >= 'a' && key[i] <= 'z') {
            x = key[i] - 'a';
        }
        else if (key[i] >= 'A' && key[i] <= 'Z') {
            x = key[i] - 'A';
        }
        else {printf("Ungültige Eingabe\n");}

        if(text[i] >= 'a' && text[i] <= 'z') {
            if (text[i] - x < 97) {
                neutext[i] = (text[i] - x) + 26;
            } else {
            neutext[i] = text[i] - x;
            }
            //y = text[i] - 'a';
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            if (text[i] - x < 65) {
                neutext[i] = (text[i] - x) + 26;
            } else {
            neutext[i] = text[i] - x;
            }
            //y = text[i] - 'A';
        }
        else if (text[i] == '_') {                   //falls Leerzeichen dann lassen
            neutext[i] = ' ';
        } else {
            neutext[i] = text[i];                    //falls Sonderzeichen dann übernehmen
        }   
    }

    return neutext;
}

//Funktion zur Ausgabe in Datei
void printFile(char* text, FILE * filename) {
    for(int i = 0; i<strlen(text); i++) {
        fprintf(filename, "%c", text[i]);  
    }
    return;
}

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




