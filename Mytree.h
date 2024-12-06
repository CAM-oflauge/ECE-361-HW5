/**
 * file @Mytree.h
 *
 * version1.3
 *
 * author@ camden Mills (millscam@pdx.edu)
 *
 * brief:
 * header file for the Mytree source code. includes funtions:
 *  - create tree
 *  - insert node
 *  - search tree
 *  - in order traversal
 *  - destroy tree
 *  - populate tree with random data
 *   - convert user data to timestamp format
*/




#ifndef BST_H
#define BST_H

#include <time.h>
#include <stdint.h>

// how data is stored in the nodes
typedef struct temp_humid_data {
    time_t timestamp;      /**< Timestamp of the data */
    uint32_t temp;         /**< Temperature value */
    uint32_t humid;        /**< Humidity value */
} temp_humid_data_t;

// how a node is made for the tree
typedef struct bst_node {
    temp_humid_data_t data;  /**< Data stored in the node */
    struct bst_node *left;   /**< Pointer to the left child */
    struct bst_node *right;  /**< Pointer to the right child */
} bst_node_t;

// start of the tree
typedef struct bst {
    bst_node_t *root; /**< Pointer to the root node of the tree */
} bst_t;

/**
 * @brief makes a new tree for tree perposes.
 * @return a pointer to the new binary search tree.
 */
bst_t *bst_create();

/**
 * @brief Inserts a new node into the binary search tree.
 * @param tree Pointer for the bst
 * @param data in format of temp_humid_data
 * @return 0 on success, -1 on failure (e.g., duplicate timestamp).
 */
int bst_insert(bst_t *tree, temp_humid_data_t data);

/**
 * @brief Searches for a node with a specific timestamp in the tree.
 * @param tree Pointer for the bst
 * @param timestamp for date
 * @return a pointer to the data if found, or NULL if not found.
 */
temp_humid_data_t *bst_search(bst_t *tree, time_t timestamp);

/**
 * @brief Performs an inorder traversal of the tree and prints the data.
 * @param tree is a pointer for the bst needed
 */
void bst_inorder_traversal(bst_t *tree);

/**
 * @brief Deletes the entire binary search tree
 */
void bst_destroy(bst_t *tree);

void populateBST(bst_t *tree, uint32_t *base, int month, int year, int days_in_month);

time_t convertTimestamp(int year, int month, int day);
#endif // BST_H


