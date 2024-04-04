#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node {
    User* user;
    struct Node* left;
    struct Node* right;
} Node;

struct Database {
    Node* root;
};

Database* db_create() {
    Database* bst = malloc(sizeof(Database));

    bst->root = NULL;

    return bst;
}

void bst_destroyRecur(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        bst_destroyRecur(node->left);
    }
    if (node->right != NULL) {
        bst_destroyRecur(node->right);
    }
    free(node);
}

void db_destroy(Database* bst) {
    bst_destroyRecur(bst->root);
    free(bst);
}

int db_sizeRecur(Node* node) {
    if (node == NULL) {
        return 0;
    }

    return 1 + db_sizeRecur(node->left) + db_sizeRecur(node->right);
}

int db_size(Database* db) {
    return db_sizeRecur(db->root);
}

// new functions