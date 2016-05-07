/*
 * MaximalClosed.h
 *
 *  Created on: Apr 3, 2016
 *      Author: durhamsm
 */

#ifndef MAXIMALCLOSED_H_
#define MAXIMALCLOSED_H_

#include "Apriori.h"
#include "ReadData.h"
#include "FrequentItemSet.h"
#include <math.h>

void assign_maximal_and_closed_status(itemset_lists_set_t* itemset_lists_set);

int get_previous_present_item_id(transaction_t superset_transaction, int start_search_item_id);
int get_next_present_item_id(transaction_t transaction, int start_search_item_id);
itemset_list_item_t* get_next_k_minus_1_subset_itemset_list_item(itemset_list_item_t* search_start_itemset_list_item, transaction_t superset_transaction, int item_id_to_skip);

int is_k_minus_1_subset(transaction_t k_minus_1_subset_transaction, transaction_t superset_transaction, int item_id_to_skip);

#endif /* MAXIMALCLOSED_H_ */
