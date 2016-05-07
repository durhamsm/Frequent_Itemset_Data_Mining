
#ifndef APRIORI_H_
#define APRIORI_H_

#include "FrequentItemsetUtilities.h"
#include "FrequentItemSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int MIN_SUP;
extern float MIN_CONFIDENCE;

enum APRIORI_ANALYSIS_TYPE {
	STANDARD,
	F1
};

typedef struct trans_ids_list_item_t {
	int item_id;
	struct trans_ids_list_item_t* next_item;
	struct trans_ids_list_item_t* prev_item;
} trans_ids_list_item_t;


typedef struct trans_ids_list_t {
	trans_ids_list_item_t* first_item;
	trans_ids_list_item_t* last_item;
	int num_elements;
} trans_ids_list_t;

typedef struct max_closed_info_t {
	int num_super_sets;
	int num_super_sets_lower_support;
} max_closed_info_t;




typedef struct itemset_list_item_t {
	struct itemset_list_item_t* next_item;
	struct itemset_list_item_t* prev_item;
	transaction_t transaction;
	trans_ids_list_t trans_ids_list;
	int last_item_id;
	int matching_prefix_id;
	max_closed_info_t max_closed_info;
//	rule_set_t rule_set;
} itemset_list_item_t;


typedef struct itemset_list_t {
	itemset_list_item_t* first_item;
	itemset_list_item_t* last_item;
	int num_elements;
} itemset_list_t;

typedef struct itemsets_list_set_t {
	itemset_list_t* level_lists;
	int total_itemsets;
} itemset_lists_set_t;




extern itemset_lists_set_t all_frequent_itemset_lists_set;


void apriori_generate_frequent_itemsets();

void memcpy_transaction(transaction_t dest_transaction, transaction_t src_transaction);

void print_all_frequent_items();

void fill_trans_ids_from_itemsets(itemset_list_item_t* lhs_item, itemset_list_item_t* rhs_item, trans_ids_list_t* trans_ids_list);

trans_ids_list_t get_null_trans_ids_list();

void fill_transaction_from_itemsets(itemset_list_item_t* lhs_item, itemset_list_item_t* rhs_item, transaction_t transaction);

extern enum APRIORI_ANALYSIS_TYPE apriori_analysis_type;

#endif
