

#include "Utilities.h"

int** allocate_int_matrix(int num_rows, int num_cols) {

	int row_num;

	int** matrix = malloc((size_t)(sizeof(int*)*num_rows));

	for (row_num = 0; row_num < num_rows; ++row_num) {
		matrix[row_num] = malloc(sizeof(int)*num_cols);
	}

	return matrix;


}
