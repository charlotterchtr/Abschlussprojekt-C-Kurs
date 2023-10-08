#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Codierfunktion
char * code(char*text, char*key) {

    //Schlüsselwort soll so lang wie Urtext sein -> iteration
    if(strlen(key) < strlen(text)) {
        int a = strlen(key);
        for(int i = strlen(key); i < strlen(text); i += 1) { 
            key[i] = key[i-a];
        }
    }
    //printf("%s\n", key);

    //Alphabet array als Hilfsarray
    char * alphabet = malloc(26);   //Speicherplatz reservieren, 26 Buchstaben im Alphabet
    for(int i = 0; i<26; i++) {
        alphabet[i] = 'A' + i;
    }
    //printf("%s\n", alphabet);

    //string crypt für verschlüsselten text
    char * crypt = malloc(strlen(text));  //Speicher in Länge von Urtext reservieren

    //Verschlüsseln
    for(int i = 0; i < strlen(text); i++) {
        int x = 0; //Werte egal
        int y = 0;

        //Buchstaben an gleicher Position in Zahl zwischen 0 und 26 umwandeln, zum Umwandeln unterscheiden zwischen groß- und kleinschreibung
        //Urtext umwandeln, Leerzeichen bei 32 lassen
        if(text[i] >= 'a' && text[i] <= 'z') {
            y = text[i] - 'a';
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            y = text[i] - 'A';
        }
        else if (text[i] == 32) {                 //falls Leerzeichen dann lassen
            y = 32;
        }
        else {printf("Ungültige Eingabe\n");}

        //key umwandeln, enthält keine Leerzeichen
        if(key[i] >= 'a' && key[i] <= 'z') {
            x = key[i] - 'a';
        }
        else if (key[i] >= 'A' && key[i] <= 'Z') {
            x = key[i] - 'A';
        }
        else {printf("Ungültige Eingabe\n");}

        //falls Leerzeichen dann bleibt Leerzeichen
        if(y == 32) {
            crypt[i] = '_';
        //falls kein Leerzeichen dann verschlüsseln
        } else {
            int summe = x + y;
            if(summe >= 26) {
                summe -= 26;
            }
            crypt[i] = alphabet[summe];
        }
    }

    //printf("%s\n", crypt);
    /*
    for(int i = 0; i<strlen(text); i++) {
        printf("%c ", crypt[i]);  
    }
    */

    free(alphabet);
    return crypt;           
}

//Funktion decode
char * decode(char*text, char*key) {

    /*
    for(int i = 0; i<strlen(text); i++) {   //text eingabe bricht ab bei Leerzeichen
        printf("%c ", text[i]);  
    }
    */
    
    //Schlüsselwort soll so lang wie Urtext sein -> iteration
    if(strlen(key) < strlen(text)) {
        int a = strlen(key);
        for(int i = strlen(key); i < strlen(text); i += 1) { 
            key[i] = key[i-a];
        }
    }
    //printf("longkey: %s\n", key); //Kontrolle

    //Alphabet array
    char * alphabet = malloc(26);   //Speicherplatz reservieren, 26 Buchstaben im Alphabet
    for(int i = 0; i<26; i++) {
        alphabet[i] = 'A' + i;
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

        //crypt in Zahlen umwandeln, ist nur groß geschrieben, Leerzeichen lassen
        if (text[i] == '_') {
            y = 32;
        } else {
        y = text[i] - 'A';
        }

        //falls Leerzeichen dann bleibt Leerzeichen
        if(y == 32) {
            neutext[i] = ' ';
        //falls kein Leerzeichen dann entschlüsseln
        } else {
            int summe = y - x;
            if(summe < 0) {
                summe += 26;
            }
            neutext[i] = alphabet[summe];
        }
    }

    free(alphabet);
    return neutext;
}

//Funktion zur Ausgabe von strings
void print(char* text) {
    for(int i = 0; i<strlen(text); i++) {
        printf("%c", text[i]);  
    }
    printf("\n");
    return;
}

int main() {

//Urtext eingeben
    int textlength = 100;               //maximale string Länge
    char * urtext = malloc(textlength); //Speicherplatz reservieren, urtext ist pointer auf array Beginn

    printf("Zu verschlüsselnden Text eingeben: \n");
    scanf("%100[^\n]s", urtext);    //string abbruch bei Zeilenumbruch, maximal 100 character

    //printf("Eingegeben ist: %s\n", urtext); //Kontrolle

//Schlüsselwort eingeben

    char * key = malloc(textlength); //Speicherplatz reservieren, urtext ist pointer auf array Beginn

    printf("Schlüsselwort (ohne Leerzeichen) eingeben: \n");
    scanf("%100s", key);    //string abbruch bei Zeilenumbruch, maximal 100 character

    //printf("Eingegeben ist: %s\n", key); //Kontrolle

//Verschlüsselung

    char * crypt = malloc(strlen(urtext));   
    crypt = code(urtext, key);              //bricht stringinput bei Leerzeichen ab?

    //Verschlüsselten Text ausgeben -> evtl. in Datei später?
    print(crypt);

//Entschlüsseln / crypt - key = urtext

    //Schlüsselwort neu eingeben //Speicher schon oben reserviert
    printf("Schlüsselwort (ohne Leerzeichen) eingeben: \n");
    scanf("%100s", key);    //string abbruch bei Zeilenumbruch, maximal 100 character

    //Speicher für neue Ausgabe reservieren in Größe von Urtext
    char * neutext = malloc(strlen(urtext));
    neutext = decode(crypt, key);   

    //entschlüsselten Text ausgeben -> evtl. in Datei später?
    print(neutext);

//dynamisch reservierten Speicher freigeben
    free(crypt);
    free(urtext);
    free(key);

}