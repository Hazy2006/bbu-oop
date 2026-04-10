#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

/// <summary>
/// Function for generating prime factors
/// Starting from 2, run a loop until square root of n, then keep dividing n to i
/// </summary>
/// <param name="n">A given number</param>
void prime_factorization(int n)
{   
    int i;
    if (n < 2)
    {
        printf("No prime factors.\n");
        return;
    }
    printf("Prime factors decomposition of %d is: ", n);
    for (i = 2; i * i <= n; i++)
    {
        int count = 0;
        while (n % i == 0)
        {
            count++;
            n /= i;
        }
        if (count == 1)
            printf("%d ", i);
        else if (count > 1)
            printf("%d^%d ", i, count);
    }
    if (n > 1)
        printf("%d ", n);
    printf("\n");
}


/// <summary>
/// Marks which digits appear in a number by setting corresponding array indices to 1.
/// </summary>
/// <param name="n">The number whose digits to extract.</param>
/// <param name="arr">An array of size 10 where indices corresponding to digits present in the number will be set to 1.</param>
void get_digits(int n, int arr[10])
{
    if (n < 0)
        n = -n;
    while (n)
    {
        arr[n % 10] = 1;
        n /= 10;
    }
}
/// <summary>
/// Determines whether two integers contain the same digits with the property required.
/// </summary>
/// <param name="a">The first integer to compare.</param>
/// <param name="b">The second integer to compare.</param>
/// <returns>Non-zero (true) if both integers have the same digit composition, zero (false) otherwise.</returns>
int same_digits(int a, int b)
{
    int arr_a[10] = { 0 }, arr_b[10] = { 0 };
    get_digits(a, arr_a);
    get_digits(b, arr_b);
    bool ok = true;
    for (int i = 0; i < 10 && ok; i++)
        if (arr_a[i] != arr_b[i])
            ok = false;
    return ok;            
}


/// <summary>
/// Finds the longest consecutive sequence of numbers in an array that have the same digits.
/// </summary>
/// <param name="array">The array of integers to search.</param>
/// <param name="n">The number of elements in the array.</param>
/// <param name="start">Pointer to store the starting index of the longest sequence.</param>
/// <param name="length">Pointer to store the length of the longest sequence.</param>
void longest_same_digit(int array[], int n, int* start, int* length)
{
    int best_len, current_len, best_start, current_start;
    best_len = current_len = 1;
    best_start = current_start = 0;
    for (int i = 1; i < n; i++)
    {
        if (same_digits(array[i], array[i - 1]))
        {
            current_len++;
            if (current_len > best_len)
            {
                best_len = current_len;
                best_start = current_start;
            }
        }
        else
        {
            current_len = 1;
            current_start = i;
        }
       /// for debugging 
       /// printf("i=%d current_start=%d current_len=%d best_len=%d\n",
       ///     i, current_start, current_len, best_len);
    }
    *start = best_start;
    *length = best_len;
}


int main()
{
    int choice;
    int n = 0;
    int array[100];
    int vector_read = 0;

    do {
        printf("Choose one of the following options:\n");
        printf("1. Read vector\n");
        printf("2. Prime factorization\n");
        printf("3. Longest same digits\n");
        printf("4. Exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Read data:\n");
            scanf("%d", &n);
            if (n <= 0)
            {
                printf("Invalid size.\n");
                break;
            }
            for (int i = 0; i < n; i++)
                scanf("%d", &array[i]);
            vector_read = 1;
            break;
        case 2: 
        {
            int number;
            printf("Enter your number:\n");
            scanf("%d", &number);
            prime_factorization(number);
            break;
        }
        case 3: 
            if (!vector_read)
                printf("Please read an array from option (1)\n");
            else
            {
                int start, len;
                longest_same_digit(array, n, &start, &len);
                printf("Start: %d, len: %d\n", start, len);
            }
            break;
        case 4: 
            printf("Goodbye!\n"); 
            break;
        default: printf("Invalid option.\n");
        }
    } while (choice != 4);
    return 0;
}