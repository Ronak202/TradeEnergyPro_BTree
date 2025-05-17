#include"energytrading.h"
// create seller node
SellerBTreeNode* createSellerNode(bool is_leaf) {
    SellerBTreeNode* node = (SellerBTreeNode*)malloc(sizeof(SellerBTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < M; i++) {
        node->children[i] = NULL;
        if (i < M - 1) node->keys[i] = NULL;
    }
    return node;
}
//split seller child;
void splitSellerChild(SellerBTreeNode* parent, int index) {
    SellerBTreeNode* child = parent->children[index];
    SellerBTreeNode* newNode = createSellerNode(child->is_leaf);
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
//insert not full seller tree
void insertNonFullSeller(SellerBTreeNode* node, seller_node* key) {
    int i = node->num_keys - 1;
    if (node->is_leaf) {
        while (i >= 0 && strcmp(key->seller_id, node->keys[i]->seller_id) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && strcmp(key->seller_id, node->keys[i]->seller_id) < 0) i--;
        i++;
        if (node->children[i]->num_keys == M - 1) {
            splitSellerChild(node, i);
            if (strcmp(key->seller_id, node->keys[i]->seller_id) > 0) i++;
        }
        insertNonFullSeller(node->children[i], key);
    }
}
//insert in seller b tree
void insertSeller(SellerBTreeNode** root, seller_node* seller) {
    if (*root == NULL) {
        *root = createSellerNode(true);
        (*root)->keys[0] = seller;
        (*root)->num_keys = 1;
    } else {
        if ((*root)->num_keys == M - 1) {
            SellerBTreeNode* new_root = createSellerNode(false);
            new_root->children[0] = *root;
            splitSellerChild(new_root, 0);
            int i = strcmp(seller->seller_id, new_root->keys[0]->seller_id) > 0 ? 1 : 0;
            insertNonFullSeller(new_root->children[i], seller);
            *root = new_root;
        } else {
            insertNonFullSeller(*root, seller);
        }
    }
}
//searching in seller b tree for havibg seller id key!!
seller_node* searchSeller(SellerBTreeNode* root, char* key) {
    if (root == NULL) return NULL;
    
    int i = 0;
    while (i < root->num_keys && strcmp(key, root->keys[i]->seller_id) > 0) i++;
    if (i < root->num_keys && strcmp(key, root->keys[i]->seller_id) == 0) return root->keys[i];
    if (root->is_leaf) return NULL;
    return searchSeller(root->children[i], key);
}
//check if seller is already present in tree or not!!
void check_insert_seller(SellerBTreeNode** root, char* key, data transaction,float price_less,float price_more) {
    seller_node* seller = searchSeller(*root, key);
    if (seller) {
        seller->total_revenue+=transaction.total_price;
        insert(&seller->Transaction_Subtree, transaction);
        return;
    }

    seller_node* newSeller = (seller_node*)malloc(sizeof(seller_node));
    strcpy(newSeller->seller_id, key);
    newSeller->price_kwh_gt_300=price_more;
    newSeller->price_kwh_lt_300=price_less;
    newSeller->Transaction_Subtree = NULL;
    newSeller->regularBuyers = NULL;
    newSeller->total_revenue=transaction.total_price;
   // newSeller->numtrans++;
    insert(&newSeller->Transaction_Subtree, transaction);
    insertSeller(root, newSeller);
}