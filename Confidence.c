/*
 * Confidence.c
 *
 *  Created on: Apr 3, 2016
 *      Author: durhamsm
 */


#include "Confidence.h"



rule_lists_set_t* all_rule_lists_sets_array;
int current_rule_set_index = 0;
int total_confidence_candidates_generated = 0;

float probability_to_print_rule;

rule_lists_set_t* allocate_rule_lists_sets_array() {
	int rule_lists_set_index = 0;
	rule_lists_set_t* rule_lists_sets_array;
	rule_lists_sets_array = malloc(sizeof(rule_lists_set_t) * (NUM_ITEMS + 1));

	for (rule_lists_set_index = 0; rule_lists_set_index < NUM_ITEMS + 1; ++rule_lists_set_index) {
		rule_lists_sets_array[rule_lists_set_index].num_rule_lists = 0;
	}


	return rule_lists_sets_array;
}

itemset_rule_list_t* allocate_rule_lists_array(int num_rule_lists) {
	itemset_rule_list_t* rule_list_array;
	rule_list_array = malloc(sizeof(itemset_rule_list_t) * num_rule_lists);
	return rule_list_array;
}

void fill_transaction_with_zeros(transaction_t transaction) {
	memset(transaction, 0, sizeof(int) * NUM_ITEMS);
}

float get_confidence(rule_level_item_t* rule) {
	return (float)rule->cocurrent_trans_ids_list.num_elements / rule->predicate_itemset->trans_ids_list.num_elements;
}

float get_probability_that_itemset_occurs_in_record(itemset_list_item_t* itemset) {
	return (float)itemset->trans_ids_list.num_elements / NUM_TRANS;
}

float get_lift(rule_level_item_t* rule_level_item) {
	return get_confidence(rule_level_item) / get_probability_that_itemset_occurs_in_record(rule_level_item->consequent_itemset);
}

void deallocate_rule_list_item(rule_level_item_t* rule) {
//	deallocate_trans_ids_list(rule->cocurrent_trans_ids_list);
	free(rule);
}

void add_rule_to_rule_level(rule_level_item_t* new_rule, rule_level_t* rule_level) {

	if (rule_level->first_item == NULL) {
		rule_level->first_item = new_rule;
		rule_level->last_item = new_rule;
	} else {
		new_rule->prev_item = rule_level->last_item;
		rule_level->last_item->next_item = new_rule;
		rule_level->last_item = new_rule;
	}

	++rule_level->num_rules;


}

rule_level_item_t* allocate_new_rule_list_item() {
	return malloc(sizeof(rule_level_item_t));
}



int is_transactions_equal(transaction_t first_transaction, transaction_t second_transaction) {
	int item_id;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		if (first_transaction[item_id] != second_transaction[item_id]) {
			return 0;
		}
	}

	return 1;
}


void fill_concurrent_trans_ids_for_rule(rule_level_item_t* rule) {
	++total_confidence_candidates_generated;
	fill_trans_ids_from_itemsets(rule->predicate_itemset, rule->consequent_itemset, &rule->cocurrent_trans_ids_list);
}

void print_transaction_with_header_strings(transaction_t transaction) {
	int item_id;
	int is_first_print_item = 1;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		if (transaction[item_id] == 1) {
			if (is_first_print_item == 1) {
				printf("(%s) ", column_headers[item_id]);
				is_first_print_item = 0;
			} else {
				printf("+ (%s) ", column_headers[item_id]);
			}


		}
	}

}

float get_random_number_between_zero_and_one() {
	return (float)rand() / RAND_MAX;
}

int is_rule_to_be_printed() {
	return print_specified_num_of_association_rules && get_random_number_between_zero_and_one() < probability_to_print_rule;
}

int get_support(rule_level_item_t* rule) {
	return rule->cocurrent_trans_ids_list.num_elements;
}

void print_rule(rule_level_item_t* rule_level_item) {

	if (!print_high_lift_rules || (print_high_lift_rules && get_lift(rule_level_item) > high_lift_threshold)) {
		if (is_rule_to_be_printed()) {
				//	print_transaction(rule_level_item->predicate_itemset->transaction);


			print_transaction_with_header_strings(rule_level_item->predicate_itemset->transaction);
			printf("  ->  ");
			print_transaction_with_header_strings(rule_level_item->consequent_itemset->transaction);
			printf(", Confidence: %f, ", get_confidence(rule_level_item));
			printf("Lift: %f, ", get_lift(rule_level_item));
			printf("Support: %d, ", get_support(rule_level_item));
			putchar('\n');
		}

	}



}

itemset_list_item_t * get_itemset_list_item(itemset_list_t* itemset_list, transaction_t search_transaction) {

	itemset_list_item_t* current_itemset_list_item = itemset_list->first_item;

	while (current_itemset_list_item != NULL) {

		if (is_transactions_equal(current_itemset_list_item->transaction, search_transaction)) {
			return current_itemset_list_item;
		}

		current_itemset_list_item = current_itemset_list_item->next_item;
	}

	printf("Error!! No Transaction Found!!\n");

	return NULL;


}

void fill_transaction_with_intersection(transaction_t lhs_transaction, transaction_t rhs_transaction, transaction_t result_transaction) {
	int item_id;
	fill_transaction_with_zeros(result_transaction);

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		if (lhs_transaction[item_id] == rhs_transaction[item_id]) {
			result_transaction[item_id] = lhs_transaction[item_id];
		}
	}


}

int consequent_itemset_prefix_ids_match(rule_level_item_t* first_rule, rule_level_item_t* second_rule) {
	return first_rule->consequent_itemset->matching_prefix_id == second_rule->consequent_itemset->matching_prefix_id;
}

void fill_transaction_with_difference(transaction_t lhs_transaction, transaction_t rhs_transaction, transaction_t result_transaction) {
	int item_id;
	memcpy(result_transaction, lhs_transaction, sizeof(int) * NUM_ITEMS);

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		if (lhs_transaction[item_id] == rhs_transaction[item_id]) {
			result_transaction[item_id] = 0;
		}
	}

}


rule_level_item_t* allocate_new_rule(itemset_list_item_t *predicate_itemset, itemset_list_item_t *consequent_itemset) {
	rule_level_item_t* new_rule = allocate_new_rule_list_item();
	new_rule->cocurrent_trans_ids_list = get_null_trans_ids_list();
	new_rule->prev_item = NULL;
	new_rule->next_item = NULL;
	new_rule->predicate_itemset = predicate_itemset;
	new_rule->consequent_itemset = consequent_itemset;
	return new_rule;
}

//rule_level_item_t* allocate_new_rule(itemset_list_item_t* current_itemset_list_item, itemset_list_item_t* base_predicate_itemset, itemset_list_item_t* to_match_predicate_itemset, int num_items_in_predicate, int num_items_in_consequent) {
//	itemset_list_item_t *new_rule_predicate_itemset, *new_rule_consequent_itemset;
//	transaction_t new_rule_predicate_transaction;
//	rule_level_item_t* new_rule;
//
//	new_rule_predicate_transaction = allocate_null_transaction();
//
//	fill_transaction_from_itemsets(base_predicate_itemset, to_match_predicate_itemset, new_rule_predicate_transaction);
//
//	new_rule_predicate_itemset = get_itemset_list_item(&all_frequent_itemset_lists_set.level_lists[num_items_in_predicate], new_rule_predicate_transaction);
//	new_rule_consequent_itemset = get_itemset_list_item_for_consequent(current_itemset_list_item->transaction, new_rule_predicate_transaction, &all_frequent_itemset_lists_set.level_lists[num_items_in_consequent]);
//
//	new_rule->predicate_itemset = new_rule_predicate_itemset;
//	new_rule->consequent_itemset = new_rule_consequent_itemset;
//
//	new_rule = allocate_new_rule(new_rule_predicate_itemset, new_rule_consequent_itemset);
//
//	return new_rule;
//}





itemset_list_item_t* get_itemset_list_item_for_predicate(transaction_t base_transaction, transaction_t consequent_transcation, itemset_list_t* itemset_list) {
	itemset_list_item_t* itemset_list_item_to_return;
	transaction_t predicate_transaction = allocate_null_transaction();
	fill_transaction_with_difference(base_transaction, consequent_transcation, predicate_transaction);
	itemset_list_item_to_return = get_itemset_list_item(itemset_list, predicate_transaction);
	deallocate_transaction(predicate_transaction);
	return itemset_list_item_to_return;
}

rule_level_item_t* get_next_matching_rule_level_item_with_matching_consequent(rule_level_item_t* search_start_rule_list_item, transaction_t superset_transaction, int item_id_to_skip) {
	rule_level_item_t* current_rule_list_item = search_start_rule_list_item;

	while (current_rule_list_item != NULL) {
		if (is_k_minus_1_subset(current_rule_list_item->consequent_itemset->transaction, superset_transaction, item_id_to_skip)) {
			break;
		}

		current_rule_list_item = current_rule_list_item->next_item;
	}

	return current_rule_list_item;
}

int exists_all_k_minus_1_other_consequents(rule_level_item_t *start_rule, transaction_t superset_consequent_transaction, transaction_t item_ids_to_iteratively_exclude, int num_items_in_consequent) {
	int num_items_need_to_find, item_id_to_skip, num_matching_items_found;
	rule_level_item_t *current_rule = start_rule;

	num_items_need_to_find = num_items_in_consequent - 2;
	item_id_to_skip = NUM_ITEMS - 1;
	num_matching_items_found = 0;

	if (num_items_need_to_find == 0) {
		return 1;
	}

	while (1) {

		item_id_to_skip = get_previous_present_item_id(item_ids_to_iteratively_exclude, item_id_to_skip - 1);
		current_rule = get_next_matching_rule_level_item_with_matching_consequent(current_rule, superset_consequent_transaction, item_id_to_skip);

		if (current_rule == NULL) {
			break;
		} else {
			++num_matching_items_found;
		}
	}

	return num_items_need_to_find == num_matching_items_found;
}

void null_initialize_rule_level(rule_level_t* rule_level) {
	rule_level->first_item = NULL;
	rule_level->last_item = NULL;
	rule_level->num_rules = 0;
	rule_level->num_items_in_predicate = 0;
	rule_level->num_items_in_consequent = 0;
}

void fill_transaction_with_union_of_transactions(transaction_t lhs_transaction, transaction_t rhs_transaction, transaction_t result_transaction) {
	int item_id;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		result_transaction[item_id] = (lhs_transaction[item_id] || rhs_transaction[item_id]);
	}


}

void generate_association_rules_for_rule_level(itemset_list_item_t* current_itemset_list_item, itemset_rule_list_t* itemset_rule_list, int rule_level_index, int num_items_in_itemset) {
	int num_items_in_consequent, num_items_in_predicate;
	itemset_list_item_t *new_rule_predicate_itemset, *new_rule_consequent_itemset;
	rule_level_item_t* base_rule_from_prev_level, *to_match_rule_from_prev_level, *new_rule;
	rule_level_t *current_rule_level, *prev_rule_level;


	itemset_list_item_t *base_consequent_itemset, *to_match_consequent_itemset;

	transaction_t new_rule_consequent_transaction, item_ids_to_iteratively_exclude;

	item_ids_to_iteratively_exclude = allocate_null_transaction();

	num_items_in_consequent = rule_level_index;
	num_items_in_predicate = num_items_in_itemset - num_items_in_consequent;

	current_rule_level = &itemset_rule_list->rule_levels_array[rule_level_index];


	prev_rule_level = &itemset_rule_list->rule_levels_array[rule_level_index - 1];

	current_rule_level->num_items_in_predicate = num_items_in_predicate;
	current_rule_level->num_items_in_consequent = num_items_in_consequent;

	base_rule_from_prev_level = prev_rule_level->first_item;

	while(base_rule_from_prev_level != NULL) {

		to_match_rule_from_prev_level = base_rule_from_prev_level->next_item;

		while (to_match_rule_from_prev_level != NULL && consequent_itemset_prefix_ids_match(base_rule_from_prev_level, to_match_rule_from_prev_level)) {

			base_consequent_itemset = base_rule_from_prev_level->consequent_itemset;
			to_match_consequent_itemset = to_match_rule_from_prev_level->consequent_itemset;

			new_rule_consequent_transaction = allocate_null_transaction();

			fill_transaction_with_union_of_transactions(base_consequent_itemset->transaction, to_match_consequent_itemset->transaction, new_rule_consequent_transaction);

			new_rule_consequent_itemset = get_itemset_list_item(&all_frequent_itemset_lists_set.level_lists[num_items_in_consequent], new_rule_consequent_transaction);
			new_rule_predicate_itemset = get_itemset_list_item_for_predicate(current_itemset_list_item->transaction, new_rule_consequent_transaction, &all_frequent_itemset_lists_set.level_lists[num_items_in_predicate]);


			new_rule = allocate_new_rule(new_rule_predicate_itemset, new_rule_consequent_itemset);

			fill_transaction_with_intersection(base_consequent_itemset->transaction, to_match_consequent_itemset->transaction, item_ids_to_iteratively_exclude);


			if (exists_all_k_minus_1_other_consequents(to_match_rule_from_prev_level->next_item, new_rule_consequent_itemset->transaction, item_ids_to_iteratively_exclude, num_items_in_consequent)) {
				fill_concurrent_trans_ids_for_rule(new_rule);

				if (get_confidence(new_rule) > MIN_CONFIDENCE) {
					add_rule_to_rule_level(new_rule, current_rule_level);
				} else {
					deallocate_rule_list_item(new_rule);
				}

			} else {
				deallocate_rule_list_item(new_rule);
			}

			to_match_rule_from_prev_level = to_match_rule_from_prev_level->next_item;

		}

		base_rule_from_prev_level = base_rule_from_prev_level->next_item;
	}

	deallocate_transaction(item_ids_to_iteratively_exclude);

}




void generate_association_rules_for_first_array_level(itemset_list_item_t* itemset_list_item, rule_level_t* rule_level, int num_items_in_itemset) {
	int consequent_item_id, ith_item_index, num_items_in_consequent, num_items_in_predicate;
	itemset_list_item_t *predicate_itemset_list_item, *consequent_itemset_list_item;
	transaction_t predicate_itemset_transaction, consequent_itemset_transaction, base_itemset_transaction;
	rule_level_item_t* new_rule;



	num_items_in_consequent = 1;
	num_items_in_predicate = num_items_in_itemset - num_items_in_consequent;

	predicate_itemset_transaction = allocate_null_transaction();
	consequent_itemset_transaction = allocate_null_transaction();

	base_itemset_transaction = itemset_list_item->transaction;
	memcpy_transaction(predicate_itemset_transaction, base_itemset_transaction);

	consequent_item_id = get_next_present_item_id(itemset_list_item->transaction, 0);

	predicate_itemset_transaction[consequent_item_id] = 0;
	consequent_itemset_transaction[consequent_item_id] = 1;

	rule_level->num_items_in_consequent = 1;
	rule_level->num_items_in_predicate = num_items_in_itemset - 1;

	for (ith_item_index = num_items_in_itemset; ith_item_index > 0; --ith_item_index) {

		predicate_itemset_list_item = get_itemset_list_item(&all_frequent_itemset_lists_set.level_lists[num_items_in_predicate], predicate_itemset_transaction);
		consequent_itemset_list_item = get_itemset_list_item(&all_frequent_itemset_lists_set.level_lists[num_items_in_consequent], consequent_itemset_transaction);

		new_rule = allocate_new_rule(predicate_itemset_list_item, consequent_itemset_list_item);

		fill_concurrent_trans_ids_for_rule(new_rule);

		if (get_confidence(new_rule) > MIN_CONFIDENCE) {
			add_rule_to_rule_level(new_rule, rule_level);
		} else {
			deallocate_rule_list_item(new_rule);
		}

		predicate_itemset_transaction[consequent_item_id] = 1;
		consequent_itemset_transaction[consequent_item_id] = 0;

		consequent_item_id = get_next_present_item_id(itemset_list_item->transaction, consequent_item_id + 1);

		predicate_itemset_transaction[consequent_item_id] = 0;
		consequent_itemset_transaction[consequent_item_id] = 1;
	}

	deallocate_transaction(predicate_itemset_transaction);
	deallocate_transaction(consequent_itemset_transaction);

}

rule_level_t* allocate_itemset_rule_levels_array(int num_rule_levels) {
	return malloc(sizeof(rule_level_t) * num_rule_levels);
}

void null_initialize_all_rule_levels(itemset_rule_list_t* rule_list) {
	int rule_level_index;

	for (rule_level_index = 0; rule_level_index < rule_list->total_rule_levels; ++rule_level_index) {
		null_initialize_rule_level(&rule_list->rule_levels_array[rule_level_index]);
	}

}

void generate_association_rules_from_itemset(itemset_list_item_t* itemset_list_item, itemset_rule_list_t* itemset_rule_list, int num_items_in_itemset) {
	int rule_level_index;

	itemset_rule_list->rule_levels_array = allocate_itemset_rule_levels_array(num_items_in_itemset);
	itemset_rule_list->total_rule_levels = num_items_in_itemset;

	null_initialize_all_rule_levels(itemset_rule_list);

//	printf("Itemset to Generate Rules: \n");
//	print_itemset_list_item(itemset_list_item);

	generate_association_rules_for_first_array_level(itemset_list_item, &itemset_rule_list->rule_levels_array[1], num_items_in_itemset);

	for (rule_level_index = 2; rule_level_index < num_items_in_itemset; ++rule_level_index) {
		generate_association_rules_for_rule_level(itemset_list_item, itemset_rule_list, rule_level_index, num_items_in_itemset);
	}


}



void generate_association_rules_from_equal_sized_itemsets(itemset_lists_set_t* itemset_lists_set, rule_lists_set_t* all_rule_lists_sets_array, int num_items_in_itemsets) {
	itemset_list_item_t* current_itemset_list_item;
	int frequent_itemset_index = 0;
	itemset_rule_list_t* current_itemset_rule_list;
	itemset_list_t current_itemset_list = itemset_lists_set->level_lists[num_items_in_itemsets];
	rule_lists_set_t* current_rule_lists_set = &all_rule_lists_sets_array[num_items_in_itemsets];

	all_rule_lists_sets_array[num_items_in_itemsets].itemset_rule_lists_array = allocate_rule_lists_array(current_itemset_list.num_elements);
	all_rule_lists_sets_array[num_items_in_itemsets].num_rule_lists = current_itemset_list.num_elements;

	current_itemset_list_item = current_itemset_list.first_item;

	while (current_itemset_list_item != NULL) {
		current_itemset_rule_list = &current_rule_lists_set->itemset_rule_lists_array[frequent_itemset_index];
		current_itemset_rule_list->base_itemset = current_itemset_list_item;

		generate_association_rules_from_itemset(current_itemset_list_item, current_itemset_rule_list, num_items_in_itemsets);

		current_itemset_list_item = current_itemset_list_item->next_item;
		++frequent_itemset_index;
	}


}



void print_rule_level(rule_level_t* rule_level) {
	rule_level_item_t* current_rule_level_item = rule_level->first_item;

//	printf("Rule Level: %d-item -> %d-item\n\n", rule_level->num_items_in_predicate, rule_level->num_items_in_consequent);

	while (current_rule_level_item != NULL) {
		print_rule(current_rule_level_item);
		current_rule_level_item = current_rule_level_item->next_item;
	}
}

void print_rules_for_rule_list(itemset_rule_list_t* rule_list) {

	int rule_level_index;

	for (rule_level_index = 1; rule_level_index < rule_list->total_rule_levels; ++rule_level_index) {
		print_rule_level(&rule_list->rule_levels_array[rule_level_index]);

	}

}

void print_rules_for_rule_lists_set(rule_lists_set_t* rule_lists_set) {
	int num_frequent_itemsets_for_rule_list_set = rule_lists_set->num_rule_lists;
	int rule_list_array_index;
	itemset_rule_list_t* rule_list;

	for (rule_list_array_index = 0; rule_list_array_index < num_frequent_itemsets_for_rule_list_set; ++rule_list_array_index) {
		rule_list = &rule_lists_set->itemset_rule_lists_array[rule_list_array_index];
		print_rules_for_rule_list(rule_list);
	}

}

int get_num_rules_for_rule_list(itemset_rule_list_t* rule_list) {
	int rule_level_index, rule_list_rule_count;
	rule_list_rule_count = 0;

	for (rule_level_index = 0; rule_level_index < rule_list->total_rule_levels; ++rule_level_index) {
		rule_list_rule_count += rule_list->rule_levels_array[rule_level_index].num_rules;
	}

	return rule_list_rule_count;
}

int get_num_rules_for_rule_list_set(rule_lists_set_t* rule_lists_set) {
	int rule_list_index, rule_list_set_rule_count;
	rule_list_set_rule_count = 0;

	for (rule_list_index = 0; rule_list_index < rule_lists_set->num_rule_lists; ++rule_list_index) {
		rule_list_set_rule_count += get_num_rules_for_rule_list(&rule_lists_set->itemset_rule_lists_array[rule_list_index]);
	}

	return rule_list_set_rule_count;
}


int get_total_num_rules(rule_lists_set_t* rule_lists_sets_array) {
	int num_items_in_itemsets, total_rule_count;
	total_rule_count = 0;
	rule_lists_set_t* rule_lists_set_for_num_items;

	for (num_items_in_itemsets = 2; num_items_in_itemsets <= NUM_ITEMS; ++num_items_in_itemsets) {
		rule_lists_set_for_num_items = &rule_lists_sets_array[num_items_in_itemsets];
		if (rule_lists_set_for_num_items->num_rule_lists == 0) {
			continue;
		}
		total_rule_count += get_num_rules_for_rule_list_set(rule_lists_set_for_num_items);
	}

	return total_rule_count;
}



int get_brute_force_num_candidates(rule_lists_set_t* rule_lists_sets_array) {
	rule_lists_set_t* rule_lists_set_for_num_items;
	int total_num_candidates, num_items_in_itemsets, num_frequent_itemsets_in_rule_list_set;
	total_num_candidates = 0;

	for (num_items_in_itemsets = 2; num_items_in_itemsets <= NUM_ITEMS; ++num_items_in_itemsets) {
		rule_lists_set_for_num_items = &rule_lists_sets_array[num_items_in_itemsets];
		num_frequent_itemsets_in_rule_list_set = rule_lists_set_for_num_items->num_rule_lists;
		total_num_candidates += num_frequent_itemsets_in_rule_list_set * (pow(2, num_items_in_itemsets) - 2);
	}

	return total_num_candidates;

}

void print_all_rules(rule_lists_set_t* rule_lists_sets_array) {
	int num_items_in_itemsets;
	rule_lists_set_t* rule_lists_set_for_num_items;

	for (num_items_in_itemsets = 2; num_items_in_itemsets <= NUM_ITEMS; ++num_items_in_itemsets) {
		rule_lists_set_for_num_items = &rule_lists_sets_array[num_items_in_itemsets];

//		printf("Rules for %d-item itemsets: \n\n", num_items_in_itemsets);
		print_rules_for_rule_lists_set(rule_lists_set_for_num_items);
	}
	putchar('\n');

//	printf("Total Number of Rules: %20d\n", get_total_num_rules(rule_lists_sets_array));
//	printf("Total Confidence Candidates\n Generated Brute Force: %20d\n", get_brute_force_num_candidates(rule_lists_sets_array));

}

void print_rule_generation_report(rule_lists_set_t* rule_lists_sets_array, long elapsed_time_nanos) {
	printf("-------------------------------\n\nAssociation Rule Report:\n\n");
	printf("\nMin Confidence: %f\n", MIN_CONFIDENCE);
	printf("%-55s%5d\n", "Total Rule Candidates Generated Brute Force: ", get_brute_force_num_candidates(rule_lists_sets_array));
	printf("%-55s%5d\n", "Total Rule Candidates Generated: ", total_confidence_candidates_generated);
	printf("%-55s%5d\n", "Total Number of High-Confidence Rules: ", get_total_num_rules(rule_lists_sets_array));
	printf("\nTime To Generate Association Rules (sec): %f\n", (float)elapsed_time_nanos / 1000000000);
	printf("\n----------------------------------------------\n\n");
}



//void print_top_rules(int num_rules_to_print) {
//	itemset_rule_list_t* rule_list;
//
//
//}

void generate_association_rules(itemset_lists_set_t* itemset_lists_set) {
	if (apriori_analysis_type != STANDARD) {
		perror("You Cannot Generate Association Rules if you Used 'Standard' Method for Frequent Itemset Generation!!");
		exit(1);
	}


	srand(time(NULL));



	int num_items_in_itemsets;
	long elapsed_time_nanos;
	start_timer();

	all_rule_lists_sets_array = allocate_rule_lists_sets_array();

	for (num_items_in_itemsets = 2; num_items_in_itemsets <= MAX_RULE_SIZE; ++num_items_in_itemsets) {
//		printf("Start %d-item itemset rules: \n", num_items_in_itemsets);
		if (itemset_lists_set->level_lists[num_items_in_itemsets].num_elements == 0) {
			continue;
		}

		generate_association_rules_from_equal_sized_itemsets(itemset_lists_set, all_rule_lists_sets_array, num_items_in_itemsets);

	}

	elapsed_time_nanos = get_elapsed_time();

	if (print_association_rules) {
		printf("Association Rules: \n\n");
		probability_to_print_rule = (float)num_association_rules_to_print / get_total_num_rules(all_rule_lists_sets_array);
		print_all_rules(all_rule_lists_sets_array);
	}


	print_rule_generation_report(all_rule_lists_sets_array, elapsed_time_nanos);



}
