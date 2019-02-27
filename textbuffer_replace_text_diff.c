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

	
	//if string is empty
	if (strcmp(input_text,"")==0) {
		
		line new_line = new_node("");

		tb->first = new_line;
		tb->last = new_line;
		tb->n_items = 0;

		return tb;
	}
	
	//if string is just one character long
	if (strlen(input_text)==1) {
	
		line new_line = new_node(input_text);

		tb->first = new_line;
		tb->last = new_line;
		tb->n_items = 1;

		return tb;
	}

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

	//If empty
	if (tb->n_items==0) {
		return NULL;
	}

	//Create string (!UNSURE! if i've done this correctly)
	char *str = malloc(textbuffer_bytes(tb));
	
	
	//Initialise curr pointer
	line curr = tb->first;
	
	//Copy string into str
	strcpy(str,curr->text);


	//printf("curr->text = %s\n", curr->text);

/*
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

		//if empty 
		if(tb1->n_items==0) {
			
			//new start
			line start = tb2->first;
			tb1->first = start;

			//new end
			line end = tb2->last;
			tb1->last =  end;

		} else {

			//Create pointer
			line curr = tb1->first;

			//Fix links
			tb1->first = tb2->first;
			tb1->first->prev = NULL;

			curr->prev = tb2->last;
			tb2->last->next = curr;
		}

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

			//if substring is present in line
			if(strstr(curr->text,match) != NULL) {
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

			//if substring is present in line
			if(strstr(curr->text,match) != NULL) {

				return line_number;
			}
		
			line_number++;
		}
	}

	return return_value;

}


// Remove lines `from` through `to` inclusive from textbuffer `tb`. If lines `from` or `to` are out of range, the program should `abort()` with an error message.
void textbuffer_delete (Textbuffer tb, size_t from, size_t to) {

	//(!UNSURE!) assumed: line 1 is from = 0 
	//edgecases

	if (tb==NULL || tb->n_items==0 || from>to || from < 0 || to>(textbuffer_lines(tb)-1)) return;


	//code
	//Create pointer for from
	line curr_from = tb->first;
	size_t count_from = 0;

	//Navigate to from
	while(curr_from!=NULL && count_from != from) {
		curr_from = curr_from->next;
		count_from++;
	}

	//printf("curr_from->text = %s\n", curr_from->text);

	//Create pointer for to
	line curr_to = tb->first;
	size_t count_to = 0;

	//Navigate to to
	while(curr_to!=NULL && count_to!= to) {
		curr_to = curr_to->next;
		count_to++;
	}

	//printf("curr_to->text = %s\n", curr_to->text);

	//Fix links
	//if from is not first
	if(curr_from->prev!=NULL) curr_from->prev->next = curr_to->next;

	//If to is not end
	if(curr_to->next!=NULL) curr_to->next->prev = curr_from->prev;

	//If from is start
	if(curr_from==tb->first) tb->first = curr_to->next;

	//If to is end
	if(curr_to==tb->last) tb->last = curr_from->prev;

	//Free everything from 'from' to 'to'
	line curr = curr_from;
	line end = curr_to->next;
	line next = curr;

	while(curr!=end) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	//change n_items
	tb->n_items = tb->n_items -to + from -1;

}

//Copy `tb2` into `tb1` at line `pos`. Afterwards, line 0 of `tb2` will be line `pos` of `tb1`, and the old line `pos` of `tb1` will follow after the last line of `tb2`.  If line `pos` is equal to the number of lines in `tb1`, `tb2` is inserted at the end.  If line `pos` is out of range, the program should `abort()` with an error message. `tb2` is unmodified and remains usable independent of `tb1`.
 
void textbuffer_paste (
	Textbuffer tb1, size_t pos, Textbuffer tb2) {

	//TODO: consider edgecases
	textbuffer_insert(tb1,pos,textbuffer_copy(tb2,0,tb2->n_items-1));
	
}

// Copy lines `from` through `to` inclusive from the textbuffer `tb`, into a new textbuffer, leaving `tb` unmodified. If lines `from` or `to` are out of range, the program should `abort()` with an error message.
Textbuffer textbuffer_copy (
	Textbuffer tb, size_t from, size_t to) {

	//if tb is null
	if(tb==NULL) return NULL;

	//if from/to out of range
	if(from>to || from>(tb->n_items-1) || to>(tb->n_items-1) || from<0 || to<0) return NULL;

	//if empty textbuffer
	if(tb->n_items==0) return textbuffer_new("");

	Textbuffer tb_copy = textbuffer_new("");

	//code
	//Create pointer for from
	line curr_from = tb->first;
	size_t count_from = 0;

	//Navigate to from
	while(curr_from!=NULL && count_from != from) {
		curr_from = curr_from->next;
		count_from++;
	}

	//printf("curr_from->text = %s\n", curr_from->text);

	//Create pointer for to
	line curr_to = tb->first;
	size_t count_to = 0;

	//Navigate to to
	while(curr_to!=NULL && count_to!= to) {
		curr_to = curr_to->next;
		count_to++;
	}

	//printf("curr_to->text = %s\n", curr_to->text);

	line prev = NULL;
	line curr = curr_from;
	tb_copy->first = NULL;
	line copy;
	
	while(curr!=curr_to->next) {
		
		//create new node
		copy = new_node(curr->text);

		//if first not initialised
		if(tb_copy->first==NULL) {
			tb_copy->first = copy;
			//printf("in fucntion: tb_copy->first->text = %s\n", tb_copy->first->text);
		}

		//if not at start of list
		if(prev!=NULL) {
			prev->next = copy;
		}

		//printf("copy->text = %s\n", copy->text);
		
		//set copy previous to be the previous pointer
		copy->prev=prev;

		prev = copy;
		
		curr = curr->next;

	}

	tb_copy->last = prev;
	tb_copy->n_items = to-from+1;

	return tb_copy;

}

//Cut lines `from` through `to` inclusive out of the textbuffer `tb`, into a new textbuffer.  The cut lines should be deleted from `tb`, and should no longer be associated with it. If lines `from` or `to` are out of range, the program should `abort()` with an error message.
Textbuffer textbuffer_cut (Textbuffer tb, size_t from, size_t to) {

	//edgecases
	if(tb==NULL || from>to || from>(tb->n_items-1) || to>(tb->n_items-1) || from<0 || to<0) return NULL;

	//if empty textbuffer
	if(tb->n_items==0) return textbuffer_new("");

	//copy the section required to be cut
	Textbuffer tb_copy = textbuffer_copy(tb,from,to);
	
	//Create empty textbuffer
	Textbuffer empty = textbuffer_new("");	

	//Paste it into an empty textbuffer
	textbuffer_paste (empty, 0, tb_copy);

	return empty;

}


// Search every line of `tb` for occurrences of `match`, and replace them all with `replace`.
void textbuffer_replace (Textbuffer tb, char *match, char *replace) {

	//edgecases
	//match is the same as replace
	//tb is null
	//tb is empty
	
	//if there is not newline after replace add one
/*
	char arr = replace[strlen(replace)];
	
	printf("here1\n");

	if (arr!="\n") {

		printf("here2\n");
		
	}

	printf("here3\n");
*/

	//copy tb
	Textbuffer tb_copy = textbuffer_copy(tb,0,tb->n_items-1);

	//Create line number is the line that match is found in
	int line_number = textbuffer_search (tb_copy, match, 0);

	while(line_number!=-1) {

		printf("tb_copy->first->text = %s\n",tb_copy->first->text);
		printf("tb_copy->first->text = %s\n",tb_copy->first->next->text);
		printf("tb_copy->first->text = %s\n",tb_copy->first->next->next->text);
		printf("tb_copy->first->text = %s\n",tb_copy->first->next->next->next->text);
		printf("tb_copy->first->text = %s\n",tb_copy->first->next->next->next->next->text);

		printf("line_number = %d\n", line_number);
		printf("tb_copy->n_items = %d\n",tb_copy->n_items);

		//change tb
		//Navigate to line number
		


		textbuffer_delete (tb_copy,line_number-1, line_number-1);

		Textbuffer replacement = textbuffer_new(replace);
		//tb_copy->n_items++;
	
		textbuffer_insert (tb_copy, line_number-1, replacement);

		line_number = textbuffer_search (tb_copy, match, 0);
		
	}


}


void white_box_tests (void) {
//interchangable shit
//When re-adding white_box_tests.c ensure you keep these lines (remove only 'Textbuffer from each of them')
Textbuffer empty;
Textbuffer new_line;
Textbuffer tb;
Textbuffer tb_copy;
Textbuffer large;

printf("\nWHITEBOX TESTS\n");


//TEST 11: Testing textbuffer_replace

	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","less\n");
	
	//tests





	
	


	


}
