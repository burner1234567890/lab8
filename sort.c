#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated = 0;

void heapify(int arr[], int l, int r, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left + l <= r && arr[left + l] > arr[largest + l]) {
		largest = left;
	}

	if (right + l <= r && arr[right + l] > arr[largest + l]) {
		largest = right;
	}

	if (largest == i) {
		return;
	}

	int temp = arr[i + l];
	arr[i + l] = arr[largest + l];
	arr[largest + l] = temp;
	heapify(arr, l, r, largest);
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int l, int r)
{
	for (int i = ((r - l + 1) / 2) - 1; i >= 0; --i) {
		heapify(arr, l, r, i);
	}

	for (int i = r; i > l; --i) {
		int temp = arr[l];
		arr[l] = arr[i];
		arr[i] = temp;
		heapify(arr, l, i - 1, 0);
	}
}

void merge(int pData[], int l, int m, int r) {
	int i = 0, j = 0, k = l;
	int size1 = m - l + 1;
	int size2 = r - m;

	int bytes1 = size1 * sizeof(int);
	int bytes2 = size2 * sizeof(int);

	int *arr1 = (int*)malloc(bytes1);
	extraMemoryAllocated += bytes1;
	int *arr2 = (int*)malloc(bytes2);
	extraMemoryAllocated += bytes2;

	for (int i = 0; i < size1; ++i) {
		arr1[i] = pData[l + i];
	}
	for (int j = 0; j < size2; ++j) {
		arr2[j] = pData[m + 1 + j];
	}

	for(; i < size1 && j < size2; ++k) {
		if (arr1[i] <= arr2[j]) {
			pData[k] = arr1[i];
			++i;
		}
		else {
			pData[k] = arr2[j];
			++j;
		}
	}
	

	for (; i < size1; ++i, ++k) {
		pData[k] = arr1[i];
	}

	for (; j < size2; ++j, ++k) {
		pData[k] = arr2[j];
	}

	free(arr1);
	free(arr2);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
		int m = l + (r - l) / 2;

		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);

		merge(pData, l, m, r);
	}

}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
	return;
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	return 0;
}
