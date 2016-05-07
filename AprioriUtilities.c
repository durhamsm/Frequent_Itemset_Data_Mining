

#include "AprioriUtilities.h"

int num_trans_ids_items_deallocated = 0;
int num_trans_ids_items_allocated = 0;
int num_itemset_list_items_deallocated = 0;
int num_itemset_list_items_allocated = 0;

struct timespec time_start, time_stop;


int is_maximal(max_closed_info_t max_closed_info) {
	return max_closed_info.num_super_sets == max_closed_info.num_super_sets_lower_support;
}

int is_closed(max_closed_info_t max_closed_info) {
	return max_closed_info.num_super_sets == 0;
}

void print_maximal_closed_status(max_closed_info_t max_closed_info) {
	printf("; Maximal: %d, ", is_maximal(max_closed_info));
	printf("Closed: %d, ", is_closed(max_closed_info));
	printf("Num Supersets: %d, ", max_closed_info.num_super_sets);
	printf("Num Supersets With Lower Sup: %d, ", max_closed_info.num_super_sets_lower_support);
}

void print_transaction_ids(trans_ids_list_t trans_ids_list) {
	printf(", Trans ids: ");

	trans_ids_list_item_t* trans_ids_list_item = trans_ids_list.first_item;

	while(trans_ids_list_item != NULL) {
		printf(", %d", trans_ids_list_item->item_id);
		trans_ids_list_item = trans_ids_list_item->next_item;
	}

}

void print_itemset_list_item(itemset_list_item_t* current_itemset_list_item) {
	print_transaction(current_itemset_list_item->transaction);
	printf(", Prefix id: %d, Last item id: %d", current_itemset_list_item->matching_prefix_id, current_itemset_list_item->last_item_id);
//	print_transaction_ids(current_itemset_list_item->trans_ids_list);
	printf(", Number of Occurrences: %d", current_itemset_list_item->trans_ids_list.num_elements);
	print_maximal_closed_status(current_itemset_list_item->max_closed_info);
	putchar('\n');
}

void print_itemset_list(itemset_list_t* itemset_list) {
	itemset_list_item_t* current_itemset_list_item = itemset_list->first_item;

	while(current_itemset_list_item != NULL) {
		print_itemset_list_item(current_itemset_list_item);
		current_itemset_list_item = current_itemset_list_item->next_item;
	}
	putchar('\n');
	putchar('\n');

}


itemset_list_item_t* create_new_itemset_list_item(trans_ids_list_t* trans_ids_list, transaction_t transaction, int prefix_id) {
//	itemset_list_item_t* new_item = allocate_null_itemset_list_item();
	++num_itemset_list_items_allocated;
	itemset_list_item_t* new_item = malloc(sizeof(itemset_list_item_t));
	new_item->prev_item = NULL;
	new_item->next_item = NULL;

	new_item->trans_ids_list = *trans_ids_list;
	new_item->transaction = transaction;
	new_item->matching_prefix_id = prefix_id;
	return new_item;
}

trans_ids_list_item_t* allocate_null_trans_ids_list_item() {
	++num_trans_ids_items_allocated;
	trans_ids_list_item_t* new_list_item = malloc(sizeof(trans_ids_list_item_t));
	new_list_item->next_item = NULL;
	new_list_item->prev_item = NULL;
	return new_list_item;
}

trans_ids_list_item_t* allocate_trans_ids_list_item(int trans_id) {
//	printf("Allocate Trans ID: %d\n", trans_id);
	trans_ids_list_item_t* new_list_item = allocate_null_trans_ids_list_item();
	new_list_item->item_id = trans_id;
	return new_list_item;
}



void deallocate_trans_ids_list(trans_ids_list_t trans_ids_list) {
	trans_ids_list_item_t* current_trans_ids_list_item = trans_ids_list.first_item;
	trans_ids_list_item_t* next_trans_ids_list_item;
//	printf("Deallocate Trans ids: ");
	while (current_trans_ids_list_item != NULL) {
//		printf(", %d", current_trans_ids_list_item->item_id);
		++num_trans_ids_items_deallocated;
		next_trans_ids_list_item = current_trans_ids_list_item->next_item;
		free(current_trans_ids_list_item);
		current_trans_ids_list_item = next_trans_ids_list_item;
	}

//	printf("Total trans ids deallocated: %d\n", num_trans_ids_items_deallocated);

}

void deallocate_transaction(transaction_t transaction) {
	//Deallocation turned off - causing problems
	free(transaction);
}

void deallocate_itemset_list_item(itemset_list_item_t* item_to_deallocate) {
	++num_itemset_list_items_deallocated;
	deallocate_transaction(item_to_deallocate->transaction);
	deallocate_trans_ids_list(item_to_deallocate->trans_ids_list);
	free(item_to_deallocate);
}

void reset_itemset_list(itemset_list_t* itemset_list) {
	itemset_list->first_item = NULL;
	itemset_list->last_item = NULL;
	itemset_list->num_elements = 0;
}

void print_pruned_item_info(itemset_list_item_t* item_to_prune) {
	printf("Prune itemset, only %d occurences\n", item_to_prune->trans_ids_list.num_elements);
	print_transaction(item_to_prune->transaction);
	print_transaction_ids(item_to_prune->trans_ids_list);
	putchar('\n');
}

trans_ids_list_t get_null_trans_id_list() {
	trans_ids_list_t trans_ids_list;
	trans_ids_list.num_elements = 0;
	trans_ids_list.first_item = NULL;
	trans_ids_list.last_item = NULL;
	return trans_ids_list;
}

max_closed_info_t get_null_max_closed_info() {
	max_closed_info_t max_closed_info = {0, 0};
	return max_closed_info;
}

//rule_set_t get_null_rule_set() {
//	rule_set_t rule_set;
//	rule_set.total_rules = 0;
//	return rule_set;
//}

itemset_list_item_t* allocate_null_itemset_list_item() {
	++num_itemset_list_items_allocated;
	itemset_list_item_t* itemset_list_item = malloc(sizeof(itemset_list_item_t));
	itemset_list_item->prev_item = NULL;
	itemset_list_item->next_item = NULL;
	itemset_list_item->transaction = allocate_null_transaction();
	itemset_list_item->trans_ids_list = get_null_trans_id_list();
	itemset_list_item->max_closed_info = get_null_max_closed_info();
//	itemset_list_item->rule_set = get_null_rule_set();

	return itemset_list_item;
}

itemset_list_item_t* allocate_single_item_itemset_list_item(int item_num) {
	itemset_list_item_t* itemset_list_item = allocate_null_itemset_list_item();
	itemset_list_item->transaction[item_num] = 1;
	itemset_list_item->last_item_id = item_num;
	return itemset_list_item;
}


long get_time_diff_as_nanoseconds(struct timespec time1, struct timespec time2) {
	return 1000000000 * (time2.tv_sec - time1.tv_sec) + time2.tv_nsec - time1.tv_nsec;
}

void start_timer() {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
}

long get_elapsed_time() {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stop);
	return get_time_diff_as_nanoseconds(time_start, time_stop);
}
