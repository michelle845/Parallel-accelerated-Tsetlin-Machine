#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TsetlinMachine.h"
#include "NoisyXORTrainingData.h"
#include "Ta.h"






void clauseoutput(tm_dat_t literals[NUMBER_OF_EXAMPLES][noOfLiterals],tm_dat_t clausesoutput[NUMBER_OF_EXAMPLES][CLAUSES]){


	const mat_b_t ta_action_data[CLAUSES][noOfLiterals]={
			#include "TA_action.txt"
			};

//#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=literals
//#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=ta_action_data
//#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=clausesoutput

	int clause=1; //IF INCLUDE 1, CLAUSE OUTPUT = 1
COL_CLAUSEOUTPUT_LOOP:for(int i = 0; i < NUMBER_OF_EXAMPLES; i++) {

ROW_CLAUSEOUTPUT_LOOP:	for(int j = 0; j < CLAUSES; j++) {
//#pragma HLS PIPELINE II=2
		         // Iterate over the columns of the B matrix
		    	  clausesoutput[i][j] = 0;
		         int x=0;
		         //
		         // Do the inner product of a row of A and col of B

ROW_CALCULATION_LOOP:	for(int k = 0; k < noOfLiterals; k++) {
//#pragma HLS UNROLL factor=2

		        	 clause=1;

		        	 if (ta_action_data[j][k] == 1 && literals[i][k] == 0) { //IF INCLUDE 0, CLAUSE OUTPUT=0
		        	        	  clause = 0;
		        	              break;
		        	 }
		        	          else if (ta_action_data[j][k] == 0) { //IF EXCLUDE, THEN SKIP
		        	              x++;
		        	          }

		        	          if (x==4){
		        	        	  clause = 0;
		        	          }
		         }
		         clausesoutput[i][j] = clause;
		         printf("%d",clausesoutput[i][j]);
		      }
		      printf("\n");
		 }

}



void read_NoisyXOR_dat(tm_dat_t NoisyXOR_dat[60000],short buf_literals[NUMBER_OF_EXAMPLES][noOfLiterals]){

	int i, j, k=0;

	tm_dat_t literals[NUMBER_OF_EXAMPLES][noOfLiterals];
	tm_dat_t NoisyXOR_2D[NUMBER_OF_EXAMPLES][FEATURES];

	//short buf_dat[NUMBER_OF_EXAMPLES][FEATURES];
	COL_EXAMPLE_LOOP:
	for(int i=0; i<NUMBER_OF_EXAMPLES;i++){
		ROW_EXAMPLE_LOOP:
		for(int j=0;j<FEATURES;j++){

			NoisyXOR_2D[i][j]=NoisyXOR_dat[i*FEATURES+j];

			//printf("%d",NoisyXOR_2D[i][j]);
		}
		//printf("\n");
	}


	//generate literals
	ROW_LITERALS_LOOP:
	for (i = 0; i < NUMBER_OF_EXAMPLES; i++) {
		COL_LITERALS_LOOP:
			for (j = 0; j < noOfLiterals; j++) {

				if ((j % 2) == 0) {

					literals[i][j] = NoisyXOR_2D[i][k];

				}
				else
				{
					if (NoisyXOR_2D[i][k] == 0) {
						literals[i][j] = 1;
						if (k < 11) {
							k++;
						}
						else {
							k = 0;
						}
					}

				else if (NoisyXOR_2D[i][k] == 1)
				{
						literals[i][j] = 0;

						if (k < 11) {
							k++;
						}
						else {
							k = 0;
						}
				}
			}
				buf_literals[i][j]=literals[i][j];
				//printf("%d ", buf_literals[i][j]);
		}
			//printf("\n");
}


}

void tm(tm_dat_t NoisyXOR_dat[60000]){
	short buf_literals[NUMBER_OF_EXAMPLES][noOfLiterals];
	short buf_dat_out[NUMBER_OF_EXAMPLES][FEATURES];

	//Read input data.Fill the internal buffer
	read_NoisyXOR_dat(NoisyXOR_dat,buf_literals);
	clauseoutput(buf_literals,buf_dat_out);


}





