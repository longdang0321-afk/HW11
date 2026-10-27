#include <stdio.h>
#include <stdlib.h>

#define MAX_N 20000

int original[MAX_N];
int a[MAX_N]; /* insertion sort */
int b[MAX_N]; /* shell sort (n/2) */
int c[MAX_N]; /* shell sort (Knuth) */

/* 배열 src를 dst로 복사 */
void copy_array(int dst[], int src[], int n)
{
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

/* 정렬이 되었는지 확인 */
int is_sorted(int arr[], int n)
{
    for (int i = 1; i < n; i++)
        if (arr[i - 1] > arr[i])
            return 0;
    return 1;
}

/* 간격(gap) 삽입 정렬: gap = 1 이면 일반 삽입 정렬 */
long long gap_insertion_sort(int arr[], int n, int gap)
{
    long long comparisons = 0;

    for (int i = gap; i < n; i++)
    {
        int key = arr[i];
        int j = i - gap;

        while (j >= 0)
        {
            comparisons++; /* arr[j]와 key 비교 */
            if (arr[j] <= key)
                break;
            arr[j + gap] = arr[j]; /* 큰 값을 gap만큼 뒤로 이동 */
            j -= gap;
        }
        arr[j + gap] = key;
    }
    return comparisons;
}

long long insertion_sort(int arr[], int n)
{
    return gap_insertion_sort(arr, n, 1);
}

/* 원래 Shell 간격: n/2, n/4, ..., 1 */
long long shell_sort_original(int arr[], int n)
{
    long long comparisons = 0;

    for (int gap = n / 2; gap >= 1; gap /= 2)
        comparisons += gap_insertion_sort(arr, n, gap);
    return comparisons;
}

/* Knuth 간격: 1, 4, 13, 40, ... (3*gap + 1) */
long long shell_sort_knuth(int arr[], int n)
{
    long long comparisons = 0;
    int gap = 1;

    /* n보다 작은 가장 큰 Knuth 간격을 찾는다. */
    while (3 * gap + 1 < n)
        gap = 3 * gap + 1;

    /* 가장 큰 간격부터 역순으로 적용한다. */
    for (; gap >= 1; gap /= 3) /* 40 -> 13 -> 4 -> 1 */
        comparisons += gap_insertion_sort(arr, n, gap);
    return comparisons;
}

int main(void)
{
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 20000};
    int count = sizeof(sizes) / sizeof(sizes[0]);

    srand(2026); /* 같은 데이터를 재현하기 위한 시드 */

    for (int s = 0; s < count; s++)
    {
        int n = sizes[s];

        for (int i = 0; i < n; i++)
            original[i] = rand() % 100000;

        copy_array(a, original, n);
        copy_array(b, original, n);
        copy_array(c, original, n);

        long long x = insertion_sort(a, n);
        long long y = shell_sort_original(b, n);
        long long z = shell_sort_knuth(c, n);

        if (!is_sorted(a, n) || !is_sorted(b, n) || !is_sorted(c, n))
        {
            printf("Sorting failed for n = %d\n", n);
            return 1;
        }

        printf("Data Size : %d\n", n);
        printf("Insertion Sort       : %lld comparisons\n", x);
        printf("Shell Sort (Original): %lld comparisons\n", y);
        printf("Shell Sort (Knuth)   : %lld comparisons\n\n", z);
    }
    return 0;
}
