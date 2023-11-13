#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int left, int right) {
    int temp = arr[left];
    int parent = left;

    while (parent < (right+1)/2) {
        int cl = parent*2 + 1;
        int cr = cl + 1;
        int child;
        if ((cr <= right)&&(arr[cr]>arr[cl])) 
            child = cr;
        else
            child = cl;
        if (temp >= arr[child])
            break;
        arr[parent] =arr[child];
        parent =child;
    }
    arr[parent] = temp;
}

void heap_sort(int arr[], int n) {
    int i;
    for (i = (n-1)/2; i >= 0; i--) {
        heapify(arr, i, n-1);
    }
    for (i = n-1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, 0, i-1);
    }
}


int main() {
    int arr[] = {9, 4, 8, 2, 5, 1};
    int n = sizeof(arr)/sizeof(int);
    
    printf("Before sorting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    heap_sort(arr, n);
    
    printf("After sorting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}