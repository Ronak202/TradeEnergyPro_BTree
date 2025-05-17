#include"energytrading.h"
// Function to create a new node
struct BTreeNode *createNode(bool is_leaf) {
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");//memory allocation failed!!!
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < M; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}
// Function to split a full child node
void splitChild( BTreeNode *parent, int index) {
     BTreeNode *child = parent->children[index];
    BTreeNode *newNode = createNode(child->is_leaf);
    
    newNode->num_keys = M/2 - 1;
    int mid=(M-1)/2;
    
    // Move keys and children to the new node
    for (int i = 0; i <newNode->num_keys; i++) {
        newNode->keys[i] = child->keys[i +mid+1];
    }
    
    if (!child->is_leaf) {
        for (int i = 0; i < M/2; i++) {
            newNode->children[i] = child->children[i + M/2];
        }
    }
    
    child->num_keys = mid;
    
    // Shift parent's children to make space for the new node
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    
    parent->children[index + 1] = newNode;
    
    // Shift parent's keys to insert the middle key from the child
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
     //promoting middle child to parent
    parent->keys[index] = child->keys[mid];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFull(struct BTreeNode *node, data key) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Insert key into the sorted order
        while (i >= 0 &&(strcmp((node->keys[i]).transaction_id,key.transaction_id)>0))
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        //insert in node
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        // Find the child to insert the key
        while (i >= 0 && (strcmp((node->keys[i]).transaction_id,key.transaction_id)>0))
         {
            i--;
        }
        i++;
        
        if (node->children[i]->num_keys == M - 1) {
            // Split child if it's full
            splitChild(node, i);
            
            // Determine which of the two children is the new one
            if (strcmp(key.transaction_id, node->keys[i].transaction_id) > 0) {
                i++;
            }
            
        }
        insertNonFull(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insert(struct BTreeNode **root, data key) {
    struct BTreeNode *node = *root;

    if (node == NULL) {
        // Tree is empty, create root
        *root = createNode(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    } else {
        if (node->num_keys == M - 1) {
            // Root is full, need to split
             BTreeNode *new_root = createNode(false);
            new_root->children[0] = node;

            splitChild(new_root, 0);

            // Decide which child gets the new key based on transaction_id
            if (strcmp(key.transaction_id, new_root->keys[0].transaction_id) > 0) {
                insertNonFull(new_root->children[1], key);
            } else {
                insertNonFull(new_root->children[0], key);
            }
            *root = new_root;
        } else {
            insertNonFull(node, key);
        }
    }
}
