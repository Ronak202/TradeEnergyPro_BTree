#ifndef energytrading_h
#define energytrading_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define ID_SIZE 6
#define DATE_SIZE 25
#define SIZE 200
#define MAX_BUYERS 100
#define MAX_PAIR 100
#define M 4 // Maximum degree of the B-tree
//array of max size of is declared to store these trees

typedef struct transaction_id_tag {
    char transaction_id[ID_SIZE];
    char seller_id[ID_SIZE];
    char buyer_id[ID_SIZE];
    int amount_energy;
    float price_kwh;
    float total_price;
    char timestamp[DATE_SIZE];
} transaction_node;

//Seller id node
typedef struct regularBuyer {
    char buyer_id[ID_SIZE];
    struct regularBuyer* next;
} regularBuyer;

typedef struct seller_id_tag {
    char seller_id[ID_SIZE];
    float price_kwh_gt_300;
    float price_kwh_lt_300;
    float total_revenue;
    struct BTreeNode* Transaction_Subtree;
    regularBuyer* regularBuyers;
} seller_node;


//Buyer id node
typedef struct Buyer_id_tag{
    char buyer_id[ID_SIZE];
    float total_energy_purchased;
    struct BTreeNode* Transaction_Subtree;
}buyer_node;
//forward declaration!!
struct PairBTreeNode;
typedef struct BTreeNode BTreeNode;
// Pair node structure
typedef struct pairs_tag {
    char pair_id[ID_SIZE*2]; // seller_id + buyer_id
    char seller_id[ID_SIZE];
    char buyer_id[ID_SIZE];
    int number_transactions;
    BTreeNode* Transaction_Subtree;
    struct pairs_tag* next;
} pair_node;
// typedef transaction_node transaction_Data;
typedef transaction_node data;
// btree node of trans
typedef struct BTreeNode {
    int num_keys; // Number of keys currently in the node
    data keys[M-1]; // Array of keys
    struct BTreeNode *children[M]; // Array of child pointers
    bool is_leaf; // True if node is a leaf
}BTreeNode;
// B-Tree for sellers
typedef struct SellerBTreeNode {
    int num_keys;
    seller_node* keys[M - 1];
    struct SellerBTreeNode* children[M];
    bool is_leaf;
} SellerBTreeNode;
// B-Tree for Buyers
typedef struct BuyerBTreeNode {
    int num_keys;
    buyer_node* keys[M - 1];
    struct BuyerBTreeNode* children[M];
    bool is_leaf;
} BuyerBTreeNode;
// B-tree node for pairs
typedef struct PairBTreeNode {
    int num_keys;
    pair_node* keys[M-1];
    struct PairBTreeNode *children[M];
    bool is_leaf;
} PairBTreeNode;
//transaction list
typedef struct TransactionList {
     data* tx;
    bool used;  // To mark if it's already printed
} TransactionList;
//buyer list
typedef struct BuyerList
{
     buyer_node *bx;
     bool used;//to mark it used
}buyerlist;
//pair list
typedef struct PairList
{
     pair_node *px;
     bool used;//to mark it used
}pairlist;
// function declaration!!!
int validT(char* s);
int validS(char* s);
int validB(char* s);
float get_valid_price(const char* prompt);
int is_valid_timestamp(const char *timestamp) ;
void getCurrentTimestamp(char *buffer);
transaction_node* createTransactionNode(SellerBTreeNode** root,BuyerBTreeNode** root1,PairBTreeNode**pairRoot, char* transactionID, char* sellerID, char* buyerID, int amt_energy, char* date_time);
void splitChild(struct BTreeNode *parent, int index);
SellerBTreeNode* createSellerNode(bool is_leaf);
struct BTreeNode *createNode(bool is_leaf);
void insertNonFull(struct BTreeNode *node, data key);
void insert(struct BTreeNode **root, data key);
void transaction_given_timeperiod(BTreeNode *root,char *start_time,char *end_time);
void printTableHeader();
void printTableRow(transaction_node* temp);
void traverse(struct BTreeNode *root);
bool search(struct BTreeNode *root, char* key);
void collect_transaction_given_amount(TransactionList *result,BTreeNode *root,double min,double max,int*count);
void display_transaction_given_amount(BTreeNode *root,double min,double max);
void insert_regular_buyer(SellerBTreeNode** root, char* seller_key, char* buyer_id);
void splitSellerChild(SellerBTreeNode* parent, int index);
void insertNonFullSeller(SellerBTreeNode* node, seller_node* key);
seller_node* searchSeller(SellerBTreeNode* root, char* key);
void insertSeller(SellerBTreeNode** root, seller_node* seller);
void check_insert_seller(SellerBTreeNode** root, char* key, data transaction,float price_less,float price_more);
void display_list_of_trans_seller(SellerBTreeNode* root);
void displayAllSellersWithBuyers(SellerBTreeNode* root);
void displayRegularBuyers(seller_node* seller);
BuyerBTreeNode* createBuyerNode(bool is_leaf);
void splitBuyerChild(BuyerBTreeNode* parent, int index);
void insertNonFullBuyer(BuyerBTreeNode* node, buyer_node* key);
void insertBuyer(BuyerBTreeNode** root, buyer_node* Buyer);
buyer_node* searchBuyer(BuyerBTreeNode* root, char* key);
void check_insert_Buyer(BuyerBTreeNode** root, char* key, data transaction);
void  display_list_of_trans_buyer(BuyerBTreeNode* root);
void collect_buyers(BuyerBTreeNode* root, buyerlist* result, int* count);
void sort_buyers(buyerlist* list, int count);
void display_sorted_buyers(BuyerBTreeNode* root);
void print_buyer(buyer_node* buyer);
PairBTreeNode *createPairNode(bool is_leaf);
void splitPairChild(PairBTreeNode* parent, int index);
void insertNonFullPair(PairBTreeNode *node, pair_node* key);
void insertPair(PairBTreeNode** root, pair_node* pair);
pair_node* searchPair(PairBTreeNode* root, char* key);
void check_insert_pair(PairBTreeNode** root,SellerBTreeNode **seller_root,char* key, data transaction);
void total_revenue_particular_seller(SellerBTreeNode**root,char *sellerID);
pair_node* createPair(char* sellerID, char* buyerID, data transaction);
void sort_pairs(pairlist* list, int count);
void collect_pairs(PairBTreeNode* root, pairlist* result, int* count);
void display_sorted_pair(PairBTreeNode* root);
void print_pair(pair_node* pair);
void readSellerData(SellerBTreeNode** seller_root);
void readTransactionData(SellerBTreeNode** seller_root, BuyerBTreeNode** buyer_root,PairBTreeNode** pair_root, BTreeNode** trans_root);
void writeTransactionDataHelper(FILE* file, BTreeNode* root);
void writeTransactionData(BTreeNode* root);
void writeSellerDataHelper(FILE* file, SellerBTreeNode* root);
void writeSellerData(SellerBTreeNode* root) ;
void freeTransactionTree(BTreeNode* root);
void freeSellerTree(SellerBTreeNode* root);
void freeBuyerTree(BuyerBTreeNode* root);
void freePairTree(PairBTreeNode* root);
void freeRegularBuyers(regularBuyer* rb);
#endif
//gcc -o energy buyer_tree.c buyer.c display.c fileoperation.c freememory.c main.c pair_tree.c pair.c seller_tree.c seller.c trans_tree.c transaction.c utils.c