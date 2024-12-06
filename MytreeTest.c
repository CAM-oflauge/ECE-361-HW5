/**
 * file @MyTreeTest.c
 *
 * version1.3
 *
 * author@ camden Mills (millscam@pdx.edu)
 *
 * brief:
 * tests all functions for the Mytree source code. this test will create a
 * tree, fill the tree with random values and then start a while loop that will allow the user
 * to send in request for data on a spacific date. once while loop has ended, test will display
 * all data in the tree in order then destroy itself.
 *
 *
 * history:
 * version 1.0: tested data input manually using the search function
 * version 1.1: implemented the populate bst function instead of manual input of data
 * version 1.2: implemented the user search functions but didnt loop them (1 time search)
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include "Mytree.h"
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "iom361_r2.h"

int main(){
	printf("ECE 361 - Binary search tree test\n");

	// get current directory
		    errno = 0;
		    char *buf = getcwd(NULL, 0);    // allocates a buffer large enough to hold the path

		    if (buf == NULL) {
		        perror("getcwd");
		        printf("Could not display the path\n");
		    }
		    else {
		        printf("Current working directory: %s\n", buf);
		        free(buf);
		    }
		    printf("\n");

	// initialize iom361
	int rtn_code = 0;
	uint32_t* base = iom361_initialize(0, 0, &rtn_code);
	    if (base == NULL || rtn_code != 0) {
	        printf("Failed to initialize I/O module. Error code: %d\n", rtn_code);
	        return 1;
	    }

	//create tree
	bst_t *tree = bst_create();
	if (!tree){
		printf("woopsies");
		return 1;
	}

	// fill up tree with data
	populateBST(tree,base, 12, 2024, 31);


	int year, month, day;
	printf("Enter a date in december of 2024, type '0 0 0' to quit(YYYY MM DD): ");
	// run data search loop
	while (year != 0){
		scanf("%d %d %d", &year, &month, &day);
		//convert ints to 1 timestamp
		time_t requestedTime = convertTimestamp(year, month, day);
		// use timestamp for search
		temp_humid_data_t* results = bst_search(tree, requestedTime);
		if (results){
			printf("Data found for date %04d-%02d-%02d: Temp = %uC, Humid = %u%%\n\n",year, month, day, results->temp, results->humid);
			printf("Enter a date in december of 2024, type '0 0 0' to quit(YYYY MM DD): ");
			}
		else{
			printf("Data not found for that date, try another:");
			}
	}


    	// print ordered list of contents
        printf("Tree contents (inorder traversal):\n");
        bst_inorder_traversal(tree);



        // Destruction of the weather tree
        bst_destroy(tree);
        printf("\nBinary search tree destroyed. Exiting program.\n");



        return 0;
}
