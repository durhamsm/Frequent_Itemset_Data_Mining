
#ifndef FREQUENT_ITEM_SET_H_
#define FREQUENT_ITEM_SET_H_

#include "Apriori.h"


extern int print_frequent_itemsets;
extern int MAX_ITEMSET_SIZE_FOR_RULES;
extern int print_association_rules;
extern int print_specified_num_of_association_rules;
extern int num_association_rules_to_print;
extern int print_high_lift_rules;
extern float high_lift_threshold;

extern enum APRIORI_ANALYSIS_TYPE apriori_analysis_type;

extern int MIN_SUP;
extern float MIN_CONFIDENCE;

extern char* column_headers[];


#endif
