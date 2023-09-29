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
		for (i=j=size/2,k=0;k<size;++k) {
			if (input[i-1]>input[j]) output[k]=input[i--];
			else output[k]=input[j++]; 
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
		output=input;
		return;
	} else {
		int mid = size/2;
		mergesort(input, mid, &output[mid], true); // left side asc
		mergesort(&input[mid], mid, output, false); // right side desc
	}
	merge(input, size, output, output_asc);
}

/* mergesort(input, size)
   Sorts size elements in the array pointed to by input, using the MergeSort
   algorithm. Output is returned as a newly allocated array, which the caller
   is responsible for freeing.
   */
/*int* mergesort(int* input, int size) {
	int* output = new int[size];
	mergesort(input, size, output, true);
	return output;
}*/
