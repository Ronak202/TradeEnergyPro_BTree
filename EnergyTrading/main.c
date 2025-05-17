#include"energytrading.h"
// Main function
int main() {
    BTreeNode *root = NULL;
    SellerBTreeNode *seller_root = NULL;
    BuyerBTreeNode *buyer_root = NULL;
    PairBTreeNode *pair_root = NULL;
    
    // Read data from files
    readSellerData(&seller_root);
    readTransactionData(&seller_root, &buyer_root, &pair_root, &root);
    
    int choice;
    char transactionID[ID_SIZE], sellerID[ID_SIZE], buyerID[ID_SIZE], timestamp[DATE_SIZE];
    int amt_energy;
    char start_time[DATE_SIZE], end_time[DATE_SIZE];
    double min_amount, max_amount;
    
    while (1) {
        printf("\nEnergy Trading System Menu:\n");
        printf("1. Add New Transaction\n");
        printf("2. Display All Transactions\n");
        printf("3. Display Transactions by Seller\n");
        printf("4. Display Regular Buyers\n");
        printf("5. Display Transactions by Buyer\n");
        printf("6. Find Transactions in Time Period\n");
        printf("7. Calculate Total Revenue by Seller\n");
        printf("8. Find Transactions by Energy Amount Range in increasing order\n");
        printf("9. Sort Buyers by Energy Purchased\n");
        printf("10. Display Seller-Buyer Pairs sorted by number of transaction!!!\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // Add New Transaction
               // Input Transaction ID with format and uniqueness check
                do {
                         printf("Enter Transaction ID (must start with 'TXXXX', 0<X<9 and be unique): ");
                         scanf("%5s", transactionID);
                    }    while (!validT(transactionID) || search(root, transactionID));

                    // Input Seller ID
                do {
                          printf("Enter Seller ID (must start with 'SXXXX'): ");
                          scanf("%5s", sellerID);
                    }     while (!validS(sellerID));

                        // Input Buyer ID
                do {
                        printf("Enter Buyer ID (must start with 'BXXXX'): ");
                        scanf("%5s", buyerID);
                   }    while (!validB(buyerID));

                // Input and validate Energy Amount
                int valid_input = 0;
                char ch;
                printf("Enter Energy Amount: ");
                while (!valid_input) 
                {
                 if (scanf("%d", &amt_energy) == 1 && amt_energy > 0) {
                 valid_input = 1;
                } else {
                printf("Energy amount must be a positive integer! Try again: ");
                amt_energy = 0;
                while ((ch = getchar()) != '\n' && ch != EOF); // read one char at time untill see newline character clear buffer
                }
                }
                //current timestamp                
                getCurrentTimestamp(timestamp);
                
                transaction_node* new_trans = createTransactionNode(&seller_root, &buyer_root, &pair_root, 
                    transactionID, sellerID, buyerID, amt_energy, timestamp);
                if (new_trans) {
                    insert(&root, *new_trans);
                    printf("Transaction added successfully!\n");
                } else {
                    printf("Failed to add transaction.\n");
                }
                printf("-----------------------------------------------------------------------------\n");
                break;
                
            case 2: // Display All Transactions
                printf("\nAll Transactions:\n");
                printTableHeader();
                traverse(root);
                printf("------------------------------------------------------------------------\n");
                break;
            
            case 3: // Display Transactions by Seller
                printf("\nTransactions by Seller:\n");
                display_list_of_trans_seller(seller_root);
                printf("--------------------------------------------------------------------------\n");
                break;

            case 4: // Display Regular Buyers
                printf("\nRegular Buyers:\n");
                displayAllSellersWithBuyers(seller_root);
                printf("--------------------------------------------------------------------------\n");
                break;
                
            case 5: // Display Transactions by Buyer
                printf("\nTransactions by Buyer:\n");
                display_list_of_trans_buyer(buyer_root);
                printf("---------------------------------------------------------------------\n");
                break;
                
            case 6: // Find Transactions in Time Period
                printf("Enter Start Timestamp (YYYY-MM-DD HH:MM:SS): ");
                scanf(" %[^\n]", start_time);
                if (!is_valid_timestamp(start_time)) {
                    printf("Invalid timestamp format!\n");
                    break;
                }
                
                printf("Enter End Timestamp (YYYY-MM-DD HH:MM:SS): ");
                scanf(" %[^\n]", end_time);
                if (!is_valid_timestamp(end_time)) {
                    printf("Invalid timestamp format!\n");
                    break;
                }
                
                printf("\nTransactions between %s and %s:\n", start_time, end_time);
                printTableHeader();
                transaction_given_timeperiod(root, start_time, end_time);
                printf("-----------------------------------------------------------------------\n");
                break;
                
            case 7: // Calculate Total Revenue by Seller
                printf("Enter Seller ID: ");
                scanf("%s", sellerID);
                if (!validS(sellerID)) {
                    printf("Invalid Seller ID format!\n");
                    break;
                }
                total_revenue_particular_seller(&seller_root, sellerID);
                printf("----------------------------------------------------------------------\n");
                break;
                
            case 8: // Find Transactions by Energy Amount Range
                printf("Enter Minimum Energy Amount: ");
                scanf("%lf", &min_amount);
                printf("Enter Maximum Energy Amount: ");
                scanf("%lf", &max_amount);
                
                if (min_amount < 0 || max_amount < 0 || min_amount > max_amount) {
                    printf("Invalid energy amount range!\n");
                    break;
                }
                
                printf("\nTransactions with energy between %.2f and %.2f:\n", min_amount, max_amount);
                display_transaction_given_amount(root, min_amount, max_amount);
                printf("---------------------------------------------------------------------------\n");
                break;
                
            case 9: // Sort Buyers by Energy Purchased
                printf("\nBuyers Sorted by Energy Purchased:\n");
                display_sorted_buyers(buyer_root);
                printf("---------------------------------------------------------------------------\n");
                break;
                
            case 10: // Display Seller-Buyer Pairs sorted by number of transaction
                printf("\nSeller-Buyer Pairs:\n");
                display_sorted_pair(pair_root);
                printf("------------------------------------------------------------------------\n");
                break;
                
            case 0: // Exit
                // Write data back to files before exiting
                writeSellerData(seller_root);
                writeTransactionData(root);
                freeTransactionTree(root);
                freeSellerTree(seller_root);
                freeBuyerTree(buyer_root);
                freePairTree(pair_root);
                printf("Data saved to files and memory freed. Exiting program...\n");
                printf("--------------------------------------------------------------------------\n");
                exit(0);                
            default:
                printf("Invalid choice! Please try again.\n");
                printf("-----------------------------------------------------------------------\n");
        }
    }
    
    return 0;
}