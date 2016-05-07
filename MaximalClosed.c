/*
 * MaximalClosed.c
 *
 *  Created on: Apr 3, 2016
 *      Author: durhamsm
 */


#include "MaximalClosed.h"



int is_k_minus_1_subset(transaction_t k_minus_1_subset_transaction, transaction_t superset_transaction, int item_id_to_skip) {
	int item_id;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		if (item_id == item_id_to_skip) {
			continue;
		}

		if (k_minus_1_subset_transaction[item_id] != superset_transaction[item_id]) {
			return 0;
		}

	}

	return 1;

}

itemset_list_item_t* get_next_k_minus_1_subset_itemset_list_item(itemset_list_item_t* search_start_itemset_list_item, transaction_t superset_transaction, int item_id_to_skip) {
	itemset_list_item_t* current_itemset_list_item = search_start_itemset_list_item;

	while (current_itemset_list_item != NULL) {
		if (is_k_minus_1_subset(current_itemset_list_item->transaction, superset_transaction, item_id_to_skip)) {
			break;
		}

		current_itemset_list_item = current_itemset_list_item->next_item;
	}

	return current_itemset_list_item;
}

int get_previous_present_item_id(transaction_t transaction, int start_search_item_id) {
	int current_item_id = start_search_item_id;

	while (1) {
		if (transaction[current_item_id] == 1) {
			return current_item_id;
		} else {
			--current_item_id;
		}
	}

	return -1;
}

int get_next_present_item_id(transaction_t transaction, int start_search_item_id) {
	int current_item_id = start_search_item_id;

	for (current_item_id = start_search_item_id; current_item_id < NUM_ITEMS; ++current_item_id) {
		if (transaction[current_item_id] == 1) {
			return current_item_id;
		}
	}

	return -1;
}

void iterate_counts_for_itemsets(itemset_list_item_t* superset_itemset_list_item, itemset_list_item_t* k_minus_1_matching_subset) {

	++k_minus_1_matching_subset->max_closed_info.num_super_sets;

	if (k_minus_1_matching_subset->trans_ids_list.num_elements > superset_itemset_list_item->trans_ids_list.num_elements) {
		++k_minus_1_matching_subset->max_closed_info.num_super_sets_lower_support;
	}


}


void iterate_counts_for_k_minus_1_subset_itemsets(itemset_lists_set_t* itemset_lists_set, itemset_list_item_t* superset_itemset_list_item, int set_level) {
	itemset_list_item_t* current_k_minus_1_itemset_list_item, *next_k_minus_1_subset_itemset_list_item;
	int ith_item_index, next_item_id_to_skip;
	transaction_t superset_transaction;

	current_k_minus_1_itemset_list_item = itemset_lists_set->level_lists[set_level - 1].first_item;
	superset_transaction = superset_itemset_list_item->transaction;

	next_item_id_to_skip = superset_itemset_list_item->last_item_id;
//	printf("Superset Itemset: ");
//	print_itemset_list_item(superset_itemset_list_item);

	for (ith_item_index = set_level; ith_item_index > 0; --ith_item_index) {
//		printf("Item Id to Skip: %d\n", next_item_id_to_skip);

		next_k_minus_1_subset_itemset_list_item = get_next_k_minus_1_subset_itemset_list_item(current_k_minus_1_itemset_list_item, superset_transaction, next_item_id_to_skip);


		if (next_k_minus_1_subset_itemset_list_item != NULL) {
//			printf("Next Subset: ");
//			print_itemset_list_item(next_k_minus_1_subset_itemset_list_item);

			iterate_counts_for_itemsets(superset_itemset_list_item, next_k_minus_1_subset_itemset_list_item);

//			printf("Next Subset after iteration: ");
//			print_itemset_list_item(next_k_minus_1_subset_itemset_list_item);
			current_k_minus_1_itemset_list_item = next_k_minus_1_subset_itemset_list_item;
		}

		next_item_id_to_skip = get_previous_present_item_id(superset_transaction, next_item_id_to_skip - 1);

	}


	int b = 7;

}

void assign_maximal_closed_status_for_level(itemset_lists_set_t* itemset_lists_set, int set_level) {

	itemset_list_t curr_level_itemset_list = itemset_lists_set->level_lists[set_level];
	itemset_list_item_t* curr_level_curr_item = curr_level_itemset_list.first_item;

	while (curr_level_curr_item != NULL) {
		iterate_counts_for_k_minus_1_subset_itemsets(itemset_lists_set, curr_level_curr_item, set_level);
		curr_level_curr_item = curr_level_curr_item->next_item;
	}


}

typedef struct maximal_closed_counts_t {
	int maximal_count;
	int closed_count;
} maximal_closed_counts_t;

//int get_num_maximal_itemsets_in_list(itemset_list_t* itemset_list) {
//	int num_closed_itemsets = 0;
//	itemset_list_item_t* current_itemset_list_item = itemset_list->first_item;
//
//	while (current_itemset_list_item != NULL) {
//		num_closed_itemsets += is_closed(current_itemset_list_item->max_closed_info);
//		current_itemset_list_item = current_itemset_list_item->next_item;
//	}
//	return num_closed_itemsets;
//
//}

maximal_closed_counts_t get_num_maximal_closed_itemsets_in_list(itemset_list_t* itemset_list) {
	itemset_list_item_t* current_itemset_list_item = itemset_list->first_item;
	maximal_closed_counts_t max_closed_counts = {0, 0};

	while (current_itemset_list_item != NULL) {
		max_closed_counts.closed_count += is_closed(current_itemset_list_item->max_closed_info);
		max_closed_counts.maximal_count += is_maximal(current_itemset_list_item->max_closed_info);
		current_itemset_list_item = current_itemset_list_item->next_item;
	}

	return max_closed_counts;

}

void print_maximal_closed_report(itemset_lists_set_t* itemset_lists_set) {
	int itemset_list_index;
	maximal_closed_counts_t total_maximal_closed_counts = {0, 0};
	maximal_closed_counts_t list_maximal_closed_counts;

	for (itemset_list_index = 0; itemset_list_index < NUM_ITEMS; ++itemset_list_index) {
		list_maximal_closed_counts = get_num_maximal_closed_itemsets_in_list(&itemset_lists_set->level_lists[itemset_list_index]);
		total_maximal_closed_counts.closed_count += list_maximal_closed_counts.closed_count;
		total_maximal_closed_counts.maximal_count += list_maximal_closed_counts.maximal_count;
	}


	printf("Maximal and Closed Itemsets Report: \n\n");
	printf("Total Maximal Itemsets: %d\n", total_maximal_closed_counts.maximal_count);
	printf("Total Closed Itemsets: %d\n\n", total_maximal_closed_counts.closed_count);

}


void assign_maximal_and_closed_status(itemset_lists_set_t* itemset_lists_set) {
	int set_level;

	for (set_level = NUM_ITEMS; set_level >= 2; --set_level) {
//		printf("Set Level: %d\n", set_level);

		if (itemset_lists_set->level_lists[set_level].num_elements == 0) {
			continue;
		}

		assign_maximal_closed_status_for_level(itemset_lists_set, set_level);
	}

	print_maximal_closed_report(itemset_lists_set);


}

