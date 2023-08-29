#include <stdio.h>

#define COMMENT_OUT   0   // pair is matched
#define COMMENT_IN    1   // pair is not matched

/* Write a program to remove all comments from
    a C program. Don't forget to handle quoted strings
     and character constants properly. C comments do
      not nest. */

// TODO: the problem lies in detecting the previous /*
//      with another '*' that disforms the output
// we need to reset state on newline 


// On newline, If a extra * is recorded, it acts as a line is completed, switch to outer '/'
// Comments such as:

/*
 * UNSUPPORTED COMMENT STYLE
 * UNSUPPORTED COMMENT STYLE
 *
 */

// block_state could also be reworked

// Arizona Ranger


int main(int argc, char **argv) {

    FILE *fp;

    int state;                // standard comment
    int block_state;          // multilined/block comment

    int ignore;               // ignore standard comment
    int c;

    if ((fp = fopen("./data.c", "r")) == NULL) {
        fprintf(stderr, "Unable to open file.\n");
        return 1;
    }

    fprintf(stdout, "\nSymbols: start \"//\" = !\tend \"//\" = @\n start block \"/*\" = !&[\tend block \"*/\" = ]@#\n");
    
    block_state = ignore = state = c = 0;
    while ((c = getc(fp)) != EOF) {
        switch (c) {
            /* 
                default_state,  -- indicates if a single quote pair has been paired
                multi_state,    -- indicates if a multi line comment has been paired

                while (not end of file)
                    char is \n:
                        disallow default comment

                    char is /:
                        start std, multi indicators
                        
                        IF prev char is /:
                            complete default comment, skip line

                    char is *:
                        IF start multi:
                            complete multi comment

                    default:
                        if ignore (sort of a carriage return)
                            print nothing
                        else
                            print char
                
            */
            

            case '\n':

                /* If its not a block comment, 
                    lift ignoration */

                if (block_state == COMMENT_OUT) {
                    ignore = 0;                 
                }

                // If the last character was a *
                // problem lies here, 
                state = COMMENT_OUT; 
 
                putchar('\n');
                break;
        
            
            case '/':
 
                // COMMENT_IN  -  is set whenever a full // pair is acquired
                // COMMENT_OUT -  is set whenever a full set is not acquired (half, or none)

                // This has more prevelence
                //if (block_state == COMMENT_IN)

                // TODO: check ignore == 0


                // TODO: !!!
                // /* * <- must have a slash next to here, if not reset double quote

                // no slash yet recorded (first char)
                if (state == COMMENT_OUT) {
                    putchar('!');
                } else if (state == COMMENT_IN) {
                    // recently a slash in a row has occured. break.
                    putchar('@');
                    ignore = 1;

                    // finishing slash
                    if (block_state == COMMENT_IN) {
                        putchar('#');
                        ignore = 0;
                    }

                    block_state = COMMENT_OUT;

                    break;
                }

                state = COMMENT_IN; // If this has happened two times in a row, ignore
                break;

            /* Multiline Comment case */
            case '*':

                /* Is there a active double
                    comment? */ 

                if (block_state == COMMENT_OUT) {
                    //putchar('&');
                    //break;
                }

                // first comment set acquired.
                // we need to check if a previous /* was set. (block_state?)
                if (block_state == COMMENT_IN) {
                    //putchar('Z');
                }

                // check block_state == in before
                if (state == COMMENT_OUT) {

                    // /* acquired, continue on: If this is a legitimate "/*" enable ignore
                    if (block_state == COMMENT_IN) {
                        putchar(']');
                        ignore = 1;
                    } else { // If this is a normal '*' we do not want to trigger, THINKKKK
                        putchar('*');
                        //ignore = 0;
                        break;
                    }
                    //putchar(']');

                    //ignore = 0;
                   
                    //if (block_state == COMMENT_IN) Dont do this. 
                    state = COMMENT_IN; // acquired /* *
                } else if (state == COMMENT_IN) {
                    // acquired "/* "
                    putchar('[');
                    ignore = 1;
                }

                block_state = COMMENT_IN; // second * added 
                break;
    
            /* Normal character */
            default: 

                // a // pair does not count,
                // if it spans between characters, 
                if (state == COMMENT_IN) {
                    state = COMMENT_OUT;
                }

                //if (block_state == COMMENT_IN) {
                //    block_state = COMMENT_OUT;
                //}

                // NOTE: we need block_ignore because ignore needs to stop on '\n'

                // we do not ignore the rest of the line
                if (ignore == 0)
                    putchar(c);
                else // we do ignore all of it
                    putchar('x');

                break;
        }
    }

    fclose(fp);

    return 0;
}
