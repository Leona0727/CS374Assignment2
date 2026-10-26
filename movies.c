#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <string.h>


// structure
struct movie {
    char *title;
    int year;
    char languages[5][21];
    int languageCount;
    double rating;
    struct movie *next;
};



void freeMovies(struct movie *head) {
    struct movie * current = head; 

    while ( current != NULL ) {
        struct movie * temp = current;
        current = current->next;
        free(temp ->title);
        free (temp );
    }
}

void showMoviesByYear (struct movie *head){
    int year;
    int found = 0;

    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    struct movie *current =head;

    while (current != NULL) {
        if (current->year == year) {
        printf("%s\n", current->title);
            found =  1;
        }
        current = current -> next;
    }
        if (found == 0) {
        printf("No data about movies released in the year %d\n", year);
    }
}

void showHighestRated (struct movie * head) {
    struct movie*current= head;

    while (current != NULL ) {
        int alreadyShown = 0;
        struct movie* check = head;

        while (check != current) {
            if (check ->year == current -> year){
                alreadyShown = 1;
                break;
            }
            check = check ->next;
        }

        if ( alreadyShown == 0) {
            struct movie *best = current;
            struct movie *search = current ->next;

            while (search != NULL){
                if (search-> year == current->year && search->rating > best->rating){
                    best = search;
                }
                search = search -> next;
            }
            printf("%d %.1f %s\n", best->year, best->rating, best->title);
        }
        current = current->next;
    }
}

void showMoviesByLanguage (struct movie *head) {
    char language[20];
int found = 0;

printf("Enter the language for which you want to see movies: ");
    scanf("%19s", language);

    struct movie *current = head;

    while (current != NULL) {
        for (int i = 0; i < current->languageCount; i++) {
            if ( strcmp (current -> languages [i], language) == 0) {
                printf("%d %s\n", current->year, current->title);
                found = 1;
                break;
            }
        }
        current = current-> next;

    }

    if ( found == 0){
        printf("No data about movies released in %s\n", language);
    }
}

void showMenu ( void ) {
    printf ("\n");
    printf("1. Show movies released in the specified year\n");
    printf( "2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf ( "4. Exit from the program\n\n");
    printf( "Enter a choice from 1 to 4: ");
}

/*
* Function: processMovieFile
* Opens a file, reads and prints each line
* filePath: path to the file
*
* This function shows sample code that opens a file, then in a loop reads and
prints each line in that file.
* You can use this code however you want in your Prog Assignment 2.
*/
struct movie *processMovieFile(char* filePath, int *movieCount){
char *currLine = NULL;
size_t len = 0;
// Open the specified file for reading only
FILE *movieFile = fopen(filePath, "r");


    if (movieFile == NULL) {
        printf ("Could not open file %s\n", filePath);
        exit( EXIT_FAILURE);
    }

    struct movie *head = NULL;
    struct movie *tail = NULL;

    *movieCount = 0;
     getline(&currLine, &len, movieFile);

// Read the movie data
while (getline(&currLine, &len, movieFile) != -1) {
    currLine[strcspn(currLine, "\n")] = '\0';

    char *savePtr;
    char *titleToken = strtok_r(currLine, ",", &savePtr);
    char *yearToken = strtok_r(NULL, ",", &savePtr);
    char *languagesToken = strtok_r(NULL, ",", &savePtr);
    char *ratingToken = strtok_r(NULL, ",", &savePtr);

    if (titleToken == NULL || yearToken == NULL ||  languagesToken == NULL || ratingToken == NULL) {
        continue;
    }

    struct movie *newMovie = malloc( sizeof (struct movie));

    if (newMovie == NULL){
        free(currLine);
        fclose(movieFile);
        freeMovies(head);
        exit(EXIT_FAILURE);
    }

    newMovie->title = malloc (strlen(titleToken) + 1);

    if (newMovie->title == NULL) {
        free (newMovie);
        free( currLine);
        fclose(movieFile);
        freeMovies (head);
        exit (EXIT_FAILURE);
    }

    strcpy (newMovie->title, titleToken);
    newMovie->year = atoi( yearToken);
    newMovie->rating = strtof(ratingToken, NULL);
    newMovie->languageCount = 0;
    newMovie->next = NULL;

    /* Remove the brackets from the language list */
    if (languagesToken[0] == '[') {
        languagesToken++;
    }

    size_t langLen = strlen (languagesToken);

    if (langLen > 0 && languagesToken[langLen - 1] == ']') {
        languagesToken[langLen - 1] = '\0';
    }

    char *langSavePtr;
    char *language = strtok_r( languagesToken, ";", &langSavePtr);

    while (language != NULL && newMovie->languageCount < 5) {
        strncpy(newMovie-> languages[newMovie->languageCount], language, 20);
        newMovie->languages[newMovie->languageCount][20] = '\0';

        newMovie ->languageCount++;
        language = strtok_r(NULL, ";", &langSavePtr);
    }

    if (head == NULL) {
        head = newMovie;
        tail = newMovie;
    } else {
        tail->next = newMovie;
        tail = newMovie;
    }

    (*movieCount) ++;
}

free(currLine);
fclose(movieFile);

return head;
}
/**
*
*/



// struct movie *newMovie = mallo


int main ( int argc, char **argv ){
if (argc < 2)
{
printf("You must provide the name of the file to process\n");
printf("Example usage: ./movies movies.csv\n");
return EXIT_FAILURE;
}
int movieCount;
struct movie*head =processMovieFile ( argv[1], &movieCount);

printf("Processed file %s and parsed data for %d movies\n", argv[1], movieCount);

int choice;

do {
    showMenu();
    scanf("%d", & choice);

    if ( choice == 1) {
        showMoviesByYear(head);

    } else if ( choice == 2){
        showHighestRated(head);
    } else if (choice == 3) {
        showMoviesByLanguage(head);

    } else if ( choice == 4 ) {
        freeMovies(head);
    } else {
    printf("You entered an incorrect choice. Try again.\n");
    }

} while (choice != 4);


return EXIT_SUCCESS;
}