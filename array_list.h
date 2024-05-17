#define CAPACITY 800

typedef struct arraylist{
    int array[CAPACITY];
    int size;
    int capacity;
} ArrayList;

ArrayList createArrayList();
void copyArrayList(ArrayList arrFrom,ArrayList *arrTo);

void append(ArrayList *arr, int value);
int get(ArrayList arr, int index);
void set(ArrayList *arr, int index, int value);
void insert(ArrayList *arr, int index, int value);
void deleteByPos(ArrayList *arr, int index);
int contains(ArrayList arr, int value);
int deleteByValue(ArrayList *arr, int value);
void printArrayList(ArrayList arr);