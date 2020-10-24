/*
 Project created by: Jozef Hruška (xhrusk25)
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
 * Function: uppercase
 * ----------------------------
 *   Converts basic ASCII symbol to it's lowercase version.
 *
 *   @sign: Char that needs to be converted
 *
 *   Returns: Lowercase ASCII char.
 */
char uppercase(char sign) {
    if (sign >= 'a' && sign <= 'z') return sign - 'a' + 'A';
    else return sign;
}

/*
  * Function: lowercase
  * ----------------------------
  *   Converts basic ASCII symbol to it's uppercase version.
  *
  *   @sign: Char that needs to be converted
  *
  *   Returns: Uppercase ASCII char.
  */
char lowercase(char sign) {
    if (sign >= 'A' && sign <= 'Z') return sign - 'A' + 'a';
    else return sign;
}

/*
  * Function: getPossibleChar
  * ----------------------------
  *   Checks city name char by char. If all given letters are included, takes the next one.
  *
  *   @city: Char array with given stdin line
  *   @letters: Char array of letters taken from program arguments
  *
  *   Returns: Enabled char if possible or '!' if not.
*/
char getPossibleChar(char city[100], char letters[]) {
    int result = 0;
    for (int i = 0; i < 100; i++) {
        if (letters[i] == '\0') {
            result = i;
            break;
        } else {
            if (city[i] == lowercase(letters[i])) continue;
            else if (city[i] == uppercase(letters[i])) continue;
            else return '!';
        }
    }

    if (result == 0) return '!';
    else return uppercase(city[result]);
}

/*
 * Function: getPossibleCity
 * ----------------------------
 *   Checks city name char by char. If all given letters are included, returns it's array index.
 *
 *   @cities: Array of char arrays (city names)
 *   @letters: Char array of letters taken from program arguments
 *
 *   Returns: City's array index, 101 if error.
 */
int getPossibleCity(char cities[100][100], char letters[]){
    int result = 101;

    for (int i = 0; i < 100; i++) {
        for (int x = 0; x < 100; x++) {
            if (letters[x] == '\0') return i;
            else {
                if (cities[i][x] == lowercase(letters[x])) continue;
                else if (cities[i][x] == uppercase(letters[x])) continue;
                else break;
            }
        }
    }

    return result;
}

int main(int argc, char * argv[]) {

    char cities[100][100];
    char enabled[28];

    // Clear array 'enabled'
    for (int i = 0; i < 27; i++) enabled[i] = 0;

    if (argc >= 2) {

        int enabledSize = 0;
        int citiesCount = 0;
        bool isDuplicate = false;

        while (fgets(cities[citiesCount], 100, stdin) != NULL) {
            // Check if character is possible
            char possibleChar = getPossibleChar(cities[citiesCount], argv[1]);

            // Create an array of enabled characters
            if (possibleChar == ' ') {
                if (enabled[27] != ' '){
                    enabled[27] = ' ';
                    enabledSize++;
                } else isDuplicate = true;
            } else if (possibleChar != '!') {
                unsigned int sign = (unsigned int) (possibleChar - 64);

                if (enabled[sign] != possibleChar){
                    enabled[sign] = possibleChar;
                    enabledSize++;
                } else isDuplicate = true;
            }

            // Increment count by one
            citiesCount++;
        }

        if (enabledSize == 1 && !isDuplicate) {
            printf("Found: %s\n", cities[getPossibleCity(cities, argv[1])]);
        } else if (enabledSize > 1 || isDuplicate){
            printf("Enable: ");
            for (int i = 0; i < (int)(sizeof(enabled)/sizeof(enabled[0])); i++) {
                if (enabled[i] >= 'A' && enabled[i] <= 'Z') printf("%c", enabled[i]);
                else if (enabled[i] == ' ') printf("[SPACE]");
                else continue;
            }
            printf("\n");
        } else printf("Not found\n");

    } else {
        char city[100];

        // Fetch all first signs and sequence them alphabetically
        while (fgets(city, 100, stdin) != NULL) {
            for (int i = 0; i < (int)(sizeof(enabled)/sizeof(enabled[0])); i++) {
                unsigned int sign = (unsigned int) (uppercase(city[0]) - 64);
                if (enabled[sign] != uppercase(city[0])) enabled[sign] = uppercase(city[0]);
            }
        }

        printf("Enable: ");
        for (int i = 0; i < (int)(sizeof(enabled)/sizeof(enabled[0])); i++) printf("%c", enabled[i]);
        printf("\n");
    }

    return 0;
}
