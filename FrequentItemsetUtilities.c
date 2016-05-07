

#include "FrequentItemsetUtilities.h"
#include "ReadData.h"

transaction_t allocate_new_transaction() {
	return malloc(sizeof(int)*NUM_ITEMS);
}

transaction_t allocate_null_transaction() {
	transaction_t new_trans = allocate_new_transaction();
	memset(new_trans, 0, NUM_ITEMS * sizeof(int));
	return new_trans;
}

void print_transaction(transaction_t transaction) {
	int item_id;

	for (item_id = 0; item_id < NUM_ITEMS; ++item_id) {
		printf("%4d ", transaction[item_id]);
	}

}

void print_all_transactions(transaction_t* transactions) {
	int trans_id;

	for (trans_id = 0; trans_id < NUM_TRANS; ++trans_id) {

		print_transaction(transactions[trans_id]);
		putchar('\n');
	}

	putchar('\n');
	putchar('\n');
}

void deallocate_item(void* item) {
	printf("Item deallocated\n");
	free(item);
}


