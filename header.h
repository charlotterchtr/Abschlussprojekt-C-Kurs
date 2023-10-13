/*
Matrikelnummer 2160621
Name Hedwig Charlotte Richter
M. Sc. Computational Science

(benotet in IT-Ausbildung)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Funktionen deklarieren für ver- und entschlüsseln

char * code(char*text, char*key);               //Chiffrieren
char * decode(char*text, char*key);             //Dechiffrieren
void printFile(char* text, FILE * filename);    //Text Ausgeben in Datei


//Funktionen für statistischen Angriff

typedef struct {                                //Struct mit Speicher für zwei int
        int  position;
        int  number;
        } AutoCorr;

//Funktion Autokorrelation
AutoCorr * Corr(FILE * datei, int textlength, char * textzeile);
//Funktion Maximum einer Zahlenmenge
AutoCorr * getMax(AutoCorr * array, int N);
//Größter gemeinsamer Teiler
int gcd(int a, int b);