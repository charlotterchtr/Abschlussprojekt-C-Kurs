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
        int y = 0;

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
            //y = text[i] - 'a';
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            if (text[i] + x > 90) {
                crypt[i] = (text[i] + x) - 26;
            } else {
            crypt[i] = text[i] + x;
            }
            //y = text[i] - 'A';
        }
        else if (text[i] == 32) {                   //falls Leerzeichen dann lassen
            crypt[i] = '_';
        } else {
            crypt[i] = text[i];                    //falls Sonderzeichen dann ok, 100 random nummer außerhalb alphabet festgelegt
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
            neutext[i] = text[i];                    //falls Sonderzeichen dann ok, 100 random nummer außerhalb alphabet festgelegt
        }   
    }

    return neutext;
}

//Funktion zur Ausgabe von strings (über printf("%s") würde bei erstem Leerzeichen abbrechen)
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