#include"energytrading.h"
//list of pair
void display_list_of_trans_pair(PairBTreeNode* root) {
    if (root != NULL) {
        for (int i = 0; i < root->num_keys; i++) {
            display_list_of_trans_pair(root->children[i]);
            
            // Print pair header information
            printf("\n Seller: %s | Buyer: %s | Transactions: %d\n",
                   root->keys[i]->seller_id,
                   root->keys[i]->buyer_id,
                   root->keys[i]->number_transactions);
            
            // Print transaction table
            printf("Transaction History:\n");
            printTableHeader();
            if (root->keys[i]->Transaction_Subtree) {
                traverse(root->keys[i]->Transaction_Subtree);
            } else {
                printf("| %-94s |\n", "No transactions found");
            }
            printf("---------------------------------------------------------------------------------------------------------------\n");
        }
        display_list_of_trans_pair(root->children[root->num_keys]);
    }
}
// Sort the set of Seller-Buyers Based on number of transaction 
void sort_pairs(pairlist* list, int count) {
    for (int i = 1; i < count; i++) {
        pairlist current = list[i];
        int j = i - 1;
        while (j >= 0 && list[j].px->number_transactions> current.px->number_transactions) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = current;  // Insert in correct position
    }
}
// Collect all pair of seller and buyer (in-order traversal)
void collect_pairs(PairBTreeNode* root, pairlist* result, int* count) {
    if (root == NULL || *count >= MAX_PAIR) return;
    for (int i = 0; i < root->num_keys; i++) {
        collect_pairs(root->children[i], result, count);  
        if (*count < MAX_BUYERS) {
            result[*count].px= root->keys[i];  // Store pair
            (*count)++;
        } else {
            printf("Warning: Reached max pairs (%d)\n", MAX_PAIR);
            return;
        }
    }
    collect_pairs(root->children[root->num_keys], result, count); 
}
// Display pair sorted by number of transaction
void display_sorted_pair(PairBTreeNode* root) {
    pairlist pairs[MAX_PAIR] = {0};
    int count = 0;
    collect_pairs(root, pairs, &count);
    sort_pairs(pairs, count);
    for (int i = 0; i < count; i++) {
        print_pair(pairs[i].px);
    }
}
void print_pair(pair_node* pair) {
    printf("\n: SellerID:%s |BuyerID: %s|Total Transaction: %d\n",pair->seller_id,pair->buyer_id, pair->number_transactions);
    printf("Transactions:");
    printTableHeader();
    traverse(pair->Transaction_Subtree);
}