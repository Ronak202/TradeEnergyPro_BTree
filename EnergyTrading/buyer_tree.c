#include"energytrading.h"
//create buyer tree node
BuyerBTreeNode* createBuyerNode(bool is_leaf) {
    BuyerBTreeNode* node = (BuyerBTreeNode*)malloc(sizeof(BuyerBTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < M; i++) {
        node->children[i] = NULL;
        if (i < M - 1) node->keys[i] = NULL;
    }
    return node;
}
//split buyer node
void splitBuyerChild(BuyerBTreeNode* parent, int index) {
    BuyerBTreeNode* child = parent->children[index];
    BuyerBTreeNode* newNode = createBuyerNode(child->is_leaf);
    int mid = (M - 1) / 2;
    newNode->num_keys = mid;

    for (int i = 0; i < mid; i++) newNode->keys[i] = child->keys[i + mid + 1];
    if (!child->is_leaf) {
        for (int i = 0; i <= mid; i++) newNode->children[i] = child->children[i + mid + 1];
    }
    child->num_keys = mid;

    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    parent->children[index + 1] = newNode;
    parent->keys[index] = child->keys[mid];
    parent->num_keys++;
}
//insert 
void insertNonFullBuyer(BuyerBTreeNode* node, buyer_node* key) {
    int i = node->num_keys - 1;
    if (node->is_leaf) {
        while (i >= 0 && strcmp(key->buyer_id, node->keys[i]->buyer_id) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && strcmp(key->buyer_id, node->keys[i]->buyer_id) < 0) i--;
        i++;
        if (node->children[i]->num_keys == M - 1) {
            splitBuyerChild(node, i);
            if (strcmp(key->buyer_id, node->keys[i]->buyer_id) > 0) i++;
        }
        insertNonFullBuyer(node->children[i], key);
    }
}
//insert in btree node;
void insertBuyer(BuyerBTreeNode** root, buyer_node* Buyer) {
    if (*root == NULL) {
        *root = createBuyerNode(true);
        (*root)->keys[0] = Buyer;
        (*root)->num_keys = 1;
    } else {
        if ((*root)->num_keys == M - 1) {
            BuyerBTreeNode* new_root = createBuyerNode(false);
            new_root->children[0] = *root;
            splitBuyerChild(new_root, 0);
            int i = strcmp(Buyer->buyer_id, new_root->keys[0]->buyer_id) > 0 ? 1 : 0;
            insertNonFullBuyer(new_root->children[i], Buyer);
            *root = new_root;
        } else {
            insertNonFullBuyer(*root, Buyer);
        }
    }
}
//search buyer 
buyer_node* searchBuyer(BuyerBTreeNode* root, char* key) {
    if (root == NULL) return NULL;
    
    int i = 0;
    while (i < root->num_keys && strcmp(key, root->keys[i]->buyer_id) > 0) i++;
    if (i < root->num_keys && strcmp(key, root->keys[i]->buyer_id) == 0) return root->keys[i];
    if (root->is_leaf) return NULL;
    return searchBuyer(root->children[i], key);
}
//check and insert into buyer!!!
void check_insert_Buyer(BuyerBTreeNode** root, char* key, data transaction) {
    buyer_node* buyer = searchBuyer(*root, key);
    if (buyer) {
        insert(&buyer->Transaction_Subtree, transaction);
        buyer->total_energy_purchased+=transaction.amount_energy;
        return;
    }

    buyer_node* newbuyer = (buyer_node*)malloc(sizeof(buyer_node));
    strcpy(newbuyer->buyer_id, key);
    newbuyer->Transaction_Subtree = NULL;
    newbuyer->total_energy_purchased=transaction.amount_energy;
    insert(&newbuyer->Transaction_Subtree, transaction);
    insertBuyer(root, newbuyer);
}
