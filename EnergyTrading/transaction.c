#include"energytrading.h"
//create transaction_node
transaction_node* createTransactionNode(SellerBTreeNode** root,BuyerBTreeNode** root1,PairBTreeNode**pairRoot, char* transactionID, char* sellerID, char* buyerID, int amt_energy, char* date_time) {
    transaction_node* newtransaction = (transaction_node*)malloc(sizeof(transaction_node));
    if (!newtransaction) return NULL;
    strcpy(newtransaction->transaction_id, transactionID);
    strcpy(newtransaction->buyer_id, buyerID);
    strcpy(newtransaction->seller_id, sellerID);
    newtransaction->amount_energy = amt_energy;
    float price_less = 0, price_more = 0;
    seller_node* sellerRecord = searchSeller(*root, sellerID);  // Already returns the seller_node*, not B-tree node
    if (sellerRecord) {
        // Seller exists - use their stored pricing
        newtransaction->price_kwh = (amt_energy > 300) ? sellerRecord->price_kwh_gt_300 : sellerRecord->price_kwh_lt_300;
    } else {
        // Seller doesn't exist - ask for price input
        printf("Seller ID: %s\n",sellerID);
        printf("Enter price for seller per kWh for amounts < 300:$ ");
        scanf("%f", &price_less);
        printf("Enter price for seller  per kWh for amounts > 300:$ ");
        scanf("%f", &price_more);
        newtransaction->price_kwh = (amt_energy > 300) ? price_more : price_less;
    }
    // Compute total price
    newtransaction->total_price = amt_energy * newtransaction->price_kwh;
    strcpy(newtransaction->timestamp, date_time);
     
    // Insert into seller tree and transaction tree
    check_insert_seller(root, sellerID, *newtransaction, price_less, price_more);
    check_insert_Buyer(root1, buyerID, *newtransaction);
    char pairid[ID_SIZE*2] = {0};  // Stack allocation is safer for fixed-size IDs
    strcpy(pairid, sellerID);      // Copy sellerID first
    strcat(pairid, buyerID);
    check_insert_pair(pairRoot,root,pairid,*newtransaction);
    return newtransaction;
}
// Find all transactions in a Given Time Period
void transaction_given_timeperiod(BTreeNode *root,char *start_time,char *end_time)
{
    if(root!=NULL)
       {

        int i=0;
        for(i=0;i<root->num_keys;i++)
         {
            transaction_given_timeperiod(root->children[i],start_time,end_time);
            if ((strcmp(root->keys[i].timestamp, start_time) >= 0) &&(strcmp(root->keys[i].timestamp, end_time) <= 0)) 
            {
                printTableRow(&(root->keys[i]));
            }  
         }  
      transaction_given_timeperiod(root->children[i],start_time,end_time);
        }
}
//Find and Display (in ascending order) the transactions with the Energy Amounts between certain range!!
void collect_transaction_given_amount(TransactionList *result,BTreeNode *root,double min,double max,int*count)
{
    if(root!=NULL)
   {
     int i=0;
    for( i=0;i<root->num_keys;i++)
    {
        collect_transaction_given_amount(result,root->children[i],min,max,count);
        if((root->keys[i]).amount_energy>=min&&(root->keys[i]).amount_energy<=max)
        {
              result[*count].used=false;
              result[*count].tx=&(root->keys[i]);
              (*count)++;
        }
    }
        collect_transaction_given_amount(result,root->children[i],min,max,count);
   }
}
void display_transaction_given_amount(BTreeNode *root,double min,double max)
{
      TransactionList result[100];  
      int count = 0;
      collect_transaction_given_amount(result,root,min,max,&count);
      printTableHeader();

      for (int printed = 0; printed < count; printed++) {
          int min_index = -1;
          for (int i = 0; i < count; i++) {
              if (!result[i].used && 
                  (min_index == -1 || result[i].tx->amount_energy < result[min_index].tx->amount_energy)) {
                  min_index = i;
              }
          }
          if (min_index != -1) {
              printTableRow(result[min_index].tx);
              result[min_index].used = true;
          }
      }
  }