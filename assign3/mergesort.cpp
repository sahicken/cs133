/*
 * mergesort.cpp
 * Implementation of a bitonic mergesort
 */

/* merge(input, size, output, asc)
   Merge the two halves of the array input (which has size elements) into
   output. If asc is true, then the output array should be in ascending order;
   otherwise it should be descending.
   */
void merge(int* input, int size, int* output, bool output_asc) {
	int i; int j; int k;
	if (output_asc) {
		for (i=0,j=size-1,k=0;k<size;++k) {
			if (input[i]<input[j]) output[k]=input[i++];
			else output[k]=input[j--];
		}
	} else {
		for (i=0,j=size-1,k=size-1;k>=0;--k) {
			if (input[i]<input[j]) output[k]=input[i++];
			else output[k]=input[j--];
		}
	}
}

/* mergesort(input, size, output, asc)
   Mergesort the input array (with size elements) into the output array. If
   asc is true, the output array should be sorted ascending, otherwise it should
   be descending.
   */
void mergesort(int* input, int size, int* output, bool output_asc) {
	// Your mergesort implementation goes here
	// base case
	// recursive case
	// recursion parameter
	if (size==0) return; // base case 0
	else if (size==1) { // base case 1
		output[0]=input[0];
		return;
	} else {
		mergesort(input, size/2, output, true);
		mergesort(input+size/2, size-size/2, output+size/2, false);
		for(int i=0; i<size;++i) input[i]=output[i];
		merge(input, size, output, output_asc);
	}
}
