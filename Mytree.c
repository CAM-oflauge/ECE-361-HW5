/**
 * file @Mytree.c
 *
 * version1.3
 *
 * author@ camden Mills (millscam@pdx.edu)
 *
 * brief:
 * source code for the Mytree api, providing functions to operate
 * a basic binary search tree.
 * includes funtions:
 *  - create tree
 *  - insert node
 *  - search tree
 *  - in order traversal
 *  - destroy tree
 *-	 populate tree with random data
 *   - convert user data to timestamp format
*/



#include <stdio.h>
#include <stdlib.h>
#include "Mytree.h"
#include "iom361_r2.h"

// Helper function prototypes
static bst_node_t *create_node(temp_humid_data_t data);
static void destroy_tree(bst_node_t *node);
static void inorder_traversal(bst_node_t *node);

// Function implementations

bst_t *bst_create() {
    bst_t *tree = (bst_t *)malloc(sizeof(bst_t));
    if (tree) {
        tree->root = NULL;
    }
    return tree;
}

void populateBST(bst_t *tree, uint32_t *base, int month, int year, int days_in_month){
	temp_humid_data_t data_array[days_in_month];
	int indices[days_in_month];

	struct tm date = {0};
	date.tm_year = year - 1900; //date from epoch
	date.tm_mon = month - 1;

	// fill array with data
	for (int i = 0; i < days_in_month; i++) {
		date.tm_mday = i + 1;
	    time_t timestamp = mktime(&date);

	    // create random temp and humidity
	    _iom361_setSensor1_rndm(0.0, 100.0, 0.0, 99.0);
	    int rtn_code;
	    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rtn_code);
	    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rtn_code);
	    float temperature = (raw_temp / (float)(1 << 20)) * 200 - 50;
	    float humidity = (raw_humid / (float)(1 << 20)) * 100;

	    // Populate the array
	    data_array[i].timestamp = timestamp;
	    data_array[i].temp = (uint32_t)temperature;
	    data_array[i].humid = (uint32_t)humidity;

	    indices[i] = i; // Initialize the index array
	}

	// Shuffle the dates via indices
	srand(time(NULL));
	for (int i = days_in_month - 1; i > 0; i--) {
	    int j = rand() % (i + 1);
	    int temp = indices[i];
	    indices[i] = indices[j];
	    indices[j] = temp;
	}

	// insert data into tree
	for (int i = 0; i < days_in_month; i++) {
	    int index = indices[i];
	    bst_insert(tree, data_array[index]);
	}
}


int bst_insert(bst_t *tree, temp_humid_data_t data) {
    if (!tree) return -1;

    bst_node_t **current = &(tree->root);
    while (*current) {
        if (data.timestamp < (*current)->data.timestamp) {
            current = &((*current)->left);
        } else if (data.timestamp > (*current)->data.timestamp) {
            current = &((*current)->right);
        } else {
            // if timestamp already exists
            return -1;
        }
    }

    *current = create_node(data);
    return *current ? 0 : -1;
}

temp_humid_data_t *bst_search(bst_t *tree, time_t timestamp) {
    if (!tree) return NULL;

    bst_node_t *current = tree->root;
    while (current) {
        if (timestamp < current->data.timestamp) {
            current = current->left;
        } else if (timestamp > current->data.timestamp) {
            current = current->right;
        } else {
            return &current->data;
        }
    }
    return NULL;
}

void bst_inorder_traversal(bst_t *tree) {
    if (!tree) return;
    inorder_traversal(tree->root);
}

void bst_destroy(bst_t *tree) {
    if (!tree) return;
    destroy_tree(tree->root);
    free(tree);
}

// helper functions

static bst_node_t *create_node(temp_humid_data_t data) {
    bst_node_t *node = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (node) {
        node->data = data;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

static void destroy_tree(bst_node_t *node) {
    if (!node) return;
    destroy_tree(node->left);
    destroy_tree(node->right);
    free(node);
}

static void inorder_traversal(bst_node_t *node) {
    if (!node) return;
    inorder_traversal(node->left);
    char date_str[20];
    struct tm *tm_info = localtime(&(node->data.timestamp));
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);
    printf("Date: %s, Temp: %uC, Humid: %u%%\n", date_str, node->data.temp, node->data.humid);
    inorder_traversal(node->right);
}
// convert time function implemetation (not a helper... or maybe it is idk)
time_t convertTimestamp(int year, int month, int day) {
    struct tm date = {0};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    return mktime(&date);
}
