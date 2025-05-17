#include"energytrading.h"
// Utility functions from utils.c
int validT(char* s) {
    int ans = 1;
    if (strlen(s) != 5 || s[0] != 'T' || s[1] == '0') {
        ans = 0;
    }

    for (int i = 1; s[i] != '\0' && ans; i++) {
        if (s[i] < '0' || s[i] > '9') {
            ans = 0;
        }
    }
    return ans;
}
//valid seller 
int validS(char* s) {
    int ans = 1;
    if (strlen(s) != 5 || s[0] != 'S' || s[1] == '0') {
        ans = 0;
    }
    if (ans) {
        for (int i = 1; s[i] != '\0' && ans; i++) {
            if (s[i] < '0' || s[i] > '9') {
                ans = 0;
            }
        }
    }
    return ans;
}
// valid buyer 
int validB(char* s) {
    int ans = 1;
    if (strlen(s) != 5 || s[0] != 'B' || s[1] == '0') {
        ans = 0;
    }
    if (ans) {
        for (int i = 1; s[i] != '\0' && ans; i++) {
            if (s[i] < '0' || s[i] > '9') {
                ans = 0;
            }
        }
    }
    return ans;
}
//valid price
float get_valid_price(const char* prompt) {
    float price;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &price) == 1 && price > 0) {
            return price;
        }
        printf("Invalid price! Price must be greater than 0.\n");
        while (getchar() != '\n');
    }
}
//is valid year or not
int is_valid_timestamp(const char *timestamp) {
    int year, month, day, hour, minute, second;
    char extra;

    // Check format using sscanf extract value from string
    if (sscanf(timestamp, "%4d-%2d-%2d %2d:%2d:%2d%c", 
               &year, &month, &day, &hour, &minute, &second, &extra) != 6) {
        return 0; 
    }

    // Basic range checks
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;

    return 1; // Valid timestamp
}
//to get current dat time!!
void getCurrentTimestamp(char *buffer) { 
    time_t now = time(NULL);//current timestamp
    struct tm *tm_info = localtime(&now);//convert raw time to local time
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);//fomat tmminfo into string like
}
//searching the node to present in tree!!
bool search(struct BTreeNode *root, char* key) {
    if (root == NULL) {
        return false;
    }

    int i = 0;
    while (i < root->num_keys && strcmp(key,root->keys[i].transaction_id)>0) {
        i++;
    }
    if (i < root->num_keys && strcmp(key,root->keys[i].transaction_id)==0) {
        return true;
    }
    if (root->is_leaf) {
        return false;
    }
    return search(root->children[i], key);
}