#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node {
    User* user;
    struct Node* left;
    struct Node* right;
    int height;
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
int height(Node* node){
    if(node == NULL){
        return 0;
    }
    return node->height;
}

int balanceFactor(Node* node){
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

int maximum(int a, int b){
    return (a > b)? a : b;
}

void updateHeight(Node* node){
    if (node != NULL){
        node->height = 1 + maximum(height(node->left), height(node->right));
    }
}

Node* rotateRight(Node* node){
    Node* pivot = node->left;

    node->left = pivot->right;
    pivot->right = node;

    updateHeight(node);
    updateHeight(pivot);

    return pivot;
}

Node* rotateLeft(Node* node) {
    Node* pivot = node->right;

    node->right = pivot->left;
    pivot->left = node;

    updateHeight(node);
    updateHeight(pivot);

    return pivot;
}

Node* rebalance(Node* node) {
    updateHeight(node);

    if (balanceFactor(node) < - 1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    } else if (balanceFactor(node) > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    return node;
}

User* db_queryRecur(Node* node, const char* email){
    if (node == NULL)
        return NULL;
    int cmp = strcmp(email, node->user->email);
    if(cmp == 0)
        return node->user;
    else if(cmp < 0)
        return db_queryRecur(node->left, email);
    else
        return db_queryRecur(node->right, email);
}

User* db_query(Database* db, const char* email){
    return db_queryRecur(db->root, email);
}

Node* newNode(User* user){
    Node* node = malloc(sizeof(Node));
    node->user = user;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node* db_insertRecur(Node* node, User* student){
    if(node == NULL){
        return newNode(student);
    }
    int cmp = strcmp(student->email, node->user->email);
    if (cmp<0){
        node->left = db_insertRecur(node->left, student);
    } else if (cmp>0){
        node->right = db_insertRecur(node->right, student);
    } 
    return rebalance(node);
}

void db_insert(Database* db, User* student){
    db->root = db_insertRecur(db->root, student);
}

void printInOrder(Node* node){
    if(node == NULL)
        return;
    
    printInOrder(node->left);
    user_print(node->user);
    printInOrder(node->right);
}

void db_printInOrder(Database* db){
    if(db->root == NULL){
        printf("Empty database");
        return;
    }
    printf("Id, Email, Name, Password\n");
    printInOrder(db->root);
}

void printPreOrder(Node* node){
    if(node == NULL)
        return;
    
    user_print(node->user);
    printPreOrder(node->left);
    printPreOrder(node->right);
}

void db_printPreOrder(Database* db){
    if(db->root == NULL){
        printf("Empty database");
        return;
    }
    printf("Id, Email, Name, Password\n");
    printPreOrder(db->root);
}

void printPostOrder(Node* node){
    if(node == NULL)
        return;
        
    printPostOrder(node->left);
    printPostOrder(node->right);
    user_print(node->user);
}

void db_printPostOrder(Database* db){
    if(db->root == NULL){
        printf("Empty database");
        return;
    }
    printf("Id, Email, Name, Password\n");
    printPostOrder(db->root);
}

Node* sucessor(Node* node){
    if(node==NULL)
        return NULL;
    Node* cur = node->right;
    while(cur->left != NULL)
        cur = cur->left;
    return cur;
}

Node* db_removeRecur(Node* node,const char* email){
    if(node == NULL)
        return node;
    int cmp = strcmp(email, node->user->email);
    if(cmp < 0)
        node->left = db_removeRecur(node->left, email);
    else if(cmp > 0)
        node->right = db_removeRecur(node->right, email);
    else{
        if(node->left == NULL){
            Node* trash = node;
            node = node->right;
            user_destroy(trash->user);
            free(trash);
        }else if(node->right == NULL){
            Node* trash = node;
            node = node->left;
            user_destroy(trash->user);
            free(trash);
        } else{
            Node* nsucessor = sucessor(node);
            node->user->id = nsucessor->user->id;
            strcpy(node->user->email, nsucessor->user->email);
            strcpy(node->user->name, nsucessor->user->name);
            strcpy(node->user->password, nsucessor->user->password);
            
            node->right = db_removeRecur(node->right, nsucessor->user->email);
        }
    }
    return rebalance(node);
}

void db_remove(Database* db, const char* email){
    db->root = db_removeRecur(db->root, email);
}
