/*
Matrikelnummer 2160621
Name Hedwig Charlotte Richter
M. Sc. Computational Science
(unbenotet)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Funktionen deklarieren

char * code(char*text, char*key);               //Chiffrieren
char * decode(char*text, char*key);             //Dechiffrieren
void printFile(char* text, FILE * filename);    //Text Ausgeben in Datei