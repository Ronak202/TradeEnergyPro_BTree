#include"energytrading.h"
// Function to read seller data from file
void readSellerData(SellerBTreeNode** seller_root) {
    FILE *file = fopen("sellertable.txt", "r");
    if (file == NULL) {
        perror("Error opening seller file");
        return;
    } 
    char line[256];
    // Skip header line
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) ){
        char sellerID[ID_SIZE];
        float price_lt_300, price_gt_300;
        
        if (sscanf(line, "%[^,],%f,%f", sellerID, &price_lt_300, &price_gt_300) == 3) {//untill comma is encountered
            seller_node* newSeller = (seller_node*)malloc(sizeof(seller_node));
            strcpy(newSeller->seller_id, sellerID);
            newSeller->price_kwh_lt_300 = price_lt_300;
            newSeller->price_kwh_gt_300 = price_gt_300;
            newSeller->total_revenue = 0;
            newSeller->Transaction_Subtree = NULL;
            newSeller->regularBuyers = NULL;
            insertSeller(seller_root, newSeller);
        }
    }
    fclose(file);
}

// Function to read transaction data from file
void readTransactionData(SellerBTreeNode** seller_root, BuyerBTreeNode** buyer_root, 
                        PairBTreeNode** pair_root, BTreeNode** trans_root) {
    FILE *file = fopen("energytrading.txt", "r");
    if (file == NULL) {
        perror("Error opening transaction file");
        return;
    }

    char line[256];
    // Skip header line
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        char transactionID[ID_SIZE], sellerID[ID_SIZE], buyerID[ID_SIZE], timestamp[DATE_SIZE];
        int amount_energy;
        
        if (sscanf(line, "%[^,],%[^,],%[^,],%d,%[^,\n]", 
                   transactionID, sellerID, buyerID, &amount_energy, timestamp) == 5) {
            transaction_node* trans = createTransactionNode(seller_root, buyer_root, pair_root, 
                transactionID, sellerID, buyerID, amount_energy, timestamp);
            if (trans) {
                insert(trans_root, *trans);
            }
        }
    }
    fclose(file);
}

// Function to write tranaction  data to file
void writeTransactionDataHelper(FILE* file, BTreeNode* root) {
    if (root != NULL) {
        for (int i = 0; i < root->num_keys; i++) {
            writeTransactionDataHelper(file, root->children[i]);
            fprintf(file, "%s,%s,%s,%d,%s\n", //write a lie to file pointed by file ptr;;
                    root->keys[i].transaction_id,
                    root->keys[i].seller_id,
                    root->keys[i].buyer_id,
                    root->keys[i].amount_energy,
                    root->keys[i].timestamp);
        }
        writeTransactionDataHelper(file, root->children[root->num_keys]);
    }
}
void writeTransactionData(BTreeNode* root) {
    FILE *file = fopen("energytrading.txt", "w");
    if (file == NULL) {
        perror("Error opening transaction file for writing");
        return;
    }

    fprintf(file, "TransactionID,SellerID,BuyerID,AmountEnergy,Timestamp\n");
    writeTransactionDataHelper(file, root);
    fclose(file);
}
//write seller dtata in file
void writeSellerDataHelper(FILE* file, SellerBTreeNode* root) {
    if (root != NULL) {
        for (int i = 0; i < root->num_keys; i++) {
            writeSellerDataHelper(file, root->children[i]);
            fprintf(file, "%s,%.2f,%.2f\n", 
                    root->keys[i]->seller_id, 
                    root->keys[i]->price_kwh_lt_300, 
                    root->keys[i]->price_kwh_gt_300);
        }
        writeSellerDataHelper(file, root->children[root->num_keys]);
    }
}

void writeSellerData(SellerBTreeNode* root) {
    FILE *file = fopen("sellertable.txt", "w");
    if (file == NULL) {
        perror("Error opening seller file for writing");
        return;
    }

    fprintf(file, "SellerID,Price_KWh_LT_300,Price_KWh_GT_300\n");
    writeSellerDataHelper(file, root);
    fclose(file);
}