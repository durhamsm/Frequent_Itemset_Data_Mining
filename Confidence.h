/*
 * Confidence.h
 *
 *  Created on: Apr 3, 2016
 *      Author: durhamsm
 */

#ifndef CONFIDENCE_H_
#define CONFIDENCE_H_

#include "Apriori.h"
#include "ReadData.h"
#include "FrequentItemSet.h"
#include "AprioriUtilities.h"
#include "MaximalClosed.h"
#include <time.h>
#include <math.h>



typedef struct rule_level_item_t {
	struct rule_level_item_t* prev_item;
	struct rule_level_item_t* next_item;
	itemset_list_item_t* predicate_itemset;
	itemset_list_item_t* consequent_itemset;
	trans_ids_list_t cocurrent_trans_ids_list;
} rule_level_item_t;

typedef struct rule_level_t {
	rule_level_item_t* first_item;
	rule_level_item_t* last_item;
	int num_rules;
	int num_items_in_predicate;
	int num_items_in_consequent;
} rule_level_t;

typedef struct itemset_rule_list_t {
	rule_level_t* rule_levels_array;
//	rule_list_item_t* first_item;
//	rule_list_item_t* last_item;
	itemset_list_item_t* base_itemset;
	int total_rule_levels;
} itemset_rule_list_t;

//For itemsets of given size
typedef struct rule_lists_set_t {
	itemset_rule_list_t* itemset_rule_lists_array;
	int num_items_in_itemsets;
	int num_rule_lists;
} rule_lists_set_t;

void generate_association_rules(itemset_lists_set_t* itemset_lists_set);


#endif /* CONFIDENCE_H_ */
