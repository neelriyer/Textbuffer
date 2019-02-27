//
//  text_textbuffer.c
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

//Helper functions
static void black_box_texbuffer_new(void);
static void black_box_textbuffer_to_str(void);
static void black_box_textbuffer_bytes(void);
static void black_box_textbuffer_swap(void);
static void black_box_textbuffer_search(void);
static void black_box_textbuffer_insert(void);
static void black_box_textbuffer_delete(void);
static void black_box_textbuffer_copy(void);
static void black_box_textbuffer_copy(void);
static void black_box_textbuffer_paste(void);
static void black_box_textbuffer_cut(void);
static void black_box_textbuffer_replace(void);



int main (void) {

	//WHITEBOX TESTS
	printf("\nWHITEBOX TESTS\n");
	white_box_tests();

	//BLACKBOX TESTS	
	printf("\nBLACKBOX TESTS\n");
	black_box_texbuffer_new();
	black_box_textbuffer_to_str();
	black_box_textbuffer_bytes();
	black_box_textbuffer_swap();
	black_box_textbuffer_search();
	black_box_textbuffer_insert();
	black_box_textbuffer_delete();
	black_box_textbuffer_copy();
	black_box_textbuffer_paste();
	black_box_textbuffer_cut();
	black_box_textbuffer_replace();


	return EXIT_SUCCESS;

}


void black_box_texbuffer_new() {

//TEST 1: Testing textbuffer_new
Textbuffer empty;
Textbuffer new_line;
Textbuffer tb;
Textbuffer single_newline;

printf("Test 1: Testing textbuffer_new\n");

//(A): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

	//tests
	assert(new_line!=NULL);
	assert (textbuffer_bytes (new_line) == strlen ("hello\nthere\n\nnewline\n\0"));
	assert(textbuffer_lines (new_line) == 4);

	textbuffer_drop(new_line);

//(B): Several items 
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(tb!=NULL);
	assert (textbuffer_bytes (tb) == strlen ("hello\nthere\nI am sam\nthis\n\0"));
	assert(textbuffer_lines (tb) == 4);

	textbuffer_drop(tb);

//(C): Single newline
	//Use function
	single_newline = textbuffer_new("\n\0");

	//tests
	assert(single_newline!=NULL);
	assert(textbuffer_bytes(single_newline) == 2);
	assert(textbuffer_lines (single_newline) == 1);

	textbuffer_drop(single_newline);

}

void black_box_textbuffer_to_str() {

//TEST 2: Testing textbuffer_to_str
Textbuffer empty;
Textbuffer new_line;
Textbuffer tb;
Textbuffer single_newline;

printf("Test 2: Testing textbuffer_to_str\n");

//(A): Empty string
	//Use function
	empty = textbuffer_new("delete\n\0");
	textbuffer_delete(empty,0,0);
	char *str = textbuffer_to_str(empty);

	//tests
	assert(str==NULL || strcmp(str,"")==0);
	
	textbuffer_drop(empty);
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

}

void black_box_textbuffer_bytes() {

//TEST 3: Testing textbuffer_bytes
Textbuffer empty;
Textbuffer new_line;
Textbuffer tb;

printf("Test 3: Testing textbuffer_bytes\n");

//(A): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_bytes(new_line);

	//tests
	assert(textbuffer_bytes(new_line) == 21);

	textbuffer_drop(new_line);

//(B): Several items
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_bytes(tb);

	//tests
	assert(textbuffer_bytes(tb) == 26);

	textbuffer_drop(tb);

//(C): NULL textbuffer (should be 0)
	assert(textbuffer_bytes(NULL)==0);


}

void black_box_textbuffer_swap() {

//TEST 4: Textbuffer swap
Textbuffer new_line;
Textbuffer tb;
Textbuffer one_line;

printf("Test 4: Testing textbuffer swap\n");
		
//(A): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_swap(new_line,1,2);

	//tests
	char *str = textbuffer_to_str(new_line);
	assert(strcmp(str,"hello\n\nthere\nnewline\n\0")==0);
	assert(textbuffer_lines(new_line)==4);

	textbuffer_drop(new_line);
	free(str);
//(B): Several items
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_swap(tb,1,2);

	//tests
	char *str1 = textbuffer_to_str(tb);
	assert(strcmp(str1,"hello\nI am sam\nthere\nthis\n")==0);
	assert(textbuffer_lines(tb)==4);

	textbuffer_drop(tb);
	free(str1);
//(C): pos1 = pos2	
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_swap(tb,1,1);

	//tests
	char *str2 = textbuffer_to_str(tb);
	assert(strcmp(str2,"hello\nthere\nI am sam\nthis\n\0")==0);
	assert(textbuffer_lines(tb)==4);

	textbuffer_drop(tb);
	free(str2);
//(D): One line textbuffer
	//Use function
	one_line = textbuffer_new("oneline\n\0");
	textbuffer_swap(one_line,0,0);

	//test
	char *str3 = textbuffer_to_str(one_line);
	assert(strcmp(str3,"oneline\n\0")==0);
	assert(textbuffer_lines(one_line)==1);

	textbuffer_drop(one_line);
	free(str3);
//(E): Swap lines at the start and end of textbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_swap(tb,0,3);

	//tests
	char *str4 = textbuffer_to_str(tb);
	assert(strcmp(str4,"this\nthere\nI am sam\nhello\n\0")==0);
	assert(textbuffer_lines(tb)==4);

	textbuffer_drop(tb);
	free(str4);

}

void black_box_textbuffer_search() {

//TEST 5: Testing textbuffer_search 

Textbuffer new_line; 
Textbuffer tb;

printf("Test 5: Testing textbuffer_search\n");

//(A): Multiple consecutive newlines in textbuffer
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

	//tests
	assert(textbuffer_search(new_line,"hello",1)==1);
	assert(textbuffer_search(new_line,"hello",0)==1);

	textbuffer_drop(new_line);
//(B): Several items in textbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"hello",1)==1);
	assert(textbuffer_search(tb,"hello",0)==1);

	textbuffer_drop(tb);
//(C): Search for non-existing word
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"non-existent",1)==-1);
	assert(textbuffer_search(tb,"non-existent",0)==-1);

	textbuffer_drop(tb);
//(D): Search for empty word
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"",1)==-1);
	assert(textbuffer_search(tb,"",0)==-1);

	textbuffer_drop(tb);
//(E): Test the boolean setup
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	//tests
	assert(textbuffer_search(tb,"hello",4)==1);
	assert(textbuffer_search(tb,"hello",5)==1);

	textbuffer_drop(tb);
//(F): Multiple words
	//Use function
	tb = textbuffer_new("O, reason not the need!\n Our basest beggars\nAre in the poorest thing superfluous.\nAllow not\n\0");

	//tests
	assert(textbuffer_search(tb,"not",0)==1);
	assert(textbuffer_search(tb,"not",1)==4);

	textbuffer_drop(tb);
}

void black_box_textbuffer_insert() {

//TEST 6: Testing textbuffer_insert

Textbuffer new_line;
Textbuffer tb;

printf("Test 6: Testing textbuffer_insert\n");

//(A): At pos = 0
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_insert(tb, 0, new_line);

	//tests
	char *str1 = textbuffer_to_str(tb);
	assert(strcmp(str1,"hello\nthere\n\nnewline\nhello\nthere\nI am sam\nthis\n\0")==0);
	assert(textbuffer_lines(tb)==8);

	textbuffer_drop(tb);
	free(str1);
//(B): At final line
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_insert(tb, 4, new_line);

	//tests
	str1 = textbuffer_to_str(tb);
	assert(strcmp(str1,"hello\nthere\nI am sam\nthis\nhello\nthere\n\nnewline\n\0")==0);
	assert(textbuffer_lines(tb)==8);

	textbuffer_drop(tb);
	free(str1);
//(C): In middle
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_insert(tb, 2, new_line);

	//tests
	str1 = textbuffer_to_str(tb);
	assert(strcmp(str1,"hello\nthere\nhello\nthere\n\nnewline\nI am sam\nthis\n\0")==0);
	assert(textbuffer_lines(tb)==8);
	
	textbuffer_drop(tb);
	free(str1);

//(D): tb1 = tb2
	//Use function
	tb = textbuffer_new("hello\nthere\n\0");
	textbuffer_insert(tb,0,tb);

	//tests
	str1 = textbuffer_to_str(tb);
	assert(strcmp(str1,"hello\nthere\n\0")==0);
	assert(textbuffer_lines(tb)==2);

	textbuffer_drop(tb);	
	free(str1);
}

void black_box_textbuffer_delete() {

//TEST 7: Testing textbuffer_delete

Textbuffer new_line;
Textbuffer tb;

printf("Test 7: Testing textbuffer_delete\n");

	//(A): Delete in the middle
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,1,2);

	//tests
	assert(textbuffer_lines(tb)==2);

	textbuffer_drop(tb);	
	
	//(B): Delete at the start
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,1);

	//tests
	assert(textbuffer_lines(tb)==2);
	
	textbuffer_drop(tb);

	//(C): Delete at end
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,2,3);

	//tests
	assert(textbuffer_lines(tb)==2);

	textbuffer_drop(tb);
	
	//(D): Delete all of it
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,3);

	//tests
	assert(tb!=NULL);
	assert(textbuffer_lines(tb)==0);

	textbuffer_drop(tb);

	//(E): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_delete(new_line,0,1);

	//tests
	assert(textbuffer_lines(new_line)==2);

	textbuffer_drop(new_line);


	//(G): Textbuffer where from  = to
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,0);

	//tests
	assert(textbuffer_lines(tb)==3);

	textbuffer_drop(tb);


}

void black_box_textbuffer_copy(void) {

//TEST 8: Testing Textbuffer_copy

Textbuffer tb;
Textbuffer tb_copy;
Textbuffer new_line;
Textbuffer large;

printf("Test 8: Testing textbuffer_copy\n");

//(A): Several items (copy all)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,0,textbuffer_lines(tb)-1);

	//tests	
	assert(textbuffer_lines(tb_copy)==4);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb); 

//(B): Several items (copy beginning)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,0,2);

	//tests
	assert(textbuffer_lines(tb_copy)==3);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb); 

//(C): Several items (copy end)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,2,3);
	
	//tests
	assert(textbuffer_lines(tb_copy)==2);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb); 

//(D): Multiple consecutive newlines
	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	tb_copy = textbuffer_copy(new_line,2,3);

	//tests
	assert(textbuffer_lines(tb_copy)==2);

	textbuffer_drop(new_line);
	textbuffer_drop(tb_copy);

#if 0
//(F): Out of range
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,9,9);
	
	//tests
	assert(tb_copy==NULL);

	textbuffer_drop(tb);
	textbuffer_drop(tb_copy);

#endif

//(G): Textbuffer is null
	//Use function
	tb_copy = textbuffer_copy(NULL,0,0);
	
	//tests
	assert(tb_copy==NULL);

	textbuffer_drop(tb_copy);

#if 0
//(H): From>To
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,1,0);
	
	//tests
	assert(tb_copy==NULL);

	textbuffer_drop(tb);
	textbuffer_drop(tb_copy);

#endif
		
//(I): From = To
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_copy(tb,1,1);

	//tests
	assert(textbuffer_lines(tb_copy)==1);

	textbuffer_drop(tb);
	textbuffer_drop(tb_copy);
	
//(J): Several items (copy middle)
	//Use function
	large = textbuffer_new("first\nsecond\nthird\nfourth\nfifth\nsixth\n\0");
	tb_copy = textbuffer_copy(large,1,3);

	//tests
	assert(textbuffer_lines(tb_copy)==3);

	textbuffer_drop(large);
	textbuffer_drop(tb_copy);
	
}

void black_box_textbuffer_paste() {

//TEST 9: Testing textbuffer_paste

Textbuffer empty;
Textbuffer tb;
Textbuffer tb1;

printf("Test 9: Testing textbuffer_paste\n");

	//(A): Paste into empty textbuffer
	//Use function
	empty = textbuffer_new("delete\n\0");
	textbuffer_delete(empty,0,0);
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_paste(empty, 0, tb);

	//tests
	assert(textbuffer_lines(empty)==4);
	textbuffer_drop(empty);
	textbuffer_drop(tb);

	//(B): Paste into middle
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb1 = textbuffer_new("paste\nhere\n\0");
	textbuffer_paste(tb,2,tb1);

	//tests
	char *str = textbuffer_to_str(tb);
	assert(strcmp(str,"hello\nthere\npaste\nhere\nI am sam\nthis\n\0")==0);

	textbuffer_drop(tb1);
	textbuffer_drop(tb);
	free(str);

	//See textbuffer_insert for full range of tests

}

void black_box_textbuffer_cut() {

//TEST 10: Testing textbuffer_cut

Textbuffer tb_copy;
Textbuffer tb;

printf("Test 10: Testing textbuffer_cut\n");
	//(A): Several items (cut everything)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 0, 3);

	//tests
	assert(textbuffer_lines(tb_copy)==4);
	assert(textbuffer_lines(tb)==0);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);

	//(B): Several items (cut start)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 0, 1);

	//tests
	assert(textbuffer_lines(tb_copy)==2);
	assert(textbuffer_lines(tb)==2);	

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);

	//(C): Several items (cut end)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 2, 3);

	//tests
	assert(textbuffer_lines(tb_copy)==2);
	assert(textbuffer_lines(tb)==2);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);
	
	//(D): Several items (cut one line)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 2, 3);

	//tests
	assert(textbuffer_lines(tb_copy)==2);
	assert(textbuffer_lines(tb)==2);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);
	
	//(F): From = to
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut(tb, 1, 1);

	//test
	assert(textbuffer_lines(tb_copy)==1);
	assert(textbuffer_lines(tb)==3);

	textbuffer_drop(tb_copy);
	textbuffer_drop(tb);

#if 0
	//(G): Textbuffer is null
	//Use function
	tb_copy = textbuffer_cut(NULL,0,0);

	//test
	assert(tb_copy==NULL);

	textbuffer_drop(tb_copy);

#endif

}

void black_box_textbuffer_replace() {

//TEST 11: Testing textbuffer_replace

Textbuffer tb;

printf("Test 11: Testing textbuffer_replace\n");

	//(A): Replace large textbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","less");
	
	//tests
	char *str1 = textbuffer_to_str(tb);
	assert(strcmp(str1,"hello\nthere\nless words\nI am sam\nless words\nless words\n\0")==0);

	textbuffer_drop(tb);
	free(str1);

	//(D): Match is not found in Texbuffer
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "not found","less");

	//tests
	char *str2 = textbuffer_to_str(tb);
	assert(strcmp(str2,"hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0")==0);

	textbuffer_drop(tb);
	free(str2);	
	
	//(E): Case sensitive test
	//Use function
	tb = textbuffer_new("hello\nthere\nMore words\nI am sam\nMore words\nMore words\n\0");
	textbuffer_replace (tb, "more","less");

	//tests	
	char *str3 = textbuffer_to_str(tb);
	assert(strcmp(str3,"hello\nthere\nMore words\nI am sam\nMore words\nMore words\n\0")==0);

	textbuffer_drop(tb);
	free(str3);	
	
	//(F): match is the same as replace
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","more");

	//tests
	char *str4 = textbuffer_to_str(tb);
	assert(strcmp(str4,"hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0")==0);

	textbuffer_drop(tb);
	free(str4);


	//(G): Replace = ""
	//Use function
	tb = textbuffer_new("hello\nthere\nmore words\nI am sam\nmore words\nmore words\n\0");
	textbuffer_replace (tb, "more","");

	//tests
	char *str5 = textbuffer_to_str(tb);
	assert(strcmp(str5,"hello\nthere\n words\nI am sam\n words\n words\n\0")==0);

	textbuffer_drop(tb);
	free(str5);

	//(H) Complex replace
	//Use function
	tb = textbuffer_new("hello\nyellow\n\0");
	textbuffer_replace(tb,"ello","f");

	//tests
	char *str6 = textbuffer_to_str(tb);
	assert(strcmp(str6,"hf\nyfw\n\0")==0);

	textbuffer_drop(tb);
	free(str6);

#if 0
//TODO account for immutable strings
	//(I): Immutable string
	//Use function
	tb = textbuffer_new("hello\nyellow berry\ngreen\n\0");
	char *immutable_string = "immutable\n\0";
	textbuffer_replace(tb,"yellow",immutable_string);

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"immutable berry")==0);
	assert(strcmp(tb->first->next->next->text,"green")==0);
#endif

}

