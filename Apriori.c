



#include "Apriori.h"
#include "ReadData.h"
#include "FrequentItemsetUtilities.h"
#include "AprioriUtilities.h"
#include <time.h>
#include <math.h>

int num_items_current_itemsets;
int total_candidates_generated = 0;




itemset_list_t current_itemset_list, previous_itemset_list, single_item_itemset_list;

itemset_lists_set_t all_frequent_itemset_lists_set;

typedef void (*itemset_generator_t)(void);

int total_trans_ids_allocated = 0;

//itemset_list_item_t* get_new_itemset_list_item(transaction_t transaction) {
//	itemset_list_item_t* new_list_item = malloc(sizeof(itemset_list_item_t));
//	new_list_item->transaction = transaction;
//	return new_list_item;
//}

void append_to_trans_ids_list(trans_ids_list_t* trans_ids_list, int trans_id) {
	trans_ids_list_item_t* new_list_item = (trans_ids_list_item_t*)allocate_trans_ids_list_item(trans_id);


//	printf("Num Elements in List: %d\n", trans_ids_list->num_elements);
	if (trans_ids_list->first_item == NULL) {
		trans_ids_list->first_item = new_list_item;
		trans_ids_list->last_item = new_list_item;
	} else {
		new_list_item->prev_item = trans_ids_list->last_item;
		trans_ids_list->last_item->next_item = new_list_item;
		trans_ids_list->last_item = new_list_item;
	}

	++(trans_ids_list->num_elements);

}

void append_to_itemset_list(itemset_list_t* itemset_list, itemset_list_item_t* itemset_list_item) {

	if (itemset_list->first_item == NULL) {
		itemset_list->first_item = itemset_list_item;
		itemset_list->last_item = itemset_list_item;
	} else {
		itemset_list_item->prev_item = itemset_list->last_item;
		itemset_list->last_item->next_item = itemset_list_item;
		itemset_list->last_item = itemset_list_item;
	}

	++itemset_list->num_elements;

}



itemset_list_t get_null_itemset_list() {
	itemset_list_t itemset_list = {NULL, NULL, 0};
	return itemset_list;
}

itemset_list_t get_itemset_list_1_item() {
	int item_num, trans_id;
	itemset_list_t itemset_list = get_null_itemset_list();
	itemset_list_item_t* itemset_list_item;



	for (item_num = 0; item_num < NUM_ITEMS; ++item_num) {
		itemset_list_item = allocate_single_item_itemset_list_item(item_num);

		for (trans_id = 0; trans_id < NUM_TRANS; ++trans_id) {
			if (dataset.transactions[trans_id][item_num]) {
				append_to_trans_ids_list(&(itemset_list_item->trans_ids_list), trans_id);
				itemset_list_item->matching_prefix_id = 0;
			}

		}

		if (itemset_list_item->trans_ids_list.num_elements >= MIN_SUP) {
			append_to_itemset_list(&itemset_list, itemset_list_item);
		} else {
			deallocate_itemset_list_item(itemset_list_item);
		}

	}

	total_candidates_generated += NUM_ITEMS;
	return itemset_list;

}


void prune_from_itemset_list(itemset_list_item_t* item_to_prune, itemset_list_t* itemset_list) {
	--itemset_list->num_elements;
	deallocate_item(item_to_prune);
}

itemset_list_item_t* get_next_frequent_itemset(itemset_list_t* itemset_list, itemset_list_item_t* current_itemset_list_item) {
	itemset_list_item_t *item_to_prune;

	while (current_itemset_list_item != NULL && current_itemset_list_item->trans_ids_list.num_elements < MIN_SUP) {
		item_to_prune = current_itemset_list_item;
		current_itemset_list_item = current_itemset_list_item->next_item;
		prune_from_itemset_list(item_to_prune, itemset_list);
	}

	return current_itemset_list_item;

}

void find_null(itemset_list_t* itemset_list) {
	itemset_list_item_t* current_itemset_list_item = itemset_list->first_item;
	itemset_list_item_t* next_itemset_list_item = current_itemset_list_item;
	int loop_it = 0;

	while (next_itemset_list_item != NULL) {
		printf("Loop it %d\n", loop_it++);
		next_itemset_list_item = next_itemset_list_item->next_item;
	}
}


void add_items_to_all_frequent_itemsets_list(itemset_list_t itemset_list) {

	if (itemset_list.first_item == NULL) {
//		printf("\nNo candidates for size %d itemsets!!\n\n", num_items_current_itemsets);
		return;
	}

	all_frequent_itemset_lists_set.level_lists[num_items_current_itemsets] = itemset_list;
	all_frequent_itemset_lists_set.total_itemsets += itemset_list.num_elements;
}

void memcpy_transaction(transaction_t dest_transaction, transaction_t src_transaction) {
	memcpy(dest_transaction, src_transaction, sizeof(int)*NUM_ITEMS);
}


void fill_transaction_from_itemsets(itemset_list_item_t* lhs_item, itemset_list_item_t* rhs_item, transaction_t transaction) {
	memcpy_transaction(transaction, lhs_item->transaction);
	transaction[rhs_item->last_item_id] = 1;
}

int is_trans_id_item_null(trans_ids_list_item_t* first_item, trans_ids_list_item_t* second_item) {
	return first_item == NULL || second_item == NULL;
}

//Doesn't work - try to fix?

//void swap_trans_id_list_items(trans_ids_list_item_t* first_item, trans_ids_list_item_t* second_item) {
//	trans_ids_list_item_t temp_item = *first_item;
//	*first_item = *second_item;
//	*second_item = temp_item;
//}

void fill_trans_ids_from_itemsets(itemset_list_item_t* lhs_item, itemset_list_item_t* rhs_item, trans_ids_list_t* trans_ids_list) {

	trans_ids_list_item_t* temp_item;

	trans_ids_list_item_t* small_trans_id_item = lhs_item->trans_ids_list.first_item;
	trans_ids_list_item_t* large_trans_id_item = rhs_item->trans_ids_list.first_item;

	while (1) {

		while (!is_trans_id_item_null(small_trans_id_item, large_trans_id_item) && small_trans_id_item->item_id < large_trans_id_item->item_id) {
			small_trans_id_item = small_trans_id_item->next_item;
		}

		if (is_trans_id_item_null(small_trans_id_item, large_trans_id_item)) {
			return;
		} else if (small_trans_id_item->item_id == large_trans_id_item->item_id) {
			append_to_trans_ids_list(trans_ids_list, small_trans_id_item->item_id);
			small_trans_id_item = small_trans_id_item->next_item;
			large_trans_id_item = large_trans_id_item->next_item;
		} else {
			temp_item = small_trans_id_item;
			small_trans_id_item = large_trans_id_item;
			large_trans_id_item = temp_item;
//			swap_trans_id_list_items(small_trans_id_item, large_trans_id_item);
		}

	}

}

trans_ids_list_t get_null_trans_ids_list() {
	trans_ids_list_t trans_ids_list = {NULL, NULL, 0};
//	trans_ids_list.first_item = NULL;
//	trans_ids_list.last_item = NULL;
//	trans_ids_list.num_elements = 0;
	return trans_ids_list;
}


void generate_itemset(itemset_list_item_t* lhs_item, itemset_list_item_t* rhs_item, int prefix_id) {

	itemset_list_item_t* new_itemset_list_item;
	transaction_t transaction = allocate_new_transaction();
	trans_ids_list_t trans_ids_list = get_null_trans_ids_list();

	fill_transaction_from_itemsets(lhs_item, rhs_item, transaction);
	fill_trans_ids_from_itemsets(lhs_item, rhs_item, &trans_ids_list);


	new_itemset_list_item = create_new_itemset_list_item(&trans_ids_list, transaction, prefix_id);
	new_itemset_list_item->last_item_id = rhs_item->last_item_id;

	if (trans_ids_list.num_elements >= MIN_SUP) {
		append_to_itemset_list(&current_itemset_list, new_itemset_list_item);
	} else {
//		print_pruned_item_info(new_itemset_list_item);
		deallocate_itemset_list_item(new_itemset_list_item);
	}

	++total_candidates_generated;

}

void print_itemset_prefix_ids(itemset_list_item_t* first_item, itemset_list_item_t* second_item) {
	printf("Left  prefix ID: %d\nRight prefix Id: %d\n",first_item->matching_prefix_id, second_item->matching_prefix_id);
}

void generate_current_itemsets() {
	itemset_list_item_t* rhs_item;
	itemset_list_item_t* lhs_item = previous_itemset_list.first_item;
	int new_item_prefix_id = 0;

	while(lhs_item != NULL) {

		rhs_item = lhs_item->next_item;

		while (rhs_item != NULL && lhs_item->matching_prefix_id == rhs_item->matching_prefix_id) {
//			print_itemset_prefix_ids(lhs_item, rhs_item);
			generate_itemset(lhs_item, rhs_item, new_item_prefix_id);
			rhs_item = rhs_item->next_item;
		}

		lhs_item = lhs_item->next_item;
		++new_item_prefix_id;
	}

}

itemset_list_item_t* get_first_lexo_higher_single_item_itemset(int item_id) {

	itemset_list_item_t* current_item = single_item_itemset_list.first_item;

	if (current_item == NULL) {
		printf("NULL!! No Iteration! \n\n");
	}

	while (current_item != NULL) {
//		printf("Next item id: %d, Comparison item id: %d\n", current_item->last_item_id, item_id);

		if (current_item->last_item_id > item_id) {
//			printf("Returned next higher lexo item\n");
			return current_item;
		}

		current_item = current_item->next_item;

	}
//	printf("Returned NULL\n");
	return NULL;



}

void generate_current_itemsets_F1() {
	itemset_list_item_t* base_item = previous_itemset_list.first_item;
	itemset_list_item_t* next_lexo_higher_single_item;

	while (base_item != NULL) {
		next_lexo_higher_single_item = get_first_lexo_higher_single_item_itemset(base_item->last_item_id);

		while(next_lexo_higher_single_item != NULL) {
			generate_itemset(base_item, next_lexo_higher_single_item, 0);
			next_lexo_higher_single_item = next_lexo_higher_single_item->next_item;
		}

		base_item = base_item->next_item;
	}

}

itemset_generator_t get_itemset_generator(enum APRIORI_ANALYSIS_TYPE analysis_type) {
	switch(analysis_type) {
		case F1:
//			printf("F1 Analysis:\n");
			return generate_current_itemsets_F1;
			break;
		case STANDARD:
//			printf("Standard Analysis:\n");
			return generate_current_itemsets;
			break;
		default:
			return NULL;
	}
}

void deallocate_itemset_list(itemset_list_t* itemset_list) {
	itemset_list_item_t* current_list_item = itemset_list->first_item;
	itemset_list_item_t* next_list_item;

	while (current_list_item != NULL) {
		next_list_item = current_list_item->next_item;
		deallocate_itemset_list_item(current_list_item);
		current_list_item = next_list_item;
	}



}

void reset_all_itemset_lists() {
	reset_itemset_list(&current_itemset_list);
	reset_itemset_list(&previous_itemset_list);
	reset_itemset_list(&single_item_itemset_list);
}

void print_itemset_list_set(itemset_lists_set_t* itemset_list_set) {
	int set_level;

	for (set_level = 0; set_level <= NUM_ITEMS; ++set_level) {
		printf("Set Level: %d\n", set_level);
		print_itemset_list(&itemset_list_set->level_lists[set_level]);
	}
}

void print_all_frequent_items() {
	printf("All Frequent Items: \n\n");
	print_itemset_list_set(&all_frequent_itemset_lists_set);
}

void print_num_frequent_itemsets_for_each_itemset_size() {
	int num_items_in_itemsets, num_itemsets_for_itemset_size;

	printf("\nNum Frequent Itemsets for Each Itemset Size: \n\n");

	for (num_items_in_itemsets = 1; num_items_in_itemsets <= NUM_ITEMS; ++num_items_in_itemsets) {
		num_itemsets_for_itemset_size = all_frequent_itemset_lists_set.level_lists[num_items_in_itemsets].num_elements;

		if (num_itemsets_for_itemset_size == 0) {
			break;
		} else {
			printf("Num %d-item Itemsets: %d\n", num_items_in_itemsets, num_itemsets_for_itemset_size);
		}
	}
}

//itemset_list_item_t* deep_copy_itemset_list_item(itemset_list_item_t* itemset_list_item) {
//	itemset_list_item_t* new_itemset_list_item = allocate_null_itemset_list_item();
//	*new_itemset_list_item = *itemset_list_item;
//	new_itemset_list_item->next_item = NULL;
//	return new_itemset_list_item;
//}

transaction_t deep_copy_transaction(transaction_t transaction_to_copy) {
	transaction_t new_transaction = allocate_null_transaction();
	memcpy_transaction(new_transaction, transaction_to_copy);
	return new_transaction;
}

trans_ids_list_t deep_copy_trans_ids_list(trans_ids_list_t trans_ids_list_to_copy) {
	trans_ids_list_t new_trans_ids_list = get_null_trans_ids_list();

	trans_ids_list_item_t* item_to_copy = trans_ids_list_to_copy.first_item;

	while (item_to_copy != NULL) {
		append_to_trans_ids_list(&new_trans_ids_list, item_to_copy->item_id);
		item_to_copy = item_to_copy->next_item;
	}

	return new_trans_ids_list;

}

itemset_list_item_t* deep_copy_itemset_list_item(itemset_list_item_t* item_to_copy) {
//	itemset_list_item_t* new_itemset_list_item = allocate_null_itemset_list_item();
	++num_itemset_list_items_allocated;
	itemset_list_item_t* new_itemset_list_item = malloc(sizeof(itemset_list_item_t));
	new_itemset_list_item->transaction = deep_copy_transaction(item_to_copy->transaction);
	new_itemset_list_item->trans_ids_list = deep_copy_trans_ids_list(item_to_copy->trans_ids_list);

	new_itemset_list_item->last_item_id = item_to_copy->last_item_id;
	new_itemset_list_item->matching_prefix_id = item_to_copy->matching_prefix_id;
	new_itemset_list_item->max_closed_info = item_to_copy->max_closed_info;
//	new_itemset_list_item->rule_set = item_to_copy->rule_set;

	return new_itemset_list_item;
}

void deep_copy_single_item_itemset_list_to_current() {
	itemset_list_item_t *new_current_itemset_list_item, *current_single_item_itemset_list_item;

	current_single_item_itemset_list_item = single_item_itemset_list.first_item;

	while (current_single_item_itemset_list_item != NULL) {
		new_current_itemset_list_item = deep_copy_itemset_list_item(current_single_item_itemset_list_item);
		append_to_itemset_list(&current_itemset_list, new_current_itemset_list_item);
		current_single_item_itemset_list_item = current_single_item_itemset_list_item->next_item;
	}

}


void print_memory_allocation_report() {
	printf("Total trans ids allocated: %d\n", num_trans_ids_items_allocated);
	printf("Total trans ids deallocated: %d\n", num_trans_ids_items_deallocated);
	printf("Total itemset list items deallocated: %d\n", num_itemset_list_items_deallocated);
	printf("Total itemset list items allocated: %d\n", num_itemset_list_items_allocated);
}

void print_efficiency_report() {
	printf("Total Candidates Generated		: %d\n", total_candidates_generated);
	printf("Number Frequent Itemsets  		: %d\n", all_frequent_itemset_lists_set.total_itemsets);
	if (NUM_ITEMS > 30) {
		printf("Num Candidates Generate With    \nBrute Force Itemset Generation	        : 2^%d\n", NUM_ITEMS);
	} else {
		printf("Num Candidates Generate With    \nBrute Force Itemset Generation	        : %d\n", (int)pow(2, NUM_ITEMS));
	}



}

void initialize_frequent_itemsets_lists_set() {
	all_frequent_itemset_lists_set.level_lists = malloc(sizeof(itemset_list_t) * (NUM_ITEMS + 1));
	all_frequent_itemset_lists_set.total_itemsets = 0;
}

void deallocate_itemset_lists_set(itemset_lists_set_t* itemset_lists_set) {
	int set_level;

	for (set_level = 0; set_level < NUM_ITEMS; ++set_level) {
		deallocate_itemset_list(&itemset_lists_set->level_lists[set_level]);
	}

	free(itemset_lists_set->level_lists);

}

void deallocate_dataset(dataset_t* dataset) {
	int trans_index;

	for (trans_index = 0; trans_index < dataset->params.num_trans; ++trans_index) {
		free(dataset->transactions[trans_index]);
	}

	free(dataset->transactions);
}


void print_report(long elapsed_time_nanos) {
	printf("-------------------------------\n\nFrequent Itemset Generation Rule Report:\n\n");
	printf("Min Support: %d\n", MIN_SUP);
	print_efficiency_report();
	print_num_frequent_itemsets_for_each_itemset_size();
	printf("\nTime to Generate Frequent Itemsets (sec): %f\n\n", (float)elapsed_time_nanos / 1000000000);
	printf("---------------------------------\n\n");
}



void apriori_generate_frequent_itemsets(enum APRIORI_ANALYSIS_TYPE analysis_type) {
	itemset_generator_t itemset_generator;
	long elapsed_time_nanos;

	start_timer();
	reset_all_itemset_lists();
	itemset_generator = get_itemset_generator(analysis_type);


	num_items_current_itemsets = 1;
	single_item_itemset_list = get_itemset_list_1_item();
	deep_copy_single_item_itemset_list_to_current();

	initialize_frequent_itemsets_lists_set();
	add_items_to_all_frequent_itemsets_list(current_itemset_list);

	for (num_items_current_itemsets = 2; num_items_current_itemsets <= NUM_ITEMS; ++num_items_current_itemsets) {
		previous_itemset_list = current_itemset_list;
		reset_itemset_list(&current_itemset_list);
		itemset_generator();
		add_items_to_all_frequent_itemsets_list(current_itemset_list);
	}

	elapsed_time_nanos = get_elapsed_time();

	if (print_frequent_itemsets) {
		print_all_frequent_items();
	}

	print_report(elapsed_time_nanos);

	deallocate_itemset_list(&single_item_itemset_list);
	deallocate_itemset_lists_set(&all_frequent_itemset_lists_set);
	deallocate_dataset(&dataset);

	print_memory_allocation_report();
}
