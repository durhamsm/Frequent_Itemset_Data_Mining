

#include "ReadData.h"
#include "Utilities.h"
#include "FrequentItemsetUtilities.h"

#define ARRAYSIZE(x)  (sizeof(x)/sizeof(*(x)))

dataset_t dataset;
int NUM_ITEMS, NUM_TRANS, MAX_RULE_SIZE;

const char* getfield(char* line, int num) {

    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")) {
        if (!--num)
            return tok;
    }
    return NULL;

}


void initialize_dataset(dataset_params_t dataset_params) {
	dataset.params = dataset_params;
	NUM_ITEMS = dataset_params.num_items;
	NUM_TRANS = dataset_params.num_trans;
	MAX_RULE_SIZE = dataset_params.max_rule_size;
	dataset.transactions = allocate_int_matrix(dataset_params.num_trans, dataset_params.num_items);
}

void read_data(dataset_params_t dataset_params) {

	initialize_dataset(dataset_params);
//	const char filename[] = "file.csv";
	   /*
		* Open the file.
		*/
	FILE *file = fopen(dataset_params.file_path, "r");
	if ( file ) {

		size_t i, j, k;
		char buffer[BUFSIZ], *ptr;
		/*
		* Read each line from the file.
		*/
		for ( i = 0; fgets(buffer, sizeof buffer, file); ++i ) {
		 /*
		  * Parse the comma-separated values from each line into 'array'.
		  */

			ptr = buffer;
			(int)strtol(ptr, &ptr, 10);
			++ptr;
			for ( j = 0; j < dataset_params.num_items; ++j, ++ptr ) {
				dataset.transactions[i][j] = (int)strtol(ptr, &ptr, 10);
			}
		}
		fclose(file);
		/*
		* Print the data in 'array'.
		*/
//		for ( j = 0; j < i; ++j ) {
//			for ( k = 0; k < dataset_params.num_items; ++k ) {
//				printf("%4d ", dataset.transactions[j][k]);
//			}
//			putchar('\n');
//		}
	}
	else /* fopen() returned NULL */ {
		perror(dataset_params.file_path);
	}


}





void load_data(dataset_params_t dataset_params) {

	read_data(dataset_params);
//	print_all_transactions(dataset.transactions);


}
