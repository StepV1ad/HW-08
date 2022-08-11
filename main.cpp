#include <future>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

bool make_thread = true;

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    auto* left = new int[nl];
    auto* right = new int[nr];
    //int left[nl], right[nr];

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }
    delete[] left;
    delete[] right;
}

void mergeSort(int* arr, int l, int r)
{
    if (l >= r)
        return;
    int m = (l + r - 1) / 2;

    if (make_thread && (r - l > 5000))
    {
        auto f1 = async(launch::async, [&]() {mergeSort(arr, l, m);});
        //mergeSort(arr, l, m);
        auto f2 = async(launch::async, [&]() {mergeSort(arr, m + 1, r); });
        //mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
    else 
    {
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main()
{
    srand(0);
    long arr_size = 2000000;
    int* array = new int[arr_size];
    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500;
    }

    time_t start, end;


    // многопоточный запуск
    time(&start);
    mergeSort(array, 0, arr_size - 1);
    time(&end);

    double seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    for (long i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            cout << "Unsorted" << endl;
            break;
        }
    }

    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500;
    }
    // однопоточный запуск
    make_thread = false;
    time(&start);
    mergeSort(array, 0, arr_size - 1);
    time(&end);
    seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);
    delete[] array;
    return 0;
}