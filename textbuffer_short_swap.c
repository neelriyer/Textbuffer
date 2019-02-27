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

#define GREEN   "\x1B[32m" 
#define RESET "\x1B[0m"


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

//Allocate a new textbuffer, whose contents is initialised with the text given in the array. The lines in the input array are all terminated by a `'\\n'`. The whole text is terminated by a `'\\0'`.

Textbuffer textbuffer_new (const char *input_text) {
	
	//If text is NULL
	if (input_text == NULL) {
		return NULL;
	}

	//Create node
	Textbuffer tb = malloc(sizeof(*tb));

    if (tb == NULL) {
        printf ("Insufficient memory \n");
        abort();
    }

	//If string is empty
	if (strlen(input_text)==1) {
		
		//Create node
		line new_line = new_node("");

		//Fix connections
		tb->first = new_line;
		tb->last = new_line;
		tb->n_items = 1;

		return tb;
	}

	//String is non-empty
	//Initialise variables
	tb->n_items = 0;
	tb->first = NULL;
	tb->last = NULL;

	//Copy input_text into running
	char *running = strdup(input_text);

	//Remove the last newline from end of string (!UNSURE! if best method)
	running[strlen(running)-1] = '\0';

	//Use strsep function to seperate string by delimiters
	char *token = strsep (&running, "\n");

	//Until no \n found in str
    while (token != NULL) { 

		//printf("token = %s\n", token);

		//Append to textbuffer
		//Create pointers
		line new_line;

		//Create new node for new line
		new_line = new_node(token);
		
		//if empty textbuffer
		if (tb->n_items == 0) {
			
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
        token = strsep(&running, "\n");
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

	if (tb==NULL) return NULL;

	return tb->n_items;

}


//Return the number of bytes of text stored in the given textbuffer, counting the newlines that would be needed.
size_t textbuffer_bytes (Textbuffer tb) {

	//if null
	if (tb==NULL) {
		return NULL;
	}

	//Initialise pointer
	line curr = tb->first;

	//Initialise size
	size_t size=0;

	while(curr!=NULL) {

		//Add length of string to size
		size = strlen(curr->text)+ size;

		//Add newline to size
		size++;

		//printf("curr->text = %s\n", curr->text);
		//printf("size = %d\n", size);
		
		//Move curr pointer
		curr = curr->next;

	}

	//Add EOF to size
	size++;
	
	return size;

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

	//Create string (!UNSURE! if i've done this correctly)
	char *str = malloc(textbuffer_bytes(tb));
	
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


/**
 * Swap the two given lines in the textbuffer.
 *
 * If line `pos1` or line `pos2` are out of range, the program should
 * `abort()` with an error message.  Lines are zero-indexed; that is,
 * the first line of a textbuffer is at position 0.
 *
 * Your implementation should not swap lines by value; instead, you
 * should manipulate your linked data structure.
 */
void textbuffer_swap (Textbuffer tb, size_t pos1, size_t pos2) {

	line curr = tb->first;

	printf("curr->text = %s\n", curr->text);
	printf("curr->next->text = %s\n", curr->next->text);
	printf("curr->next->next->text = %s\n", curr->next->next->text);
	printf("curr->next->next->next->text = %s\n", curr->next->next->next->text);
	printf("tb->n_items = %d\n", tb->n_items);
	printf("pos1 = %d\n", pos1);
	printf("pos2 = %d\n", pos2);

	//Code

	line a = NULL, b = NULL;
	int i;

	for(curr = tb->first,i=0; curr!=NULL; curr = curr->next,i++) {
	
		
		if(i == pos1) {
		
			printf("curr->text = %s\n", curr->text);
			a = curr;

		}
		
		else if (i == pos2) {

			printf("curr->text = %s\n", curr->text);
			b = curr; 

		}

	}

	if (a!=NULL && b!=NULL) {

		

		//swap a and b
		line temp = a; 
    	a = b; 
    	b = temp; 

		//swap a->next and b->next
		temp = a->next; 
    	a->next = b->next; 
    	b->next = temp; 

		temp = a->prev; 
    	a->prev = b->prev; 
    	b->prev = temp; 
	}

	printf("\nAfter swtiching\n");
	curr = tb->first;
	printf("curr->text = %s\n", curr->text);
	printf("curr->next->text = %s\n", curr->next->text);
	printf("curr->next->next->text = %s\n", curr->next->next->text);
	printf("curr->next->next->next->text = %s\n", curr->next->next->next->text);
	printf("tb->n_items = %d\n", tb->n_items);
	printf("pos1 = %d\n", pos1);
	printf("pos2 = %d\n", pos2);
			


}



void white_box_tests (void) {

	printf("\nWHITEBOX TESTS\n");
	//TEST 1: Testing textbuffer_new
	printf("Test 1: Testing textbuffer_new\n");

		//(A): Empty textbuffer

		//Use function
		Textbuffer empty = textbuffer_new("\n\0");

		//tests
		//textbuffer is not NULL
		assert(empty!=NULL);

		//textbuffer first is not null
		assert(empty->first!=NULL);

		//textbuffer last is not null
		assert(empty->last!=NULL);

		//textbuffer is empty
		assert(empty->n_items == 1);

		//textbuffer first line is ""
		assert(strcmp(empty->first->text,"")==0);

		//textbuffer first->next is NULL
		assert(empty->first->next == NULL);

		//textbuffer last->next is NULL
		assert(empty->last->next == NULL);
		

		//(B): Multiple consecutive newlines
		//Use function
		Textbuffer new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

		//tests
		//textbuffer is not NULL
		assert(new_line!=NULL);

		//textbuffer first is not null
		assert(new_line->first!=NULL);

		//textbuffer last is not null
		assert(new_line->last!=NULL);

		//n_items should be 4
		//printf("new_line->n_items = %d\n", new_line->n_items);
		assert(new_line->n_items == 4);

		//Check each line individually
		assert(strcmp(new_line->first->text,"hello")==0);
		assert(strcmp(new_line->first->next->text, "there")==0);
		assert(strcmp(new_line->first->next->next->text,"")==0);
		assert(strcmp(new_line->first->next->next->next->text,"newline")==0);


		//(C): Several items 
		//Use function
		Textbuffer tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
		
		//tests
		//tb is not NULL
		assert(tb!=NULL);

		//tb first is not null
		assert(tb->first!=NULL);

		//tb last is not null
		assert(tb->last!=NULL);
		
		//Initialise curr pointer 
		line curr = tb->first;

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

		printf("Test 1: Passed\n");


	//TEST 2: Testing textbuffer_to_str
	printf("Test 2: Testing textbuffer_to_str\n");

		//(A): Empty string
	
			//Use function
			char *str = textbuffer_to_str(empty);

			//tests
			//str should be a new line
			assert(strcmp(str,"\n")==0);

		//(B): Multiple consecutive newlines

			//Use function
			str = textbuffer_to_str(new_line);

			//tests
			//create char array for comparison
			char compare[100];
			strcpy(compare,new_line->first->text);
			strcat(compare,"\n");
			strcat(compare,new_line->first->next->text);
			strcat(compare,"\n");
			strcat(compare,new_line->first->next->next->text);
			strcat(compare,"\n");
			strcat(compare,new_line->first->next->next->next->text);
			strcat(compare,"\n");
			
			//compare str and char array
			assert(strcmp(compare,str)==0);

		//(C): Several items
				
			//Use function
			str = textbuffer_to_str(tb);

			//tests
			//create char array for comparison
			strcpy(compare,tb->first->text);
			strcat(compare,"\n");
			strcat(compare,tb->first->next->text);
			strcat(compare,"\n");
			strcat(compare,tb->first->next->next->text);
			strcat(compare,"\n");
			strcat(compare,tb->first->next->next->next->text);
			strcat(compare,"\n");
			
			//compare str and char array
			assert(strcmp(compare,str)==0);

		printf("Test 2: Passed\n");


	//TEST 3: Testing textbuffer_bytes
	printf("Test 3: Testing textbuffer_bytes\n");

		//(A): Empty string

			//Use function
			textbuffer_bytes(empty);

			//test
			//Should be 2 (one for newline and one for EOF)
			assert(textbuffer_bytes(empty) == 2);
			
			
		//(B): Multiple consecutive newlines

			//Use function
			textbuffer_bytes(new_line);

			//tests
			assert(textbuffer_bytes(new_line) == 22);
			

		//(C): Several items

			//Use function
			textbuffer_bytes(tb);

			//tests
			assert(textbuffer_bytes(tb) == 27);


		//(D): NULL textbuffer

			assert(textbuffer_bytes(NULL)==NULL);
			
		printf("Test 3: Passed\n");



	//TEST 4: Textbuffer swap

		printf("Test 4: Testing textbuffer swap\n");
		textbuffer_swap(tb,2,3);	
			

		
		
	



}
