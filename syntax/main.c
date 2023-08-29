
#include <stdio.h>
#include <string.h>

#define STATE_OUT 0
#define STATE_IN  1

/* Write a program to check a C program for rudimentary syntax
	errors like unbalanced parenthesis, brackets and braces. Don't 
   forget about quotes, both single and double, escape sequences
	and comments. (This program is hard if you do it in full generality */

#define RED 	31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define PURPLE  35
#define CYAN    36
#define WHITE 	37

// background colors
#define BGREEN  42
#define BYELLOW 43
#define BBLUE   44
#define BPURPLE 45
#define BCYAN   46
#define BWHITE  47

#define ERROR_COMMENT  "comment unmatched"
#define ERROR_BRACKET  "bracket unmatched"
#define ERROR_DCOMMENT "double comment unmatched"
#define ERROR_QUOTE    "string literal quote mismatch"
#define ERROR_BRACE    "brace unmatched"
#define ERROR_PAREN	   "unmatched parenthesis"
#define ERROR_ARROW	   "unmatched preprocessor arrow"

static const char *error_message;

void print(int color, const char *msg) {
	printf("\e[1;%dm%s\e[0m", color, msg);
}

void bprint(int fg, int bg, const char msg) {
	printf("\e[1;%d;%dm%c\e[0m", bg, fg, msg);
}

void raise_error(int *error, int value, const char *errmsg) {
	(*error) = value;
	error_message = errmsg;
}

int main(void) {

	FILE *fp;

	int single_quote;
	int double_quote;

	int single_comment;
	int multi_comment;

	int bracket_state;	// []
	int brace_state;	// {}

	int arrow_state;
	int paren_state;

	int ignore_comment;
	int escape;			// continue from selected characters if \ set, then unset

	int err;
	int c;

	const char *filename = "./data";

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Unable to open file\n");
		return 1;
	}

	//fp = stdin;

	putchar('\n');

	c = err = 0;
	bracket_state = brace_state = arrow_state = 0;
	multi_comment = single_comment = double_quote = single_quote = 0;
	while ((c = getc(fp)) != EOF) {

		/* 
			Unbalanced or errors of the following:
			
		strings:
			""
			''
		enclosures:
			[]
			()
			{}
		comments:
			//
			'/_*'

		*/

		switch (c) {

			case '\\': // last == '\\'? instead
				escape = 1;
				break;

			case '\n':

				if (escape == 1) {
					escape = 0;
				}	

				// comments
				if (multi_comment == STATE_OUT) {
					ignore_comment = 0;
				}

				//if (single_comment == STATE_OUT) {
				//	// quotes are open
				//	raise_error(&err, 1, ERROR_COMMENT);
				//	goto ERROR;
				//} else {
				//	// remove error flag if closed correctly
				//	raise_error(&err, 0, "");
				//}

				//if (double_quote == STATE_IN) {
					// if we opened one, close the error
					//raise_error(&err, 0, "");
				//}

				if (err == 1 && strcmp(error_message, ERROR_DCOMMENT) != 0) {
					goto ERROR;
				}

				single_comment = STATE_OUT;

				// quotes	
				single_quote = STATE_OUT;
				double_quote = STATE_OUT;

				// braces, brackets
				bracket_state = STATE_OUT;
				paren_state = STATE_OUT;

				arrow_state = STATE_OUT;

				putchar(c);
				break;

			case '/':

				if (single_comment == STATE_OUT) {
					print(BLUE, "!");	
				} else if (single_comment == STATE_IN) {
					print(BLUE, "@");
					ignore_comment = 1;
					raise_error(&err, 0, "");
					
					// finishing double comment slash
					if (multi_comment == STATE_IN) {
						print(CYAN, "#");
						ignore_comment = 0;
						raise_error(&err, 0, "");
					}
					
					multi_comment = STATE_OUT;

					break;
				}

				single_comment = STATE_IN;
				raise_error(&err, 1, ERROR_COMMENT);

				break;			

			case '*':

				if (single_comment == STATE_OUT) {
					
					if (multi_comment == STATE_IN) {
						print(YELLOW, "]");
						ignore_comment = 1;
						//raise_error(&err, 1, ""); // close dcomment
					} else { // normal * ignore this
						putchar('*');
						break;
					}

					single_comment = STATE_IN;
				} else if (single_comment == STATE_IN) {

					// start /*
					raise_error(&err, 1, ERROR_DCOMMENT);					

					print(YELLOW, "[");
					ignore_comment = 1;
				}

				multi_comment = STATE_IN;
				break;


			// End comment detection

			case '"':

				// TODO: ensure this is not within a comment

				if (escape == 1) {
					escape = 0;
					putchar(c);
					continue;
				}

				// no quote yet recorded
				if (double_quote == STATE_OUT) {
					print(GREEN, "+");
					raise_error(&err, 1, ERROR_QUOTE);
				} else if (double_quote == STATE_IN) {
					print(GREEN, "-");
					raise_error(&err, 0, "");
					//double_quote = STATE_OUT; // forgot to reset state
				}

				double_quote = STATE_IN;

				break;

			case '\'':
			
				if (escape == 1) {
					escape = 0;
					putchar(c);
					continue;
				}

	
				// TODO: make sure a '"' cannot be inside a single quote
				if (single_quote == STATE_OUT) {
					print(RED, "~");
					raise_error(&err, 1, ERROR_QUOTE);
				} else if (single_quote == STATE_IN) {
					print(RED, "~");
					raise_error(&err, 0, "");
					//single_quote = STATE_OUT;
				}

				single_quote = STATE_IN;

				break;

			// End strings

			case '[':
				
				if (bracket_state == STATE_OUT) {
					print(GREEN, "[");
					raise_error(&err, 1, ERROR_BRACKET);
				}

				bracket_state = STATE_IN;

				break;
			
			case ',':
				break;

			case ']':
				if (bracket_state == STATE_IN) {
					print(GREEN, "]");
					raise_error(&err, 0, "");
				}

				bracket_state = STATE_OUT;
				
				break;

			// parenthesis start

			case '(':
				if (paren_state == STATE_OUT) {
					print(RED, "(");
					raise_error(&err, 1, ERROR_PAREN);
				}

				paren_state = STATE_IN;

				break;

			case ')':
				if (paren_state == STATE_IN) {
					print(RED, ")");
					raise_error(&err, 0, "");
				}

				paren_state = STATE_OUT;

				break;

			case '<':
				if (arrow_state == STATE_OUT) {
					print(BLUE, "<");
					raise_error(&err, 1, ERROR_ARROW);
				}
		
				arrow_state = STATE_IN;
				break;

			case '>':
				if (arrow_state == STATE_IN) {
					print(BLUE, ">");
					raise_error(&err, 0, "");
				}

				arrow_state = STATE_OUT;
				break;

			default:	
	
				if (ignore_comment == 1) {
					bprint(WHITE, BBLUE, c);
				} else if (double_quote == STATE_IN) {
					bprint(WHITE, BGREEN, c);
				} else if (single_quote == STATE_IN) {
					bprint(WHITE, BGREEN, c);
				} else {
					putchar(c);
				}

				if (single_comment == STATE_IN) {
					single_comment = STATE_OUT;
				}

				// If there is a random character outside a quote raise exception	
		}	

		/* check for error flag */	


	}

	ERROR:
		printf("\e[1;31mexit status: %d\e[0m\n", err);
		if (err == 1) {
			printf("\e[1;31mSyntax Error: ^error %s\e[0m\n", error_message);
		}

	fclose(fp);

	return 0;
}











