# TradeEnergyPro_BTree(Using BTrees)
## Overview
TradeEnergyPro is an Energy Trading Record Management System designed for smart grid environments. It enables accurate recording, tracking, and analysis of energy trading transactions between sellers and buyers to optimize operations and manage financial records.

## Transaction Attributes
- **Transaction ID**: Unique identifier for each transaction.
- **Buyer ID**: Unique ID of the energy buyer.
- **Seller ID**: Unique ID of the energy seller (reference to seller node).
- **Amount of Energy (kWh)**: Energy traded in kilowatt-hours.
- **Price per kWh**: Price at which energy is traded.
- **Total Price:**: Computed as Amount of Energy * Price per kWh. 
- **Timestamp**: Date and time of the transaction.

## Seller Attributes
- **Seller ID**
- **Energy rate per kWh (below 300 kWh)**
- **Energy rate per kWh (above 300 kWh)**
- **Regular buyers**: Buyers with more than five transactions (stored as a linked list of Buyer IDs).
-**Transaction Subtree:**:A reference to a tree containing all transactions associated with this seller.

## Buyer Attributes
- **Buyer ID**
- **Total Energy Purchased**: Sum of all energy bought. 
-**Transaction Subtree:**::A reference to a tree containing all transactions associated with this buyer. 
  
## Functional Requirements
- **Add New Transactions**: Automatically applies the correct seller rate.
- **Display All Transactions**: Lists all stored transactions.
- **Transaction Lists**:
  - For each seller
  - For each buyer
- **Filter Transactions by Time Period**.
- **Calculate Seller Revenue**: Total revenue for a specific seller.
- **Sorting Features**:
  - Buyers by energy purchased.
  - Seller/Buyer pairs by transaction count.
- **Display transaction in the given range Amount**:Find and Display (in ascending order) the transactions with the Energy Amounts between
certain range. 

Output:
![image](https://github.com/user-attachments/assets/9462c32c-c8da-4283-90d7-56bea36624c5)
![image](https://github.com/user-attachments/assets/4da440c6-5896-4894-99fb-42ba6b8a9140)
![image](https://github.com/user-attachments/assets/d133c30a-08c8-45dd-b0b4-619e7d04d4aa)
![image](https://github.com/user-attachments/assets/d9324d9c-3b3f-444d-b29d-8bfac6a94135)

Sort by Seller Buyer pair
![image](https://github.com/user-attachments/assets/64a17e0f-ad62-4b9a-83a5-c8d9208dec7e)




This project efficiently manages energy trading data using linked lists for optimized storage and retrieval.
