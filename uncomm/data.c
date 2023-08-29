
// this is a comment//erhhrihwri

/* hello world, this is a block comment */ THIS IS RAW UNVALID TEXT, THAT SHALL NOT BE ABRIDGED

/* this is a multi-lined comment that should
    be detected succesfully, and deleted */

// /* this is a embedded comment that is also detected */ //

/* this is a gross gnu comment          THIS SHOULD BE CENSORED
 * yes, still going
 * still continuing
 * another "*" yes.
 * moooore!!! *'s
 * no more!
*/ This should be spared.

/*yes
 */I think

// data.c
//
//
// shows a C data sample, parses in main.c
// //
// /*
//     Not right
// */

The following is expected to fail horribly

/*
 * ugly gnu comment
 */and this part isnt 


int main(void) {

    int x = 12;
    int *p = NULL;

    p = x;
    
    return 0;
}

