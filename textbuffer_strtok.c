//
//  textbuffer.c
//  Assignment 1
//
//	COMP2521: Data Structures and Algorithms
//
//  Created by IYER, Neel Ram on 6/12/18.
//  zID: z5165452
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"


typedef struct line *line;

//Textbuffer line structure
struct line {

	char* text;
	line prev;
	line next;

};

//Textbuffer structure
struct textbuffer { 

    line first;
    line last;
    int n_items; 

};

//Create a new node, initialised with the item provided.
//Obtained from lists.c lab01
line new_node (char *item) {

	
	line n = malloc(sizeof(*n));

    if (n == NULL) {
        printf ("Insufficient memory \n");
        abort();
    }

    n->text = strdup(item);
    n->next = NULL;
	n->prev = NULL;

    return n;
	
}

/**
 * Allocate a new textbuffer, whose contents is initialised with the
 * text given in the array.  The lines in the input array are all
 * terminated by a `'\\n'`. The whole text is terminated by a `'\\0'`.
 */
Textbuffer textbuffer_new (const char *input_text) {
	
	//If no text 
	if (input_text == NULL) {
		return NULL;
	}

	//Create node
	Textbuffer tb = malloc(sizeof(*tb));

    if (tb == NULL) {
        printf ("Insufficient memory \n");
        abort();
    }

	//Initialise variables
	tb->n_items = 0;
	tb->first = NULL;
	tb->last = NULL;

	//Create array of length text +1
    char str[strlen(input_text)+1]; 

	//Copy text into str array
	strcpy(str,input_text);

	//Use strtok function to segment str array by \n
    char* token = strtok(str, "\n"); 

	//Until no \n found in str
    while (token != NULL) { 

		printf("token = %s\n", token);

		//Append to textbuffer
		//Create pointers
		line new_line;

		//Create new node for new line
		new_line = new_node(token);
		
		//if empty textbuffer
		if (tb->n_items ==0) {
			
			//Increment count
			tb->n_items++;
			
			tb->first = new_line;
			tb->last = new_line;

			tb->first->next = NULL;
			tb->first->prev = NULL;
		
			tb->last->next = NULL;
			tb->last->prev = NULL;

		} else {

		//if non-empty textbuffer
		new_line->prev = tb->last;
		tb->last->next = new_line;
		tb->last = new_line;

		//Increment count
		tb->n_items++;
		
		}

		//Update token
        token = strtok(NULL, "\n"); 
    }
	
	//tests

/*
	printf("tb->first->text = %s\n", tb->first->text);
	printf("tb->first->next->text = %s\n", tb->first->next->text);
	printf("tb->first->next->next->text = %s\n", tb->first->next->next->text);
	printf("tb->first->next->next->next->text = %s\n", tb->first->next->next->next->text);
*/

    return tb;

}
	

//Release any resources used by a given textbuffer. It is an error to access the textbuffer after this.
void textbuffer_drop (Textbuffer tb) {

	//Initialise pointers
	line curr = tb->first;
    line next;

    while (curr!= NULL) {

		//printf("deleting\n");
        next = curr->next;
        free(curr);
        curr = next;

    }
}

//Return the number of lines of text stored in the given textbuffer.
size_t textbuffer_lines (Textbuffer tb) {

	assert(tb!=NULL);
	return tb->n_items;

}


//Return the number of bytes of text stored in the given textbuffer, counting the newlines that would be needed.
size_t textbuffer_bytes (Textbuffer tb) {

	//Initialise pointers
	line curr = tb->first;

}


/**
 * Return an array containing the text in the given textbuffer.  Each
 * individual line of the textbuffer needs to be terminated by a `'\\n'`,
 * including the last line.  If there are no lines in the textbuffer,
 * return the empty string.
 *
 * It is the caller's responsibility to free the returned array.
 */
char *textbuffer_to_str (Textbuffer tb) {

	//Create string (!UNSURE! how big it should be)
	//assumption: size = max length of line * number of lines
	//assumption: max length of line = 100 
	char *str = malloc(100*tb->n_items);
	
	//If empty
	if (tb->n_items==0) {
		return str;
	}

	//Initialise curr pointer
	line curr = tb->first;
	
	//Copy string into str
	strcpy(str,curr->text);

/*
	printf("curr->text = %s\n", curr->text);
	printf("tb->first->text = %s\n", tb->first->text);
	printf("tb->first->next->text = %s\n", tb->first->next->text);
	printf("tb->first->next->next->text = %s\n", tb->first->next->next->text);

*/

	//Concatenate newline
	strcat(str,"\n");

	//Move to next node
	curr = curr->next;
	
	while(curr!=NULL) {
		
		//Concatenate string 
		strcat(str,curr->text);

		//Concatenate new line
		strcat(str,"\n");

		//Move to next node
		curr = curr->next;

	}

	//printf("str is:\n%s", str);
	
	return str;

}

void white_box_tests (void) {

	printf("\nWHITEBOX TESTS\n");
	//TEST 1: Testing textbuffer_new
	printf("Test 1: Testing textbuffer_new\n");

		//(A): Empty textbuffer

		//Use function
		Textbuffer empty = textbuffer_new("\n");

		//tests
		//textbuffer is not NULL
		assert(empty!=NULL);

		//textbuffer first is not null
		printf("empty->first->text = %s\n", empty->first->text);
		assert(empty->first!=NULL);

		//textbuffer last is not null
		assert(empty->last!=NULL);

		//textbuffer is empty
		assert(empty->n_items == 0);

		//textbuffer first line is ""
		assert(strcmp(empty->first->text,"")==0);

		//textbuffer first->next is NULL
		assert(empty->first->next == NULL);

		//Drop textbuffer
		textbuffer_drop(empty);

		//(B): One line


		//(C): Several items

		//Use function
		Textbuffer tb = textbuffer_new("hello\nthere\nI am sam\nthis\n");
		
		//tests
		//tb is not NULL
		assert(tb!=NULL);

		//tb first is not null
		assert(tb->first!=NULL);

		//tb last is not null
		assert(tb->last!=NULL);
		
		//Initialise curr pointer 
		line curr = tb->first;

/*
		printf("tb->n_items = %d\n", tb->n_items);
		printf("curr->text is %s\n", curr->text);
		printf("curr->text->next is %s\n", curr->next->text);
		printf("curr->text->next->next is %s\n", curr->next->next->text);

*/
		//test n_items
		assert(tb->n_items == 4);

		//test each line
		assert(strcmp(curr->text,"hello") == 0);
		assert(strcmp(curr->next->text,"there") == 0);
		assert(strcmp(curr->next->next->text,"I am sam") == 0);
		assert(strcmp(curr->next->next->next->text,"this") == 0);
		assert(curr->next->next->next->next == NULL);

		//test last pointer
		assert(strcmp(tb->last->text,"this")==0);

		printf("Passed\n");

	//TEST 2: Testing textbuffer_to_str
	printf("Test 2: Testing textbuffer_to_str\n");

		//Use function
		textbuffer_to_str(tb);

		//tests
		//tb is not NULL
		assert(tb!=NULL);

		//tb first is not null
		assert(tb->first!=NULL);

		//tb last is not null
		assert(tb->last!=NULL);
		
		//Initialise curr pointer 
		curr = tb->first;

		//(A): Empty string

		//(B): One line

		//(C): Several items
		
				
		

		
		
	



}












