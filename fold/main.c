
#include <ctype.h>
#include <stdio.h>

#define SPACE_OUT 0
#define SPACE_IN  1

#define MAX_LENGTH 20

/* Write a program to "fold" long input lines into
    two or more shorter lines after the last non-blank
     character that occurs before the n-th column of input.
   
    Make sure your program does something intelligent with 
     very long lines, and if there are no blanks or tabs before
      the specified column. */

// TODO: handle very long lines wrap

int main(void) {

    FILE *fp;

	int state;	// spacing state, do not print spaces at newline
	int last;   // record the last character before the newline

    int nc;
    int c;

    if ((fp = fopen("data", "r")) == NULL) {
        fprintf(stderr, "Unable to open file.\n");
        return 1;
    }

    state = last = nc = 0;
    while ((c = fgetc(fp)) != EOF) {
        ++nc;

		switch (c) {

			case '\n':
				nc = 0;
				putchar(c);

				state = SPACE_IN;	// Deadlock acquired, 

				break;

			case ' ':

				// if recently newlined, do not print spaces
				// until a new character has been encountered				

				if (nc != 0 && state == SPACE_OUT)
					putchar(c);

				break;
	
			case '\t':
				if (nc != 0 && state == SPACE_OUT)
					putchar(c);

				break;
			
			default:
									//	48 -symbols
				if (nc >= MAX_LENGTH) {

					// if the last character was not a blank
					// or we have a case like looooooooooooong
					if (isblank(last) != 0) {
					
						putchar('\n');
						nc = 0;
			
					// If a reoccuring character happened and its normal
					} else if (isblank(last) != 0 && state == SPACE_OUT) {

						putchar('\n');
						nc = 0;
					}

					/*else if (isgraph(last) != 0) {
						// if the last character was a normal character
						// and nc is outrageously large
						putchar('\n');
						nc = 0;
					}*/

					// TODO: long whole words need to be wrapped
					// loooooo
					// oooooooo
					// ooong

				}

				state = SPACE_OUT;

				putchar(c);

				break;
		}	
	
		last = c;
    }

    fclose(fp);

    return 0;
}

