/*
In Kommandozeile schreiben zum
    a) chiffrieren
        gcc main.c
        ./a.out c Praktische_Physik.txt 
        (chiffrierter Text wird in Datei Crypt.txt geschrieben)
    b) dechiffrieren
        gcc main.c
        ./a.out d Crypt.txt
        (dechiffrierter Text wird in Datei Clear.txt geschrieben)
    
    Schlüsselwort kann bliebig vergeben werden, ohne Umlaute, sollte natürlich zum ver- und entschlüsseln dasselbe sein
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    char * crypt = malloc(strlen(text));  //Speicher in Länge von Urtext reservieren

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

//Funktion zur Ausgabe von strings (über printf("%s") würde bei erstem Leerzeichen abbrechen)
/*
void print(char* text) {
    for(int i = 0; i<strlen(text); i++) {
        printf("%c", text[i]);  
    }
    printf("\n");
    return;
}
*/

//Funktion zur Ausgabe in Datei
void printFile(char* text, FILE * filename) {
    for(int i = 0; i<strlen(text); i++) {
        fprintf(filename, "%c", text[i]);  
    }
    //fprintf(filename, "\n");
    return;
}


int main(int argc, char **argv) {

        //eine Eingebae akzeptiert (argc = 2)
        if(argc < 2 || argc > 3) { //argc = 0 immer Betriebssystem, arc = 1 ist erste Eingabe
        printf("Usage: no valid imput \n");
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

else {
    printf("Ungültige Eingabe, bitte c zum Chiffrieren und d zum Dechiffrieren eingeben, danach Dateiname\n");
}
    

}