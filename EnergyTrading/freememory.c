#include"energytrading.h"
// Free a transaction B-tree
void freeTransactionTree(BTreeNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i <= root->num_keys; i++) {
        freeTransactionTree(root->children[i]);
    }
    free(root);
}

// Free regular buyers linked list
void freeRegularBuyers(regularBuyer* rb) {
    while (rb != NULL) {
        regularBuyer* next = rb->next;
        free(rb);
        rb = next;
    }
}

// Free a seller B-tree
void freeSellerTree(SellerBTreeNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i <= root->num_keys; i++) {
        freeSellerTree(root->children[i]);
    }
    
    for (int i = 0; i < root->num_keys; i++) {
        if (root->keys[i] != NULL) {
            freeTransactionTree(root->keys[i]->Transaction_Subtree);
            freeRegularBuyers(root->keys[i]->regularBuyers);
            free(root->keys[i]);
        }
    }
    free(root);
}

// Free a buyer B-tree
void freeBuyerTree(BuyerBTreeNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i <= root->num_keys; i++) {
        freeBuyerTree(root->children[i]);
    }
    
    for (int i = 0; i < root->num_keys; i++) {
        if (root->keys[i] != NULL) {
            freeTransactionTree(root->keys[i]->Transaction_Subtree);
            free(root->keys[i]);
        }
    }
    free(root);
}

// Free a pair B-tree
void freePairTree(PairBTreeNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i <= root->num_keys; i++) {
        freePairTree(root->children[i]);
    }
    
    for (int i = 0; i < root->num_keys; i++) {
        if (root->keys[i] != NULL) {
            freeTransactionTree(root->keys[i]->Transaction_Subtree);
            free(root->keys[i]);
        }
    }
    free(root);
}