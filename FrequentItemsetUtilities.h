/*
 * FrequentItemsetUtilities.h
 *
 *  Created on: Mar 30, 2016
 *      Author: durhamsm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FREQUENTITEMSETUTILITIES_H_
#define FREQUENTITEMSETUTILITIES_H_



typedef int* transaction_t;

typedef struct dataset_params_t {
	int num_trans;
	int num_items;
	int max_rule_size;
	char* file_path;
} dataset_params_t;

extern dataset_params_t dataset_params;

typedef struct dataset_t {
	transaction_t* transactions;
	dataset_params_t params;
} dataset_t;



transaction_t allocate_new_transaction();

transaction_t allocate_null_transaction();
void print_transaction(transaction_t transaction);

void print_all_transactions(transaction_t* transactions);

void deallocate_item(void* item);



#endif /* FREQUENTITEMSETUTILITIES_H_ */
