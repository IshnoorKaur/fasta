# CIS2520-F23-A2 

## Student Information

Name: Ishnoor Kaur 

Student Number: 1226429 

## Resources 
- Open Data Structures (in pseudocode) by Pat Morin
    This helped me review basic concepts from past courses. After the refresh, I was able to understand the code better. 

## Implementation
The assignment is complete. 

## Summary 
The relative amounts of time required to perform the tasks in these programs vary based on the data structure used for storing FASTA records.

- arraydouble_main.c reads FASTA records from a file, stores them in a dynamically growing array, and measures the time taken for processing. The code demonstrates how to allocate memory dynamically for records and how the processing time varies with the number of records. The time complexity of array-based storage is O(n) for each insertion operation, leading to slower performance as the number of records grows.

- llheadonly_main.c implements a similar FASTA record processing task, but it uses a linked list data structure with a head pointer. Linked lists provide faster insertion times than arrays, as records can be easily added at the beginning of the list. However, the time taken still increases with more records, but the growth rate is typically slower than in array-based storage. The time complexity of insertion is O(1).

- llheadtail_main.c is also similar but uses a linked list with both head and tail pointers for efficient insertion at the end. The code shows how the choice of data structure can impact the performance of record storage and processing tasks. This leads to improved performance compared to arrays and standard linked lists. The time complexity of insertion is O(1).

In summary, the choice of data structure significantly impacts the relative amount of time required for storing and processing FASTA records. Arrays tend to become slower as records increase, while linked lists, especially those with tail pointers, offer more efficient and consistent performance, especially as the number of records grows.
