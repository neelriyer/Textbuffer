//TEST 1: Testing textbuffer_new
printf("Test 1: Testing textbuffer_new\n");

//(A): Empty textbuffer

	//Use function
	empty = textbuffer_new("");

	//tests
	//textbuffer is not NULL
	assert(empty!=NULL);

	//textbuffer first is not null
	assert(empty->first!=NULL);

	//textbuffer last is not null
	assert(empty->last!=NULL);

	//textbuffer is empty
	assert(empty->n_items == 0);

	//textbuffer first line is ""
	assert(strcmp(empty->first->text,"")==0);

	//textbuffer first->next is NULL
	assert(empty->first->next == NULL);

	//textbuffer last->next is NULL
	assert(empty->last->next == NULL);


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


//TEST 2: Testing textbuffer_to_str
printf("Test 2: Testing textbuffer_to_str\n");

//(A): Empty string

	//Use function
	empty = textbuffer_new("");
	char *str = textbuffer_to_str(empty);

	//tests
	//str should be empty
	assert(str==NULL);

//(B): Multiple consecutive newlines

	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
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
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
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


//TEST 3: Testing textbuffer_bytes
printf("Test 3: Testing textbuffer_bytes\n");

//(A): Empty string

	//Use function
	textbuffer_bytes(empty);

	//test
	//Should be 1
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


//TEST 5: Testing textbuffer_search 
printf("Test 5: Testing textbuffer_search\n");

//(!UNSURE!) TODO find better way to do this
size_t return_value = 0;
return_value--;

//(A): Empty textbuffer
	empty = textbuffer_new("");
	textbuffer_search(empty,"",TRUE);

	assert(textbuffer_search(empty,"",FALSE)==return_value);
	
//(B): Multiple consecutive newlines in textbuffer
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");

	assert(textbuffer_search(new_line,"hello",TRUE)==1);
	assert(textbuffer_search(new_line,"hello",FALSE)==1);

//(C): Several items in textbuffer
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");

	assert(textbuffer_search(tb,"hello",TRUE)==1);
	assert(textbuffer_search(tb,"hello",FALSE)==1);

//(D): Search for non-existing word

	assert(textbuffer_search(tb,"non-existent",TRUE)==return_value);
	assert(textbuffer_search(tb,"non-existent",FALSE)==return_value);

//(E): Search for empty word

	assert(textbuffer_search(tb,"",TRUE)==return_value);
	assert(textbuffer_search(tb,"",FALSE)==return_value);

//(F): Test the boolean setup

	assert(textbuffer_search(tb,"hello",4)==1);
	assert(textbuffer_search(tb,"hello",5)==1);

//(G): Multiple words
		
	tb = textbuffer_new("O, reason not the need!\n Our basest beggars\nAre in the poorest thing superfluous.\nAllow not\n\0");

	assert(textbuffer_search(tb,"not",0)==1);
	assert(textbuffer_search(tb,"not",1)==4);

	
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
	assert(tb->first->next->next->next->next->next->next->next->next==NULL);	

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
	assert(tb->first->next->next->next->next->next->next->next->next==NULL);

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
empty = textbuffer_new("");

	//Use function
	textbuffer_insert(tb, 2, empty);

	//tests
	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text,"")==0);
	assert(strcmp(tb->first->next->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->next->text,"this")==0);


//(E): Merge two empty textbuffers
	empty = textbuffer_new("");
	Textbuffer empty2 = textbuffer_new("");

	//Use function
	textbuffer_insert(empty,0,empty2);

	//tests
	assert(strcmp(empty->first->text,"")==0);
	assert(empty->first->next == NULL);

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

//TEST 7: Testing textbuffer_delete
printf("Test 7: Testing textbuffer_delete\n");
	
	//(!UNSURE!) if you change pos indexing increment the numbers in textbuffer_delete by 1
	//(A): Delete in the middle
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,1,2);

	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"this")==0);

	//(B): Delete at the start
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,1);

	assert(strcmp(tb->first->text,"I am sam")==0);
	assert(strcmp(tb->first->next->text,"this")==0);
	assert(strcmp(tb->last->text,"this")==0);
	
	//(C): Delete at end
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,2,3);

	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->last->text,"there")==0);
	
	//(D): Delete all of it
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,3);

	assert(tb->first==NULL);
	assert(tb->last==NULL);

	//(E): Multiple consecutive newlines
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	textbuffer_delete(new_line,0,1);

	assert(strcmp(new_line->first->text,"")==0);
	assert(strcmp(new_line->first->next->text,"newline")==0);
	
	//(F): Empty Textbuffer
	empty = textbuffer_new("");
	textbuffer_delete(tb,0,0);

	assert(tb->first==NULL);
	assert(tb->last==NULL);

	//(G): Textbuffer where from  = to
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,0,0);

	assert(strcmp(tb->first->text,"there")==0);
	assert(strcmp(tb->first->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->text,"this")==0);
	assert(strcmp(tb->last->text,"this")==0);
	
	//(H): Textbuffer where from is greater than to
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_delete(tb,2,1);

	assert(strcmp(tb->first->text,"hello")==0);
	assert(strcmp(tb->first->next->text,"there")==0);
	assert(strcmp(tb->first->next->next->text,"I am sam")==0);
	assert(strcmp(tb->first->next->next->next->text,"this")==0);

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
	
//(B): Several items (copy beginning)

	//Use function
	tb_copy = textbuffer_copy(tb,0,2);

	//tests
	assert(strcmp(tb_copy->first->text,"hello")==0);
	assert(strcmp(tb_copy->first->next->text,"there")==0);
	assert(strcmp(tb_copy->first->next->next->text,"I am sam")==0);
	assert(tb_copy->last->next==NULL);
	
//(C): Several items (copy end)

	//Use function
	tb_copy = textbuffer_copy(tb,2,3);

	//tests
	assert(strcmp(tb_copy->first->text,tb->first->next->next->text)==0);
	assert(strcmp(tb_copy->first->next->text,tb->first->next->next->next->text)==0);
	assert(tb_copy->last->next==NULL);
	
//(D): Multiple consecutive newlines

	//Use function
	new_line = textbuffer_new("hello\nthere\n\nnewline\n\0");
	tb_copy = textbuffer_copy(tb,2,3);

	//tests
	assert(strcmp(tb_copy->first->text,tb->first->next->next->text)==0);
	assert(strcmp(tb_copy->first->next->text,tb->first->next->next->next->text)==0);
	assert(tb_copy->last->next==NULL);
	

//(E): Empty string

	//Use function
	empty = textbuffer_new("");
	tb_copy = textbuffer_copy(empty,0,0);

	//tests	
	assert(strcmp(tb_copy->first->text,"")==0);

//(F): Out of range

	//Use function
	tb_copy = textbuffer_copy(tb,9,9);
	
	//tests
	assert(tb_copy==NULL);

//(G): Textbuffer is null

	//Use function
	tb_copy = textbuffer_copy(NULL,0,0);
	
	//tests
	assert(tb_copy==NULL);

//(H): From>To

	//Use function
	tb_copy = textbuffer_copy(tb,1,0);

	//tests
	assert(tb_copy==NULL);
		
//(I): From = To

	//Use function
	tb_copy = textbuffer_copy(tb,1,1);

	//tests
	//printf("tb_copy->first->text = %s\n", tb_copy->first->text);
	//printf("tb->first->next->text = %s\n", tb->first->next->text);
	assert(strcmp(tb_copy->first->text,tb->first->next->text)==0);
	assert(tb_copy->first->next==NULL);
	

//(J): Several items (copy middle)

	//Use function
	large = textbuffer_new("first\nsecond\nthird\nfourth\nfifth\nsixth\n\0");
	tb_copy = textbuffer_copy(large,1,3);

	//tests
	assert(strcmp(tb_copy->first->text,"second")==0);
	assert(strcmp(tb_copy->first->next->text,"third")==0);
	assert(strcmp(tb_copy->first->next->next->text,"fourth")==0);
	assert(tb_copy->first->next->next->next==NULL);

//TEST 9: Testing textbuffer_paste
printf("Test 9: Testing textbuffer_paste\n");

	//TODO: add more tests
	//Use function
	empty = textbuffer_new("");
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	textbuffer_paste(empty, 0, tb);

	//tests
	assert(strcmp(empty->first->text,"hello")==0);
	assert(strcmp(empty->first->next->text,"there")==0);
	assert(strcmp(empty->first->next->next->text,"I am sam")==0);
	assert(strcmp(empty->first->next->next->next->text,"this")==0);
	assert(empty->first->next->next->next->next==NULL);
	assert(empty->last->next==NULL);

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

	//(B): Several items (cut start)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 0, 1);

	//tests
	assert(strcmp(tb_copy->first->text,"hello")==0);
	assert(strcmp(tb_copy->first->next->text,"there")==0);
	assert(tb_copy->first->next->next==NULL);	

	//(C): Several items (cut end)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 2, 3);

	//tests
	assert(strcmp(tb_copy->first->text,"I am sam")==0);
	assert(strcmp(tb_copy->first->next->text,"this")==0);
	assert(tb_copy->first->next->next==NULL);
	
	//(D): Several items (cut one line)
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut (tb, 2, 3);

	//tests
	assert(strcmp(tb_copy->first->text,"I am sam")==0);
	assert(strcmp(tb_copy->first->next->text,"this")==0);
	assert(tb_copy->first->next->next==NULL);
	
	//(E): From, to out of bounds
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut(tb, 9, 10);

	//test
	assert(tb_copy==NULL);
	
	//(F): From = to
	//Use function
	tb = textbuffer_new("hello\nthere\nI am sam\nthis\n\0");
	tb_copy = textbuffer_cut(tb, 1, 1);

	//test
	assert(strcmp(tb_copy->first->text,"there")==0);
	assert(tb_copy->first->next==NULL);

	//(G): Textbuffer is null
	//Use function
	tb_copy = textbuffer_cut(NULL,0,0);

	//test
	assert(tb_copy==NULL);
	
	//(H): Empty textbuffer
	//Use function
	empty = textbuffer_new("");
	tb_copy = textbuffer_cut(empty,0,0);

	//test
	assert(strcmp(empty->first->text,"")==0);
	assert(empty->first->next==NULL);
