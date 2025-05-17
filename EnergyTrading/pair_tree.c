#include"energytrading.h"
// Create a new pair B-tree node
PairBTreeNode *createPairNode(bool is_leaf) {
    PairBTreeNode* newPnode = (PairBTreeNode*)malloc(sizeof(PairBTreeNode));
    if(newPnode == NULL) {
        printf("\nMemory allocation failed!!\n");
        return NULL;
    }
    newPnode->is_leaf = is_leaf;
    newPnode->num_keys = 0;
    for (int i = 0; i < M; i++) {
        newPnode->children[i] = NULL;
        if (i < M - 1) newPnode->keys[i] = NULL;
    }
    return newPnode;
}

// Split a full child node in pair B-tree
void splitPairChild(PairBTreeNode* parent, int index) {
    PairBTreeNode* child = parent->children[index];
    PairBTreeNode* newNode = createPairNode(child->is_leaf);
    int mid = (M - 1) / 2;
    newNode->num_keys = mid;

    // Copy keys to new node
    for (int i = 0; i < mid; i++) {
        newNode->keys[i] = child->keys[i + mid + 1];
    }

    // Copy children if not leaf
    if (!child->is_leaf) {
        for (int i = 0; i <= mid; i++) {
            newNode->children[i] = child->children[i + mid + 1];
        }
    }

    child->num_keys = mid;

    // Make space in parent for new child
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    // Link new child to parent
    parent->children[index + 1] = newNode;
    parent->keys[index] = child->keys[mid];
    parent->num_keys++;
}

// Insert key into non-full pair node
void insertNonFullPair(PairBTreeNode *node, pair_node* key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        // Find position to insert and shift keys
        while (i >= 0 && strcmp(key->pair_id, node->keys[i]->pair_id) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        // Insert key
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        // Find child to insert into
        while (i >= 0 && strcmp(key->pair_id, node->keys[i]->pair_id) < 0) {
            i--;
        }
        i++;

        // Split child if full
        if (node->children[i]->num_keys == M - 1) {
            splitPairChild(node, i);
            // Determine which child to insert into
            if (strcmp(key->pair_id, node->keys[i]->pair_id) > 0) {
                i++;
            }
        }
        // Recursive insert
        insertNonFullPair(node->children[i], key);
    }
}

// Insert pair into B-tree
void insertPair(PairBTreeNode** root, pair_node* pair) {
    if (*root == NULL) {
        *root = createPairNode(true);
        (*root)->keys[0] = pair;
        (*root)->num_keys = 1;
    } else {
        if ((*root)->num_keys == M - 1) {
            // Root is full, need to split
            PairBTreeNode* new_root = createPairNode(false);
            new_root->children[0] = *root;
            splitPairChild(new_root, 0);

            // Determine which child gets the new pair
            int i = strcmp(pair->pair_id, new_root->keys[0]->pair_id) > 0 ? 1 : 0;
            insertNonFullPair(new_root->children[i], pair);
            *root = new_root;
        } else {
            insertNonFullPair(*root, pair);
        }
    }
}

// Search for a pair in the B-tree
pair_node* searchPair(PairBTreeNode* root, char* key) {
    if (root == NULL) return NULL;
    
    int i = 0;
    while (i < root->num_keys && strcmp(key, root->keys[i]->pair_id) > 0) {
        i++;
    }
    
    if (i < root->num_keys && strcmp(key, root->keys[i]->pair_id) == 0) {
        return root->keys[i];
    }
    
    if (root->is_leaf) {
        return NULL;
    }
    
    return searchPair(root->children[i], key);
}
void check_insert_pair(PairBTreeNode** root, SellerBTreeNode **seller_root, char* key, data transaction) {
    pair_node* pair = searchPair(*root, key);
    if (pair) {
        // Pair exists, just add transaction
        insert(&pair->Transaction_Subtree, transaction);
        pair->number_transactions++;
        if (pair->number_transactions > 5) {
            // Get the seller node from seller tree
            seller_node* seller = searchSeller(*seller_root, pair->seller_id);
            if (seller) {
                // Check if buyer already exists in regular buyers list
                regularBuyer* current = seller->regularBuyers;
                bool exists = false;
                bool buyerFound = false;  
                
                while (current != NULL && !buyerFound) {
                    if (strcmp(current->buyer_id, pair->buyer_id) == 0) {
                        exists = true;
                        buyerFound = true; 
                    }
                    current = current->next;
                }
                
                // Add to regular buyers if not already present
                if (!exists) {
                    regularBuyer* newBuyer = (regularBuyer*)malloc(sizeof(regularBuyer));
                    if (newBuyer) {
                        strcpy(newBuyer->buyer_id, pair->buyer_id);
                        newBuyer->next = seller->regularBuyers;
                        seller->regularBuyers = newBuyer;
                        printf("Added %s to %s's regular buyers\n", 
                               pair->buyer_id, pair->seller_id);
                    }
                }
            }
        }
        return;
    }
    
    // Create new pair node
    pair_node* newpair = (pair_node*)malloc(sizeof(pair_node));
    if (newpair == NULL) {
        printf("Memory allocation failed for new pair\n");
        return;
    }
    
    strcpy(newpair->pair_id, key);
    strcpy(newpair->seller_id, transaction.seller_id);
    strcpy(newpair->buyer_id, transaction.buyer_id);
    newpair->number_transactions = 1;
    newpair->Transaction_Subtree = NULL;
    newpair->next = NULL;
    
    // Insert transaction
    insert(&newpair->Transaction_Subtree, transaction);
    
    // Insert into B-tree
    insertPair(root, newpair);
}