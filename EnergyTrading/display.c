#include"energytrading.h"
// table header
void printTableHeader() {
    printf("----------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-10s | %-12s | %-12s | %-20s |\n", 
           "Trans ID", "Seller ID", "Buyer ID", "Energy(kWh)", "Price/kWh($)", "Total($)", "Timestamp");
    printf("----------------------------------------------------------------------------------------------------------------\n");
}
//print singlr row
void printTableRow(transaction_node* temp) {
    printf("| %-10s | %-10s | %-10s | %-12d | %-12.2f | %-12.2f | %-20s |\n", temp->transaction_id, temp->seller_id, temp->buyer_id, 
           temp->amount_energy, temp->price_kwh,temp->total_price, temp->timestamp);
}

// Function to traverse and print the B-tree in-order
void traverse(struct BTreeNode *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->num_keys; i++) {
            traverse(root->children[i]);
            // printf("%d ", root->keys[i]);
            printTableRow(&(root->keys[i]));    
        }
        traverse(root->children[i]);
    }
}
