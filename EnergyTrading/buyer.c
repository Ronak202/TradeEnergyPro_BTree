#include"energytrading.h"
// Create a set of Transactions for Every Buyer
void  display_list_of_trans_buyer(BuyerBTreeNode* root){
    if (root != NULL) {
        for (int i = 0; i < root->num_keys; i++) {
            display_list_of_trans_buyer(root->children[i]);
            printf("\n== Buyer ID: %s | Total_Energy_Purchased %f==\n",
                   root->keys[i]->buyer_id, root->keys[i]->total_energy_purchased);
            printTableHeader();
            traverse(root->keys[i]->Transaction_Subtree);
        }
        display_list_of_trans_buyer(root->children[root->num_keys]);
    }
} // Sort the set of Buyers Based on Energy Bought 
void sort_buyers(buyerlist* list, int count) {
    for (int i = 1; i < count; i++) {
        buyerlist current = list[i];
        int j = i - 1;
        while (j >= 0 && list[j].bx->total_energy_purchased > current.bx->total_energy_purchased) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = current;  // Insert in correct position
    }
}
// Collect all buyers (in-order traversal)
void collect_buyers(BuyerBTreeNode* root, buyerlist* result, int* count) {
    if (root == NULL || *count >= MAX_BUYERS) return;

    for (int i = 0; i < root->num_keys; i++) {
        collect_buyers(root->children[i], result, count);  
        if (*count < MAX_BUYERS) {
            result[*count].bx = root->keys[i];  // Store buyer
            (*count)++;
        } else {
            printf("Warning: Reached max buyers (%d)\n", MAX_BUYERS);
            return;
        }
    }
    collect_buyers(root->children[root->num_keys], result, count); 
}
// Display buyers sorted by energy_purchased
void display_sorted_buyers(BuyerBTreeNode* root) {
    buyerlist buyers[MAX_BUYERS] = {0};
    int count = 0;
    collect_buyers(root, buyers, &count);
    sort_buyers(buyers, count);
    for (int i = 0; i < count; i++) {
        print_buyer(buyers[i].bx);
    }
}
void print_buyer(buyer_node* buyer) {
    printf("\nBuyer: %s (Total: %.2f kWh)\n",buyer->buyer_id, buyer->total_energy_purchased);
    printTableHeader();
    traverse(buyer->Transaction_Subtree);
}