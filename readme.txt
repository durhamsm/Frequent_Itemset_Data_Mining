To Compile:

make clean
make

To Run:

./frequent_itemset


-----------------

Changing the Data set to Analyze and Other parameters:

Please see the comments in "FrequentItemSet.c" file for instructions on how to analyze various data sets and also alter the support/confidence thresholds. All of these parameters are located above the "main" function in that file.

-----------------

Here is a list of the primary files where the code for the various parts of question 3 lives:

ReadData.c: this file contains the code for reading data from the csv files.

FrequentItemSet.c: this is the file that contains "main" function. also contains the variables that you will alter to run various data sets, support and confidence levels.

FrequentItemSetUtilities.c: utilities functions

Apriori.c: this file contains the code for generating the frequent itemsets.

AprioriUtilities.c: this file contains utilities and other functions that are necessary for the frequent itemsets to be generated, but were deemed less important in terms of demonstrating knowledge of the material.

MaximalClosed.c: this file contains the code for determining whether each of the frequent item sets are closed/maximal.

Confidence.c: this file contains the code for generating the association rules. I apologize, there are a lot of functions in this file. I was going to break into smaller files but don't have time.

data/ directory: contains the csv files, along with an excel file that was used to transform the categorical/numerical data in binary columns.

Makefile: for making/cleaning program.


---------------

General Comments About program:

I decided to implement in C because I thought I could make the program more efficient; however, I think it would have been better to use C++/C with the ability to use List objects, as opposed to the linked lists that I implemented for this assignment. As stated in the writeup, I encounted seg fault errors when the minimum support was very low and/or when a low confidence was used for generating the association rules. Also, for the last two data sets, I had to limit the size of the association rules - possible explanations for this are briefly discussed in the writeup.
