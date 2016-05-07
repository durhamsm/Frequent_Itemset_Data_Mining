
#include "Apriori.h"
#include "ReadData.h"
#include "FrequentItemSet.h"
#include "MaximalClosed.h"
#include "Confidence.h"
#include <math.h>



//*******Program Parameters******************

//The following 3 sets of pairs of lines should be uncommented for the dataset that you would
//like to run analysis on. The 'column_headers' array of strings is used for printing the association rules.

//bike sharing data set
//dataset_params_t dataset_params = {17379, 16, 16, "./data/bike_sharing_data_transformed.csv"};
//char* column_headers[] = {"Spring", "Summer", "Fall", "Winter", "Day", "Night", "Holiday", "Working Day", "Clear", "Misty or Cloudy", "Light Precip", "Heavy Precip", "Low Temp", "High Temp", "Low Total Count", "High Total Count"};

//adult data set
dataset_params_t dataset_params = {32561, 112, 4, "./data/adult_data_transformed.csv"};
char* column_headers[] = {"Age: <= 20"	,"Age: 21 - 40"	,"Age: 41 - 60"	,"Age: 61 - 80"	,"Age: 81 - 95"	,"Private"	,"Self-emp-not-inc"	,"Self-emp-inc"	,"Federal-gov"	,"Local-gov"	,"State-gov"	,"Without-pay"	,"Never-worked"	,"Bachelors"	,"Some-college"	,"11th"	,"HS-grad"	,"Prof-school"	,"Assoc-acdm"	,"Assoc-voc"	,"9th"	,"7th-8th"	,"12th"	,"Masters"	,"1st-4th"	,"10th"	,"Doctorate"	,"5th-6th"	,"Preschool"	,"Married-civ-spouse"	,"Divorced"	,"Never-married"	,"Separated"	,"Widowed"	,"Married-spouse-absent"	,"Married-AF-spouse"	,"Tech-support"	,"Craft-repair"	,"Other-service"	,"Sales"	,"Exec-managerial"	,"Prof-specialty"	,"Handlers-cleaners"	,"Machine-op-inspct"	,"Adm-clerical"	,"Farming-fishing"	,"Transport-moving"	,"Priv-house-serv"	,"Protective-serv"	,"Armed-Forces"	,"Wife"	,"Own-child"	,"Husband"	,"Not-in-family"	,"Other-relative"	,"Unmarried"	,"White"	,"Asian-Pac-Islander"	,"Amer-Indian-Eskimo"	,"Other"	,"Black"	,"Male"	,"Female"	,"Low Capital Gain"	,"Low Capital Loss"	,"Low Hrs Per Week"	,"High Capital Gain"	,"High Capital Loss"	,"High Hrs Per Week"	,"United-States","Cambodia"	,"England"	,"Puerto-Rico"	,"Canada"	,"Germany"	,"Outlying-US(Guam-USVI-etc)"	,"India"	,"Japan"	,"Greece"	,"South"	,"China"	,"Cuba"	,"Iran"	,"Honduras"	,"Philippines"	,"Italy"	,"Poland"	,"Jamaica"	,"Vietnam"	,"Mexico"	,"Portugal"	,"Ireland"	,"France"	,"Dominican-Republic"	,"Laos"	,"Ecuador"	,"Taiwan"	,"Haiti", "Columbia"	,"Hungary"	,"Guatemala"	,"Nicaragua"	,"Scotland"	,"Thailand"	,"Yugoslavia"	,"El-Salvador"	,"Trinadad&Tobago"	,"Peru","Hong","Holand-Netherlands"	,"<=50K"	,">50K"};

//extended bakery 75k items data set
//dataset_params_t dataset_params = {75000, 50, 50, "./data/75000-out2.csv"};
//char* column_headers[] = {"Item 1",	"Item 2",	"Item 3",	"Item 4",	"Item 5",	"Item 6",	"Item 7",	"Item 8",	"Item 9",	"Item 10",	"Item 11",	"Item 12",	"Item 13",	"Item 14",	"Item 15",	"Item 16",	"Item 17",	"Item 18",	"Item 19",	"Item 20",	"Item 21",	"Item 22",	"Item 23",	"Item 24",	"Item 25",	"Item 26",	"Item 27",	"Item 28",	"Item 29",	"Item 30",	"Item 31",	"Item 32",	"Item 33",	"Item 34",	"Item 35",	"Item 36",	"Item 37",	"Item 38",	"Item 39",	"Item 40",	"Item 41",	"Item 42",	"Item 43",	"Item 44",	"Item 45",	"Item 46",	"Item 47",	"Item 48",	"Item 49",	"Item 50"};

//extended bakery 1M items data set
//dataset_params_t dataset_params = {1045555, 50, 50, "./data/75000-out2.csv"};
//char* column_headers[] = {"Item 1",	"Item 2",	"Item 3",	"Item 4",	"Item 5",	"Item 6",	"Item 7",	"Item 8",	"Item 9",	"Item 10",	"Item 11",	"Item 12",	"Item 13",	"Item 14",	"Item 15",	"Item 16",	"Item 17",	"Item 18",	"Item 19",	"Item 20",	"Item 21",	"Item 22",	"Item 23",	"Item 24",	"Item 25",	"Item 26",	"Item 27",	"Item 28",	"Item 29",	"Item 30",	"Item 31",	"Item 32",	"Item 33",	"Item 34",	"Item 35",	"Item 36",	"Item 37",	"Item 38",	"Item 39",	"Item 40",	"Item 41",	"Item 42",	"Item 43",	"Item 44",	"Item 45",	"Item 46",	"Item 47",	"Item 48",	"Item 49",	"Item 50"};


//plants data set
//dataset_params_t dataset_params = {34781, 61, 4, "./data/plants_transformed.csv"};
//char* column_headers[] = {"co"	,"ct"	,"de"	,"dc"	,"fl"	,"ga"	,"hi"	,"id"	,"il"	,"in"	,"ia"	,"ks"	,"ky"	,"la"	,"me"	,"md"	,"ma"	,"mi"	,"mn"	,"ms"	,"mo"	,"mt"	,"ne"	,"nv"	,"nh"	,"nj"	,"nm"	,"ny"	,"nc"	,"nd"	,"oh"	,"ok"	,"or"	,"pa"	,"pr"	,"ri"	,"sc"	,"sd"	,"tn"	,"tx"	,"ut"	,"vt"	,"va"	,"vi"	,"wa"	,"wv"	,"wi"	,"wy"	,"al"	,"bc"	,"mb"	,"nb"	,"lb"	,"nf"	,"nt"	,"ns"	,"nu"	,"on"	,"qc"	,"sk"	,"yt"};


//Set minimum support and confidence levels.
int MIN_SUP = 500;
float MIN_CONFIDENCE = 0.7;

//The 'STANDARD' method is F(k-1) X F(k-1) method. Set variable to 'F1' to run F(k-1) X F(1) method.
enum APRIORI_ANALYSIS_TYPE apriori_analysis_type = STANDARD;

//Set to 1 to run maximal-closed analysis. Otherwise set to 0.
int run_maximal_closed_analysis = 1;

//This will cause all frequent itemsets to be printed
int print_frequent_itemsets = 0;


int print_association_rules = 1;

//If you would only like to see a randomly selected number of association rules, then
// 'print_specified_num_of_association_rules' should be set to 1, and 'num_association_rules_to_print'
//should be set to the number of rules that you want to be printed. Otherwise, if you want to see all
//rules printed, then set 'print_specified_num_of_association_rules' to 0, and make sure 'print_association_rules' (above)
//is set to 1. If you would like to see the top association rules for a given level of support, then you need to increase the
//MIN_CONFIDENCE until the low-confidence rules are "weeded out."
int print_specified_num_of_association_rules = 1;
int num_association_rules_to_print = 10;

//If you want to only see rules that have lift above a certain value, then set 'print_high_lift_rules' to 1, and
//then set 'high_lift_threshold' to desired threshold. I used this threshold to print the rules with the top lift scores,
//by setting the confidence level to a low value (0.5), and then gradually increasing the lift_threshold until only the top-10 rules were printed.
int print_high_lift_rules = 1;
float high_lift_threshold = 0;


//*************************


int main(void) {

	load_data(dataset_params);

	apriori_generate_frequent_itemsets(apriori_analysis_type);

	//Maximal Closed Analysis
//	if (run_maximal_closed_analysis) {
//		assign_maximal_and_closed_status(&all_frequent_itemset_lists_set);
//	}

	//Association Rule Generation
//	generate_association_rules(&all_frequent_itemset_lists_set);


	return EXIT_SUCCESS;
}


//Notes for developer

//*************

//"deallocate_transaction"in AprioriUtilities.c is currently turned off, as it is causing problems.
