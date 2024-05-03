/*
*   James Hofer
*   1000199225
*   Compile: /usr/bin/c99 -o app2 app2.c
*
*   Usage1: ./app2 (for typing or pasting input)
*   Usage2: ./app2 < input.dat (for input redirection)
*
*   This program reads in two sequences of integers, A and X.
*   It then finds the largest interleaved sequence of X that
*   is contained in A (is a subsequence of A).
*
*   https://ranger.uta.edu/~weems/NOTES3318/LAB/LAB2FALL23/lab2fall23.pdf
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


/*
*   isSubSeq - check if interleaved sub-sequence X is in sequence A
*            - performs in linear time
*
*   A: sequence of integers
*   len_A: length of sequence, A
*   X: sub-sequence of integers
*   len_X: length of sequence, X
*   interleave: interleave factor of X
*
*   return: true if X is a sub-sequence of A, false otherwise
*/
bool isSubSeq(int *A, int len_A, int *X, int len_X, int interleave)
{
    bool match = false;
    int j = 0;
    int k = interleave;

    for(int i = 0; i < len_A; i++)
    {
        if(A[i] == X[j])
        {
            // k > 1 dont advance X[j] (interleave)
            // k == 1 advance X[j] and reset k
            if(k == 1)
            {
                j++;
                k = interleave;
            }
            else
                k--;
            
            // len_X + 1 before end of A then match
            if(j == len_X)
            {
                match = true;
                break;
            }
        }
    }

    return match;
}


/*
*   Binary search for highest interleave factor
*   prints results to stdout
*   modified from Dr. Weems' code
*
*   A: sequence of integers
*   len_A: length of sequence, A
*   X: sub-sequence of integers
*   len_X: length of sequence, X
*/
int binSearch(int *A, int len_A, int *X, int len_X)
{
    int low, mid, high;     // interleave range
    low = 0;                // min possible interleave
    high = len_A / len_X;   // max possible interleave
    int repeats = 0;        // largest interleave that passed
    bool match;

    while (low <= high)
    {
        // find middle search point
        mid = (low+high) / 2;

        // check if mid is a valid interleave
        match = isSubSeq(A, len_A, X, len_X, mid);
        printf("low %d mid %d high %d %s\n", low, mid, high, match ? "passed" : "failed");

        // update bounds
        if(match)
        {
            low = mid + 1;
            repeats = mid;
        }
        else
            high = mid - 1;
    }

    printf("Maximum repeats is %d\n", repeats);
}


/*
*   Main function
*   reads in sequences, A and X
*   calls binSearch() for highest interleave factor
*/
int main()
{
    // check if redirected input
    if(isatty(0))
        printf("Type input by line or Paste entire input.\n");
    
    // read in lengths of sequences, A and X
    int len_A, len_X;
    scanf("%d %d", &len_A, &len_X);

    // create arrays for sequences on the heap
    int *A = (int*) malloc((len_A) * sizeof(int));
    int *X = (int*) malloc((len_X) * sizeof(int));
    if(!A || !X)
    {
        printf("Malloc Error %d\n",__LINE__);
        exit(0);
    }

    // read in A and X values
    for(int i = 0; i < len_A; i++)
        scanf("%d", &A[i]);
    scanf("%*d");   // skip -9999

    for(int i = 0; i < len_X; i++)
        scanf("%d", &X[i]);
    scanf("%*d");   // skip -9999
    
    // binary search for highest interleave factor
    binSearch(A, len_A, X, len_X);

    // clean up and exit
    free(A);
    free(X);
    return 0;
}