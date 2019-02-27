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
#include <stdbool.h>

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
    size_t n_items; 

};

//For boolean data type in textbuffer_search function
typedef enum
{

  FALSE,
  TRUE

} boolean;

//Create a new node, initialised with the item provided.
//Obtained from lists.c lab01
static line new_node (char *item) {

	
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
	return tb->n_items;

}


//Return the number of bytes of text stored in the given textbuffer, counting the newlines that would be needed.
size_t textbuffer_bytes (Textbuffer tb) {

	//if null
	assert(tb!=NULL);

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
	//size++;
	
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

	printf("here1\n");	
	
	//Initialise curr pointer
	line curr = tb->first;

	printf("here2\n");
	
	//Copy string into str
	strcpy(str,curr->text);

	printf("here3\n");
	printf("curr->text = %s\n", curr->text);
	

/*
	printf("curr->text = %s\n", curr->text);
	printf("tb->first->text = %s\n", tb->first->text);
	printf("tb->first->next->text = %s\n", tb->first->next->text);
	printf("tb->first->next->next->text = %s\n", tb->first->next->next->text);

*/

	

	printf("here4\n");

	//Concatenate newline if curr->next is not null
	if(curr->next!=NULL) strcat(str,"\n");

	printf("here5\n");
	
	//Move to next node
	curr = curr->next;


	while(curr!=NULL) {

		printf("here6\n");
		
		//Concatenate string 
		strcat(str,curr->text);

		printf("str = %s\n", str);
		printf("curr->text = %s\n", curr->text);

		//if next node is NULL break
		if(curr->next==NULL) break;

		//Otherwise go to next node
		curr = curr->next;
		
		//Concatenate new line (here, 1st iter)
		strcat(str,"\n");

	}

	//printf("str is:\n%s", str);
	
	
	return str;

}


// Swap the two given lines in the textbuffer. If line `pos1` or line `pos2` are out of range, the program should `abort()` with an error message.  Lines are zero-indexed; that is, the first line of a textbuffer is at position 0. Your implementation should not swap lines by value; instead, you should manipulate your linked data structure.
void textbuffer_swap (Textbuffer tb, size_t pos1, size_t pos2) {

	if (tb==NULL) {
		return;
	}

	size_t index = tb->n_items - 1;

	//Edgecases
	if(pos1>index || pos2>index) {
		printf("Error out of bounds\n");
		abort();
	}
	
	if (index == 0) {
		return;
	}

	if (pos1 == pos2) {
		return;
	}
	
	//before switching
/*
	line curr = tb->first;
	printf("before switching\n");
	printf("curr->text = %s\n", curr->text);
	printf("curr->next->text = %s\n", curr->next->text);
	printf("curr->next->next->text = %s\n", curr->next->next->text);
	printf("curr->next->next->next->text = %s\n", curr->next->next->next->text);
	printf("tb->n_items = %d\n", tb->n_items);
	printf("pos1 = %d\n", pos1);
	printf("pos2 = %d\n", pos2);
*/

	//make pos1 come before pos2
	if (pos1>pos2) {
		size_t tmp = pos1;
		pos1 = pos2;
		pos2 = tmp;
	}
		
	//Create pointer for pos1
	line curr_pos1 = tb->first;
	size_t count_pos1 = 0;

	//Navigate to pos1
	while(curr_pos1!=NULL && count_pos1 != pos1) {

		curr_pos1 = curr_pos1->next;
		count_pos1++;

	}

	//printf("curr_pos1->text = %s\n", curr_pos1->text);
	//Create pointer for pos2
	line curr_pos2 = tb->first;
	size_t count_pos2 = 0;

	//Navigate to pos2
	while(curr_pos2!=NULL && count_pos2 != pos2) {

		curr_pos2 = curr_pos2->next;
		count_pos2++;

	}

	//printf("curr_pos2->text = %s\n", curr_pos2->text);

	//printf("\nDuring switch\n");
	//CASE 1: If next to each other
	if (curr_pos1->next == curr_pos2) {

		//Create ptrs to next and previous
		line next = curr_pos2->next;	
		line prev = curr_pos1->prev;

		//Fix links
		curr_pos1->next = next;
		curr_pos2->prev = prev;

		//If pos1 not at edges of linked list
		if (curr_pos1->next!=NULL) {
			
			//Fix links
			next = curr_pos1->next;
			next->prev = curr_pos1;
			//printf("next->text = %s\n",next->text);
			//printf("curr_pos1->text = %s\n",curr_pos1->text);
		
		}

		//If pos2 not at edges of linked list
		if (curr_pos2->prev!=NULL) {
			
			//Fix links
			prev = curr_pos2->prev;
			prev->next = curr_pos2;
			//printf("prev->text = %s\n",prev->text);
			//printf("curr_pos2->text = %s\n",curr_pos2->text);
		}

		//Fix links in between pos1 and pos2
		curr_pos1->prev = curr_pos2;
		curr_pos2->next = curr_pos1;

	}

	//CASE 2: If not next to each other
	else {

		line prev = curr_pos2->prev;
		line next = curr_pos2->next;

		//Change links that travel from pos1, pos2 to other nodes
		//pos2
		line ptr = curr_pos1->prev;
		line ptr1 = curr_pos1->next;
		curr_pos2->prev = ptr;
		curr_pos2->next = ptr1;

		//pos1
		curr_pos1->prev = prev;
		curr_pos1->next = next;

		//Change links that travel towards pos1, pos2
		//pos2
		if (curr_pos2->next!=NULL) {
			next = curr_pos2->next;
			next->prev = curr_pos2;
		}

		if (curr_pos2->prev!=NULL) {
			prev = curr_pos2->prev;
			prev->next = curr_pos2;
		}

		//pos1
		if (curr_pos1->next!=NULL) {
			next = curr_pos1->next;
			next->prev = curr_pos1;
		}

		if (curr_pos1->prev!=NULL) {
			prev = curr_pos1->prev;
			prev->next = curr_pos1;
		}
	}


	//Update first and last if necessary
	if (pos1==0) {
		tb->first = curr_pos2;
	}
	if (pos2 == index) {
		tb->last = curr_pos1;
	}

	//printf("curr_pos2->text = %s\n", curr_pos2->text);
	//After swtiching

/*
	printf("\nAfter swtiching\n");
	curr = tb->first;
	printf("curr->text = %s\n", curr->text);
	printf("curr->next->text = %s\n", curr->next->text);
	printf("curr->next->next->text = %s\n", curr->next->next->text);
	printf("curr->next->next->next->text = %s\n", curr->next->next->next->text);
	printf("tb->last->text = %s\n",tb->last->text);
	printf("tb->n_items = %d\n", tb->n_items);
	printf("pos1 = %d\n", pos1);
	printf("pos2 = %d\n", pos2);
*/
	return;

}

//Merge `tb2` into `tb1` at line `pos`. Afterwards, line 0 of `tb2` will be line `pos` of `tb1`, and the old line `pos` of `tb1` will follow after the last line of `tb2`.  If line `pos` is equal to the number of lines in `tb1`, `tb2` is inserted at the end.  If line `pos` is out of range, the program should `abort()` with an error message. After this operation, `tb2` can no longer be used, as if we had used `textbuffer_drop()` on it.
//
void textbuffer_insert (Textbuffer tb1, size_t pos, Textbuffer tb2) {

	//TODO: edgecases such as
	//tb2 is empty
	//tb1 || tb2 is NULL
	//tb1 = tb2
	//pos if out of bounds
	//CASES: insert at start, insert at end, insert in middle

	//edgecases
	if(tb2==NULL || tb1 ==NULL) return;

	if(tb1 == tb2) return;

	if(tb1->n_items == 0 && tb2->n_items == 0) return;

	if(pos>tb1->n_items) {
		printf("Error: Position out of bounds\n");
		abort();
	}

	//CASE 1: INSERT AT START
	if(pos==0) {

		//Create pointer
		line curr = tb1->first;

		//Fix links
		tb1->first = tb2->first;
		tb1->first->prev = NULL;

		curr->prev = tb2->last;
		tb2->last->next = curr;

	}

	//CASE 2: INSERT AT END
	else if(pos == (tb1->n_items)) {

		//Create pointer
		line curr = tb1->last;

		//Fix links
		tb1->last = tb2->last;
		tb1->last->next = NULL;

		curr->next = tb2->first;
		tb2->first->prev = curr;

	}

	//CASE 3: INSERT IN MIDDLE
	else {

		//Create pointer
		line curr = tb1->first;
		size_t count = 0;

		//Navigate to pos
		while(curr!=NULL && count != pos) {
			curr = curr->next;
			count++;
		}		

		//Fix links
		line prev = curr->prev;
		
		prev->next = tb2->first;
		tb2->first->prev = prev;

		tb2->last->next = curr;
		curr->prev = tb2->last;			
	}

	//modify n_items
	tb1->n_items = textbuffer_lines(tb1) + textbuffer_lines(tb2);
	
	//free textbuffer tb2
	free(tb2);
}


/**
 * From the beginning/end of the textbuffer, search for the first
 * occurrence of the string `match`.  Return the number of the line
 * containing the match, or -1 if there was no match.
 *
 * @param tb	the Textbuffer to search in
 * @param match	the string to match on
 * @param rev	if true, search from the last line backwards;
 *		otherwise, search forwards from the first line.
 */
size_t textbuffer_search (Textbuffer tb, char *match, bool rev) {

	//return value (!UNSURE!)
	size_t return_value = 0;
	return_value--;

	//edgecases
	if(tb==NULL || strlen(match)==0 || match==NULL) {
		return return_value;
	}

	//if not bool
	if (rev !=0 && rev !=1) {
		return return_value;
	}

/*
	printf("rev = %d\n", rev);
	printf("match is %s\n", match);
	printf("strlen(match) = %d\n", strlen(match));
	
*/
	//rev is true
	if (rev==TRUE || rev==1) {

		size_t line_number = tb->n_items;
		line curr;
	
		for(curr = tb->last;curr!=NULL;curr = curr->prev) {

			if(strcmp(curr->text,match)==0) {
				return line_number;
			}
			line_number--;
		}
	}

	//rev is false
	if (rev==FALSE || rev == 0) {

		size_t line_number = 1;
		line curr;
		
		for (curr = tb->first;curr!=NULL;curr = curr->next) {

			if(strcmp(curr->text,match)==0) {
				return line_number;
			}
		
			line_number++;
		}
	}

	return return_value;

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
			assert(new_line->first->next->next->next->next == NULL);


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
			assert(strcmp(str,"")==0);

		//(B): Multiple consecutive newlines

			//Use function
			printf("here\n");
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
			
			//compare str and char array
			assert(strcmp(compare,str)==0);

		printf("Test 2: Passed\n");


	//TEST 3: Testing textbuffer_bytes
	printf("Test 3: Testing textbuffer_bytes\n");

		//(A): Empty string

			//Use function
			textbuffer_bytes(empty);

			//test
			//Should be 2 (one for newline)
			assert(textbuffer_bytes(empty) == 1);
			
		//(B): Multiple consecutive newlines

			//Use function
			textbuffer_bytes(new_line);

			//tests
			assert(textbuffer_bytes(new_line) == 21);

		//(C): Several items

			//Use function
			textbuffer_bytes(tb);

			//tests
			assert(textbuffer_bytes(tb) == 26);

		//(D): NULL textbuffer (should get an error)
			//assert(textbuffer_bytes(NULL)==NULL);
			
		printf("Test 3: Passed\n");


	//TEST 4: Textbuffer swap
		printf("Test 4: Testing textbuffer swap\n");
		
		//(A): Empty string

			textbuffer_swap(empty,0,0);

			assert(strcmp(empty->first->text,"")==0);

		//(B): Multiple consecutive newlines

			textbuffer_swap(new_line,1,2);
		
			assert(strcmp(new_line->first->text,"hello")==0);
			assert(strcmp(new_line->first->next->text,"")==0);
			assert(strcmp(new_line->first->next->next->text, "there")==0);
			assert(strcmp(new_line->first->next->next->next->text,"newline")==0);
			assert(curr->next->next->next->next == NULL);
			
		//(C): Several items

			textbuffer_swap(tb,1,2);

			assert(strcmp(tb->first->text,"hello") == 0);
			assert(strcmp(tb->first->next->text,"I am sam") == 0);
			assert(strcmp(tb->first->next->next->text,"there") == 0);
			assert(strcmp(tb->first->next->next->next->text,"this") == 0);
			assert(tb->first->next->next->next->next == NULL);
			
		//(D): pos1 = pos2

			textbuffer_swap(tb,1,1);

			assert(strcmp(tb->first->text,"hello") == 0);
			assert(strcmp(tb->first->next->text,"I am sam") == 0);
			assert(strcmp(tb->first->next->next->text,"there") == 0);
			assert(strcmp(tb->first->next->next->next->text,"this") == 0);
			assert(tb->first->next->next->next->next == NULL);

		//(E): pos1 is out of bounds

			//textbuffer_swap(tb,9,1);
			//should give error (remove comment and run)

		//(F): pos1 is out of bounds (neg)

			//textbuffer_swap(tb,-9,1);
			//should give error (remove comment and run)

		//(G): One line textbuffer

			Textbuffer one_line = textbuffer_new("oneline\n\0");
			textbuffer_swap(one_line,0,0);

			assert(strcmp(one_line->first->text,"oneline") == 0);

			textbuffer_drop(one_line);

		//(H): Swap lines at the start and end of textbuffer

			textbuffer_swap(tb,0,3);

			assert(strcmp(tb->first->text,"this") == 0);
			assert(strcmp(tb->first->next->text,"I am sam") == 0);
			assert(strcmp(tb->first->next->next->text,"there") == 0);
			assert(strcmp(tb->first->next->next->next->text,"hello") == 0);
			assert(tb->first->next->next->next->next == NULL);

		printf("Test 4: Passed\n");

	//TEST 5: Testing textbuffer_search 
	printf("Test 5: Testing textbuffer_search\n");

		//(!UNSURE!)
		size_t return_value = 0;
		return_value--;

		//(A): Empty textbuffer
			textbuffer_search(empty,"",TRUE);
			assert(textbuffer_search(empty,"",FALSE)==return_value);
			
		//(B): Multiple consecutive newlines in textbuffer

			assert(textbuffer_search(new_line,"hello",TRUE)==1);
			assert(textbuffer_search(new_line,"hello",FALSE)==1);

		//(C): Several items in textbuffer

			assert(textbuffer_search(tb,"hello",TRUE)==4);
			assert(textbuffer_search(tb,"hello",FALSE)==4);

		//(D): Search for non-existing word

			assert(textbuffer_search(tb,"non-existent",TRUE)==return_value);
			assert(textbuffer_search(tb,"non-existent",FALSE)==return_value);

		//(E): Search for empty word
	
			assert(textbuffer_search(tb,"",TRUE)==return_value);
			assert(textbuffer_search(tb,"",FALSE)==return_value);

		//(F): Test the boolean setup

			assert(textbuffer_search(tb,"hello",4)==4);
			assert(textbuffer_search(tb,"hello",5)==4);


	//TEST 6: Testing textbuffer_insert
	printf("Test 6: Testing textbuffer_insert\n");

		//(A): At pos = 0
		tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
		new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

			//Use function
			textbuffer_insert(tb, 0, new_line);

			//tests
			assert(strcmp(tb->first->text,"hello")==0);
			assert(strcmp(tb->first->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->text,"")==0);
			assert(strcmp(tb->first->next->next->next->text,"newline")==0);
			assert(strcmp(tb->first->next->next->next->next->text,"hello")==0);
			assert(strcmp(tb->first->next->next->next->next->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->next->next->next->next->text,"I am sam")==0);	
			assert(strcmp(tb->first->next->next->next->next->next->next->next->text,"this")==0);	

			//drop
			textbuffer_drop(tb);

		//(B): At final line
		tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
		new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

			//Use function
			textbuffer_insert(tb, 4, new_line);

			//tests
			assert(strcmp(tb->first->text,"hello")==0);
			assert(strcmp(tb->first->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->text,"I am sam")==0);
			assert(strcmp(tb->first->next->next->next->text,"this")==0);

			assert(strcmp(tb->first->next->next->next->next->text,"hello")==0);
			assert(strcmp(tb->first->next->next->next->next->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->next->next->next->next->text,"")==0);	
			assert(strcmp(tb->first->next->next->next->next->next->next->next->text,"newline")==0);

		//(C): In middle
		tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
		new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

			//Use function
			textbuffer_insert(tb, 2, new_line);

			//tests
			assert(strcmp(tb->first->text,"hello")==0);
			assert(strcmp(tb->first->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->text,"hello")==0);
			assert(strcmp(tb->first->next->next->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->next->next->text,"")==0);
			assert(strcmp(tb->first->next->next->next->next->next->text,"newline")==0);
			assert(strcmp(tb->first->next->next->next->next->next->next->text,"I am sam")==0);	
			assert(strcmp(tb->first->next->next->next->next->next->next->next->text,"this")==0);

		//(D): Merge empty textbuffer with tb
		tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
		empty = textbuffer_new("\n\0");

			//Use function
			textbuffer_insert(tb, 2, empty);

			//tests
			assert(strcmp(tb->first->text,"hello")==0);
			assert(strcmp(tb->first->next->text,"there")==0);
			assert(strcmp(tb->first->next->next->text,"")==0);
			assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
			assert(strcmp(tb->first->next->next->next->next->text,"this")==0);


		//(E): Merge two empty textbuffers
		empty = textbuffer_new("\n\0");
		Textbuffer empty2 = textbuffer_new("\n\0");

			//Use function
			textbuffer_insert(empty,0,empty2);

			//tests
			assert(strcmp(empty->first->text,"")==0);
			assert(strcmp(empty->first->next->text,"")==0);
			assert(empty->first->next->next == NULL);

			//drop (!UNSURE!) TODO: error produced when empty tb dropped 
			//textbuffer_drop(empty2);

		//(F): pos out of bounds
		tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
		new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
			
			//Use function (should get an error)
			//textbuffer_insert(tb,9,new_line); 

		//(G): tb1 = tb2
		tb = textbuffer_new("hello\nthere\n\0");
		
			//Use function
			textbuffer_insert(tb,0,tb);

			//tests
			assert(strcmp(tb->first->text,"hello")==0);
			assert(strcmp(tb->first->next->text,"there")==0);
			assert(tb->first->next->next==NULL);

		printf("Test 6: Passed\n");
			
			
			
			

			
		
			
			



}
