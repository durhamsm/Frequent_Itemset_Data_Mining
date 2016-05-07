
#ifndef READ_DATA_H_
#define READ_DATA_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FrequentItemsetUtilities.h"








void load_data(dataset_params_t dataset_params);


extern dataset_t dataset;
extern int NUM_ITEMS, NUM_TRANS, MAX_RULE_SIZE;





#endif
