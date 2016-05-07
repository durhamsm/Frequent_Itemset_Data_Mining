/*
 * AprioriUtilities.h
 *
 *  Created on: Mar 30, 2016
 *      Author: durhamsm
 */

#ifndef APRIORIUTILITIES_H_
#define APRIORIUTILITIES_H_

#include "FrequentItemsetUtilities.h"
#include "Apriori.h"
#include <time.h>

extern int num_trans_ids_items_deallocated;
extern int num_trans_ids_items_allocated;
extern int num_itemset_list_items_deallocated;
extern int num_itemset_list_items_allocated;

extern struct timespec time_start, time_stop;

void print_transaction_ids(trans_ids_list_t trans_ids_list);
void print_itemset_list(itemset_list_t* itemset_list);

itemset_list_item_t* allocate_new_itemset_list_item();

itemset_list_item_t* create_new_itemset_list_item(trans_ids_list_t* trans_ids_list, transaction_t transaction, int prefix_id);

trans_ids_list_item_t* allocate_null_trans_ids_list_item();

void deallocate_trans_ids_list(trans_ids_list_t trans_ids_list);

void deallocate_itemset_list_item(itemset_list_item_t* item_to_deallocate);

void reset_itemset_list(itemset_list_t* itemset_list);

void print_pruned_item_info(itemset_list_item_t* item_to_prune);

itemset_list_item_t* allocate_null_itemset_list_item();

itemset_list_item_t* allocate_single_item_itemset_list_item(int item_num);

void print_itemset_list_item(itemset_list_item_t* current_itemset_list_item);

void deallocate_trans_ids_list(trans_ids_list_t trans_ids_list);
void deallocate_transaction(transaction_t transaction);

long get_time_diff_as_nanoseconds(struct timespec time1, struct timespec time2);

void start_timer();

long get_elapsed_time();

#endif /* APRIORIUTILITIES_H_ */
