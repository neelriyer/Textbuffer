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

static line new_node (char *item);
static line traverse(Textbuffer tb, size_t pos, bool rev);



//Allocate a new textbuffer, whose contents is initialised with the text given in the array. The lines in the input array are all terminated by a `'\\n'`. The whole text is terminated by a `'\\0'`.

Textbuffer textbuffer_new (const char *input_text) {
	
	//If text is NULL
	if (input_text == NULL) {
		abort();
	}

	//Create node
	Textbuffer tb = malloc(sizeof(*tb));

    if (tb == NULL) {
        fprintf (stderr, "couldn't allocate memory\n");
        abort();
    }
	//if string is empty
	//this bit shouldn't be here
	//TODO (look at safari)
	if (strcmp(input_text,"")==0) {
		
		line new_line = new_node("");

		tb->first = new_line;
		tb->last = new_line;
		tb->n_items = 0;

		return tb;
	}

	//create str
	char *str = strdup(input_text);

	if (str == NULL) {
        fprintf (stderr, "couldn't allocate memory\n");
        abort();
    }
	
	//if string is just one character long
	if (strlen(input_text)==1) {

		line new_line = new_node(str);

		tb->first = new_line;
		tb->last = new_line;
		tb->n_items = 1;

		return tb;
	}

	//Initialise variables
	tb->n_items = 0;
	tb->first = NULL;
	tb->last = NULL;

	//Remove the last newline from end of string
	str[strlen(str)-1] = '\0';

	//Use strsep function to seperate string by delimiters
	char *token = strsep (&str, "\n");

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
        token = strsep(&str, "\n");
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
    line next=NULL, curr=NULL;
	if(tb!=NULL) curr = tb->first;

    while (curr!= NULL) {

        next = curr->next;
		free(curr->text);
        free(curr);
        curr = next;

    }
	free(tb);
}

//Return the number of lines of text stored in the given textbuffer.
size_t textbuffer_lines (Textbuffer tb) {

	if(tb==NULL) return 0;
	return tb->n_items;
}


//Return the number of bytes of text stored in the given textbuffer, counting the newlines that would be needed.
size_t textbuffer_bytes (Textbuffer tb) {

	//Initialise size
	size_t size=0;

	//if null
	if(tb==NULL) return size;

	//Initialise pointer
	line curr = tb->first;

	if(tb->n_items==0) return size;

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
//TODO: create an extra newline at end. remove it.
char *textbuffer_to_str (Textbuffer tb) {

	//If empty
	if (tb->n_items==0) {
		return NULL;
	}

	//Create string (+1 add end is for newline for last line)
	char *str = malloc(textbuffer_bytes(tb)+1);

	if (str == NULL) {
        fprintf (stderr, "couldn't allocate memory\n");
        abort();
    }
	
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

		//Move to next node
		curr = curr->next;

		//Concatenate new line
		strcat(str,"\n");
	}

	return str;

}


// Swap the two given lines in the textbuffer. If line `pos1` or line `pos2` are out of range, the program should `abort()` with an error message.  Lines are zero-indexed; that is, the first line of a textbuffer is at position 0. Your implementation should not swap lines by value; instead, you should manipulate your linked data structure.
void textbuffer_swap (Textbuffer tb, size_t pos1, size_t pos2) {

	if (tb==NULL) {
		return;
	}

	size_t index = tb->n_items - 1;

	//Edgecases (TODO consider more)
	if(pos1>index || pos2>index) {
		printf("Error out of bounds\n");
		abort();
	}
	
	if (index == 0 ||(pos1 == pos2)) {
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

	} else {

		//CASE 2: If not next to each other
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

void textbuffer_insert (Textbuffer tb1, size_t pos, Textbuffer tb2) {

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

	} else {
		
		//CASE 3: INSERT IN MIDDLE
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

ssize_t textbuffer_search (Textbuffer tb, char *match, bool rev) {

	//edgecases
	if(tb==NULL || strlen(match)==0 || match==NULL) {
		return -1;
	}

	//if not bool
	if (rev !=0 && rev !=1) {
		return -1;
	}

/*
	printf("rev = %d\n", rev);
	printf("match is %s\n", match);
	printf("strlen(match) = %d\n", strlen(match));
	
*/
	//rev is true
	if (rev==TRUE || rev==1) {

		int line_number = (int)(tb->n_items);
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

		int line_number = 1;
		line curr;
		
		for (curr = tb->first;curr!=NULL;curr = curr->next) {

			//if substring is present in line
			if(strstr(curr->text,match) != NULL) {

				return line_number;
			}
		
			line_number++;
		}
	}

	return -1;

}


// Remove lines `from` through `to` inclusive from textbuffer `tb`. If lines `from` or `to` are out of range, the program should `abort()` with an error message.

void textbuffer_delete (Textbuffer tb, size_t from, size_t to) {

	//edgecases (TODO are there more?)
	if (tb==NULL || tb->n_items==0) return;

	
	if(from>to || to>(textbuffer_lines(tb)-1)) {
		abort();
	}

	//Navigate to the line at position = from
	line curr_from = traverse(tb,from,0);

	//Navigate to the line at position = to	
	line curr_to = traverse(tb,to,1);

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

	//Effectively the same as insert
	textbuffer_insert(tb1,pos,textbuffer_copy(tb2,0,tb2->n_items-1));
	
}

// Copy lines `from` through `to` inclusive from the textbuffer `tb`, into a new textbuffer, leaving `tb` unmodified. If lines `from` or `to` are out of range, the program should `abort()` with an error message.
Textbuffer textbuffer_copy (
	Textbuffer tb, size_t from, size_t to) {

	//if tb is null
	if(tb==NULL) return NULL;

	//if from/to out of range
	if(from>to || from>(tb->n_items-1) || to>(tb->n_items-1)) return NULL;

	//if empty textbuffer
	if(tb->n_items==0) return NULL;

	//Create empty textbuffer
	Textbuffer tb_copy = textbuffer_new("copy\n\0");
	textbuffer_delete(tb_copy,0,0);

	//Navigate to the line at position = from
	line curr_from = traverse(tb,from,0);

	//Navigate to the line at position = to
	line curr_to = traverse(tb,to,1);

	//Create ptrs
	line prev = NULL;
	line curr = curr_from;

	//create new node
	line copy = new_node(curr->text);

	//Initialise first node
	tb_copy->first = copy;
	//printf("in fucntion: tb_copy->first->text = %s\n", tb_copy->first->text);
	//printf("copy->text = %s\n", copy->text);
	
	//set copy previous to be the previous pointer
	copy->prev=prev;

	//Move ptrs along
	prev = copy;
	curr = curr->next;

	while(curr!=curr_to->next) {
		
		//create new node
		copy = new_node(curr->text);

		//Fix links for prev
		prev->next = copy;

		//printf("copy->text = %s\n", copy->text);
		
		//set copy previous to be the previous pointer
		copy->prev=prev;

		//Move ptrs along
		prev = copy;
		curr = curr->next;

	}
	
	//Fix tb_copy->last
	tb_copy->last = copy;

	//Fix n_items
	tb_copy->n_items = to-from+1;

	return tb_copy;

}

//Cut lines `from` through `to` inclusive out of the textbuffer `tb`, into a new textbuffer.  The cut lines should be deleted from `tb`, and should no longer be associated with it. If lines `from` or `to` are out of range, the program should `abort()` with an error message.
//TODO add erro message for program abortion
Textbuffer textbuffer_cut (Textbuffer tb, size_t from, size_t to) {

	//edgecases
	if(tb==NULL ||from>to || from>(tb->n_items-1) || to>(tb->n_items-1)) return NULL;

	//if empty textbuffer
	if(tb->n_items==0) return textbuffer_new("");

	//copy the section required to be cut
	Textbuffer tb_copy = textbuffer_copy(tb,from,to);

	//delete the section that was cut
	textbuffer_delete(tb,from,to);
	
	//Create empty textbuffer
	Textbuffer empty = textbuffer_new("delete\n\0");
	textbuffer_delete(empty,0,0);	

	//Paste it into an empty textbuffer
	textbuffer_paste (empty, 0, tb_copy);

	return empty;

}


// Search every line of `tb` for occurrences of `match`, and replace them all with `replace`.
void textbuffer_replace (Textbuffer tb, char *match, char *replace) {

	//Edgecases:
	//-If tb is null
	//-Textbuffer is empty (string is empty)
	//-Match is the same as replace
	//-Match is not found in Textbuffer
	if(tb==NULL || tb->n_items==0 || strcmp(match,replace)==0 || (int)textbuffer_search(tb,match,0)==-1) return;

	//create string
	char *string = textbuffer_to_str(tb);

	//Create buffer of appropriate size and assign pointer
    char *buffer=malloc(strlen(string)+1), *ptr = string;

	if (buffer == NULL) {
        fprintf (stderr, "couldn't allocate memory\n");
        abort();
    }
	
	//Search for substring in string
	ptr = strstr(ptr,match);

	//While substring is present in string
    while(ptr!=NULL){

		//Copy into buffer
        strncpy(buffer, string, (size_t)(ptr-string));

		//End buffer (Not guaranteed to be NULL terminated)
        buffer[ptr-string] = '\0';

		//Concenate replacement
        strncat(buffer, replace, strlen(replace));

		size_t length = strlen(ptr+strlen(match));

        strncat(buffer, ptr+strlen(match), length);

        strncpy(string, buffer, strlen(buffer));

        ptr++;
		
		//Re-check if substring is present
		ptr = strstr(ptr,match);
    }

	//empty textbuffer
	textbuffer_delete(tb,0,tb->n_items-1);

	//create modified textbuffer
	Textbuffer tb_mod = textbuffer_new(string);

	//Insert tb_mod into tb
	textbuffer_insert(tb, 0, tb_mod);

}

void textbuffer_undo (Textbuffer tb) {
	return;
}

void textbuffer_redo (Textbuffer tb) {
	return;
}

char *textbuffer_diff (Textbuffer tb1, Textbuffer tb2) {
	return "";
}

void white_box_tests (void) {

//Declarations
//Textbuffer empty;
Textbuffer new_line;
Textbuffer tb;
Textbuffer tb_copy;
Textbuffer large;
Textbuffer tb1;
Textbuffer tb_null;

//TEST 1: Testing textbuffer_new
printf("Test 1: Testing textbuffer_new\n");

//(B): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

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

	textbuffer_drop(new_line);
//(C): Several items 
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

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

	textbuffer_drop(tb);
//(D): single newline
	//Use function
	Textbuffer single_newline = textbuffer_new("\n\0");

	//tests
	assert(strcmp(single_newline->first->text,"\n")==0);

	textbuffer_drop(single_newline);
//TEST 2: Testing textbuffer_to_str
printf("Test 2: Testing textbuffer_to_str\n");

//(A): NULL textbuffer
	//Use function
	tb_null = textbuffer_new(NULL);
	char *str = textbuffer_to_str(tb_null);

	//tests
	assert(str==NULL || strcmp(str,"")==0);
	free(str);
//(B): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	str = textbuffer_to_str(new_line);

	//tests
	assert(strcmp("hello\nthere\n\nnewline\n\0",str)==0);

	textbuffer_drop(new_line);
	free(str);
//(C): Several items
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	str = textbuffer_to_str(tb);

	//tests
	assert(strcmp("hello\nthere\nI am sam\nthis\n\0",str)==0);

	textbuffer_drop(tb);
	free(str);


//TEST 3: Testing textbuffer_bytes
printf("Test 3: Testing textbuffer_bytes\n");

//(A): Empty string
	//Use function
	textbuffer_bytes("");

	//test
	assert(textbuffer_bytes("") == 0);
//(B): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_bytes(new_line);

	//tests
	assert(textbuffer_bytes(new_line) == 21);

	textbuffer_drop(new_line);
//(C): Several items
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_bytes(tb);

	//tests
	assert(textbuffer_bytes(tb) == 26);

	textbuffer_drop(tb);
//(D): NULL textbuffer (should get an error)
	//Use function & test
	assert(textbuffer_bytes(NULL)==0);


//TEST 4: Textbuffer swap
printf("Test 4: Testing textbuffer swap\n");

//(B): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_swap(new_line,1,2);

	//test
	assert(strcmp(new_line->first->text,"hello")==0);
	assert(strcmp(new_line->first->next->text,"")==0);
	assert(strcmp(new_line->first->next->next->text, "there")==0);
	assert(strcmp(new_line->first->next->next->next->text,"newline")==0);
	assert(new_line->first->next->next->next->next == NULL);

	textbuffer_drop(new_line);

//(C): Several items
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_swap(tb,1,2);

	//test
	assert(strcmp(tb->first->text,"hello") == 0);
	assert(strcmp(tb->first->next->text,"I am sam") == 0);
	assert(strcmp(tb->first->next->next->text,"there") == 0);
	assert(strcmp(tb->first->next->next->next->text,"this") == 0);
	assert(tb->first->next->next->next->next == NULL);
	
	textbuffer_drop(tb);

//(D): pos1 = pos2
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_swap(tb,1,1);

	//tests
	assert(strcmp(tb->first->text,"hello") == 0);
	assert(strcmp(tb->first->next->text,"there") == 0);
	assert(strcmp(tb->first->next->next->text,"I am sam") == 0);
	assert(strcmp(tb->first->next->next->next->text,"this") == 0);
	assert(tb->first->next->next->next->next == NULL);

	textbuffer_drop(tb);

//(G): One line textbuffer
	//Use function
	Textbuffer one_line = textbuffer_new("oneline\n\0");
	textbuffer_swap(one_line,0,0);

	//tests
	assert(strcmp(one_line->first->text,"oneline") == 0);

	textbuffer_drop(one_line);
//(H): Swap lines at the start and end of textbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_swap(tb,0,3);

	//tests
	assert(strcmp(tb->first->text,"this") == 0);
	assert(strcmp(tb->first->next->text,"there") == 0);
	assert(strcmp(tb->first->next->next->text,"I am sam") == 0);
	assert(strcmp(tb->first->next->next->next->text,"hello") == 0);
	assert(tb->first->next->next->next->next == NULL);

	textbuffer_drop(tb);


//TEST 5: Testing textbuffer_search 
printf("Test 5: Testing textbuffer_search\n");


//(B): Multiple consecutive newlines in textbuffer
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

	//tests
	assert(textbuffer_search(new_line,"hello",TRUE)==1);
	assert(textbuffer_search(new_line,"hello",FALSE)==1);

	textbuffer_drop(new_line);

//(C): Several items in textbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"hello",TRUE)==1);
	assert(textbuffer_search(tb,"hello",FALSE)==1);
	
	textbuffer_drop(tb);

//(D): Search for non-existing word
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"non-existent",TRUE)==-1);
	assert(textbuffer_search(tb,"non-existent",FALSE)==-1);

	textbuffer_drop(tb);

//(E): Search for empty word
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"",TRUE)==-1);
	assert(textbuffer_search(tb,"",FALSE)==-1);

	textbuffer_drop(tb);

//(F): Test the boolean setup
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"hello",4)==1);
	assert(textbuffer_search(tb,"hello",5)==1);

	textbuffer_drop(tb);

//(G): Multiple words
	//Use function
	tb = textbuffer_new("O, reason not the need!\n Our basest beggars\nAre in the poorest thing superfluous.\nAllow not\n\0");

	//tests
	assert(textbuffer_search(tb,"not",0)==1);
	assert(textbuffer_search(tb,"not",1)==4);

	textbuffer_drop(tb);


//TEST 6: Testing textbuffer_insert
printf("Test 6: Testing textbuffer_insert\n");

//(A): At pos = 0
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
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
	assert(tb->first->next->next->next->next->next->next->next->next==NULL);	

	textbuffer_drop(tb);

//(B): At final line
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
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
	assert(tb->first->next->next->next->next->next->next->next->next==NULL);

	textbuffer_drop(tb);

//(C): In middle
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
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

	textbuffer_drop(tb);



//(G): tb1 = tb2
	//Use function
	tb = textbuffer_new("hello\nthere\n\0");
	textbuffer_insert(tb,0,tb);

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(tb->first->next->next==NULL);

	textbuffer_drop(tb);



//TEST 7: Testing textbuffer_delete
printf("Test 7: Testing textbuffer_delete\n");

	//(A): Delete in the middle
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,1,2);

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"this")==0);

	textbuffer_drop(tb);	
	
	//(B): Delete at the start
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,1);

	//tests
	assert(strcmp(tb->first->text,"I am sam")==0);
	assert(strcmp(tb->first->next->text,"this")==0);
	assert(strcmp(tb->last->text,"this")==0);
	
	textbuffer_drop(tb);

	//(C): Delete at end
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,2,3);

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->last->text,"there")==0);

	textbuffer_drop(tb);
	
	//(D): Delete all of it
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,3);

	//tests
	assert(tb->first==NULL);
	assert(tb->last==NULL);

	textbuffer_drop(tb);

	//(E): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_delete(new_line,0,1);

	//tests
	assert(strcmp(new_line->first->text,"")==0);
	assert(strcmp(new_line->first->next->text,"newline")==0);

	textbuffer_drop(new_line);

	//(G): Textbuffer where from  = to
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,0);

	//tests
	assert(strcmp(tb->first->text,"there")==0);
	assert(strcmp(tb->first->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->text,"this")==0);
	assert(strcmp(tb->last->text,"this")==0);

	textbuffer_drop(tb);


//TEST 8: Testing Textbuffer_copy
printf("Test 8: Testing textbuffer_copy\n");

//(A): Several items (copy all)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,0,tb->n_items-1);

	//tests	
	assert(strcmp(tb_copy->first->text,"hello")==0);
	assert(strcmp(tb_copy->first->next->text,"there")==0);
	assert(strcmp(tb_copy->first->next->next->text,"I am sam")==0);
	assert(strcmp(tb_copy->first->next->next->next->text,"this")==0);
	assert(tb_copy->last->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb); 

//(B): Several items (copy beginning)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,0,2);

	//tests
	assert(strcmp(tb_copy->first->text,"hello")==0);
	assert(strcmp(tb_copy->first->next->text,"there")==0);
	assert(strcmp(tb_copy->first->next->next->text,"I am sam")==0);
	assert(tb_copy->last->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb); 

//(C): Several items (copy end)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,2,3);
	
	//tests
	assert(strcmp(tb_copy->first->text,tb->first->next->next->text)==0);
	assert(strcmp(tb_copy->first->next->text,tb->first->next->next->next->text)==0);
	assert(tb_copy->last->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb); 

//(D): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	tb_copy = textbuffer_copy(new_line,2,3);

	//tests
	assert(strcmp(tb_copy->first->text,new_line->first->next->next->text)==0);
	assert(strcmp(tb_copy->first->next->text,new_line->first->next->next->next->text)==0);
	assert(tb_copy->last->next==NULL);

	textbuffer_drop(new_line);
	textbuffer_drop(tb_copy);


//(F): Out of range
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,9,9);
	
	//tests
	assert(tb_copy==NULL);

	textbuffer_drop(tb);
	textbuffer_drop(tb_copy);

//(G): Textbuffer is null
	//Use function
	tb_copy = textbuffer_copy(NULL,0,0);
	
	//tests
	assert(tb_copy==NULL);

	textbuffer_drop(tb_copy);

//(H): From>To
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,1,0);
	
	//tests
	assert(tb_copy==NULL);

	textbuffer_drop(tb);
	textbuffer_drop(tb_copy);
		
//(I): From = To
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,1,1);

	//tests
	assert(strcmp(tb_copy->first->text,tb->first->next->text)==0);
	assert(tb_copy->first->next==NULL);

	textbuffer_drop(tb);
	textbuffer_drop(tb_copy);
	
//(J): Several items (copy middle)
	//Use function
	large = textbuffer_new("first\nsecond\nthird\nfourth\nfifth\nsixth\n\0");
	tb_copy = textbuffer_copy(large,1,3);

	//tests
	assert(strcmp(tb_copy->first->text,"second")==0);
	assert(strcmp(tb_copy->first->next->text,"third")==0);
	assert(strcmp(tb_copy->first->next->next->text,"fourth")==0);
	assert(tb_copy->first->next->next->next==NULL);

	textbuffer_drop(large);
	textbuffer_drop(tb_copy);

//TEST 9: Testing textbuffer_paste
printf("Test 9: Testing textbuffer_paste\n");

	//(B): Paste into middle
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb1 = textbuffer_new("paste\nhere\n\0");
	textbuffer_paste(tb,2,tb1);

	//tests
	str = textbuffer_to_str(tb);
	assert(strcmp(str,"hello\nthere\npaste\nhere\nI am sam\nthis\n\0")==0);

	textbuffer_drop(tb1);
	textbuffer_drop(tb);
	free(str);

	//See textbuffer_insert for full range of tests

//TEST 10: Testing textbuffer_cut
printf("Test 10: Testing textbuffer_cut\n");
	//(A): Several items (cut everything)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 0, 3);

	//tests
	assert(strcmp(tb_copy->first->text,"hello")==0);
	assert(strcmp(tb_copy->first->next->text,"there")==0);
	assert(strcmp(tb_copy->first->next->next->text,"I am sam")==0);
	assert(strcmp(tb_copy->first->next->next->next->text,"this")==0);
	assert(tb_copy->first->next->next->next->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);

	//(B): Several items (cut start)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 0, 1);

	//tests
	assert(strcmp(tb_copy->first->text,"hello")==0);
	assert(strcmp(tb_copy->first->next->text,"there")==0);
	assert(tb_copy->first->next->next==NULL);	

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);

	//(C): Several items (cut end)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 2, 3);

	//tests
	assert(strcmp(tb_copy->first->text,"I am sam")==0);
	assert(strcmp(tb_copy->first->next->text,"this")==0);
	assert(tb_copy->first->next->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);
	
	//(D): Several items (cut one line)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 2, 3);

	//tests
	assert(strcmp(tb_copy->first->text,"I am sam")==0);
	assert(strcmp(tb_copy->first->next->text,"this")==0);
	assert(tb_copy->first->next->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);
	
	//(E): From, to out of bounds
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut(tb, 9, 10);

	//test
	assert(tb_copy==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);
	
	//(F): From = to
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut(tb, 1, 1);

	//test
	assert(strcmp(tb_copy->first->text,"there")==0);
	assert(tb_copy->first->next==NULL);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);

	//(G): Textbuffer is null
	//Use function
	tb_copy = textbuffer_cut(NULL,0,0);

	//test
	assert(tb_copy==NULL);

	textbuffer_drop(tb_copy);
	

//TEST 11: Testing textbuffer_replace
printf("Test 11: Testing textbuffer_replace\n");

	//(A): Replace large textbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","less");
	
	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text,"less words")==0);
	assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->next->text,"less words")==0);
	assert(strcmp(tb->first->next->next->next->next->next->text,"less words")==0);

	textbuffer_drop(tb);


	//(D): Match is not found in Texbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "not found","less");

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text,"more words")==0);
	assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->next->text,"more words")==0);
	assert(strcmp(tb->first->next->next->next->next->next->text,"more words")==0);

	textbuffer_drop(tb);	
	
	//(E): Case sensitive test
	//Use function
	tb = textbuffer_new("hello\nthere\nMore words\nI am sam\nMore words\nMore words\n\0");
	textbuffer_replace (tb, "more","less");

	//tests	
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text,"More words")==0);
	assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->next->text,"More words")==0);
	assert(strcmp(tb->first->next->next->next->next->next->text,"More words")==0);
	
	textbuffer_drop(tb);
	
	//(F): match is the same as replace
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","more");

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text,"more words")==0);
	assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->next->text,"more words")==0);
	assert(strcmp(tb->first->next->next->next->next->next->text,"more words")==0);

	textbuffer_drop(tb);

	//(G): Replace = ""
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","");

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text," words")==0);
	assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->next->text," words")==0);
	assert(strcmp(tb->first->next->next->next->next->next->text," words")==0);

	textbuffer_drop(tb);

	//(H) Complex replace
	//Use function
	tb = textbuffer_new("hello\nyellow\n\0");
	textbuffer_replace(tb,"ello","");

	//tests
	assert(strcmp(tb->first->text,"h")==0);
	assert(strcmp(tb->first->next->text,"yw")==0);

	textbuffer_drop(tb);

/*//TODO account for immutable strings
	//(I): Immutable string
	//Use function
	tb = textbuffer_new("hello\nyellow berry\ngreen\n\0");
	char *immutable_string = "immutable\n\0";
	textbuffer_replace(tb,"yellow",immutable_string);

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"immutable berry")==0);
	assert(strcmp(tb->first->next->next->text,"green")==0);
	
*/

}

//TODO create more helper functions
//Create a new node, initialised with the item provided.
//Obtained from lists.c lab01
static line new_node (char *item) {

	line n = malloc(sizeof(*n));

    if (n == NULL) {
        fprintf (stderr, "couldn't allocate memory\n");
        abort();
    }
	
	char *str = strdup(item);

	if (str == NULL) {
        fprintf (stderr, "couldn't allocate memory\n");
        abort();
    }

    n->text = str;
    n->next = NULL;
	n->prev = NULL;

    return n;
}


//Traverses linked list until curr reaches pos. Returns line at pos. If rev is 0 curr starts from the beginning. If rev is 1 curr starts from the end.
static line traverse(Textbuffer tb, size_t pos, bool rev) {

	if (tb==NULL) return NULL;

	line curr=NULL;

	//start from beginning 
	if(rev==0) {

		//Create pointer for first
		curr = tb->first;
		size_t count = 0;

		//Navigate to pos
		while(curr!=NULL && count!= pos) {
			curr = curr->next;
			count++;
		}
	} 
	
	//start from end
	else if (rev==1) {

		//Create pointer for first
		curr = tb->last;
		size_t count = tb->n_items-1;

		//Navigate to pos
		while(curr!=NULL && count!= pos) {
			curr = curr->prev;
			count--;
		}
	}
	
	return curr;

}
