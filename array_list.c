#include <stdio.h>
#include <stdlib.h>
#include "array_list.h"

ArrayList createArrayList() {
    ArrayList arr;

    arr.size = 0;        
    arr.capacity = CAPACITY;

    return arr;
}

void copyArrayList(ArrayList arrFrom,ArrayList *arrTo) {
    arrTo->size = arrFrom.size;
    arrTo->capacity = arrFrom.capacity;
    for (int i=0; i < arrFrom.size; i++)
        arrTo->array[i] = arrFrom.array[i];

}

void append(ArrayList *arr, int value) {
    arr->array[arr->size++] = value;
}

int get(ArrayList arr, int index) {
    if (index < 0 || index >= arr.size) {
        printf("Index out of bounds.\n");
        exit(1);
    }
    return arr.array[index];
}

void set(ArrayList *arr, int index, int value) {
    arr->array[index] = value;
}

void insert(ArrayList *arr, int index, int value) {
    for (int i = arr->size; i > index; i--) {
        arr->array[i] = arr->array[i - 1];
    }
    arr->array[index] = value;
    arr->size++;
}

void deleteByPos(ArrayList *arr, int index) {
    for (int i = index; i < arr->size - 1; i++) {
        arr->array[i] = arr->array[i + 1];
    }
    arr->size--;
}


int deleteByValue(ArrayList *arr, int value) {
    int i;
    for (i = 0; i < arr->size; i++) {
        if (arr->array[i] == value)
            break;
    }
    if (i == arr->size)  // error
        return 0;
    for ( ; i < arr->size - 1; i++) {
        arr->array[i] = arr->array[i + 1];
    }
    arr->size--;
    return 1;
}

int contains(ArrayList arr, int value) {
    for (int i = 0; i < arr.size; i++) {
        if (arr.array[i] == value)
            return 1;
    }
    return 0;
}

void printArrayList(ArrayList arr) {
    printf("Array: [");
    for (int i = 0; i < arr.size; i++) {
        if(i == arr.size-1){
            printf("%d", arr.array[i]);
        } else {
            printf("%d, ", arr.array[i]);
        }
    }
    printf("]\n");
}