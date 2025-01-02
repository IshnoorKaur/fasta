# FASTA Processing Performance

## Overview
This project explores the performance implications of using different data structures for processing and storing FASTA records. It highlights how the choice of data structure affects the time required for tasks like reading, storing, and processing records.

## Resources
- [Open Data Structures (in pseudocode) by Pat Morin](https://opendatastructures.org/)

## Summary
The relative performance of these programs varies depending on the data structure used:

### 1. `arraydouble_main.c`
- **Description**: Reads FASTA records from a file, stores them in a dynamically growing array, and measures processing time.
- **Key Characteristics**:
  - **Data Structure**: Dynamically growing array.
  - **Time Complexity**: O(n) for each insertion, where `n` is the number of records.
  - **Performance**: Slows down significantly as the number of records increases due to the need for resizing and copying.
- **Use Case**: Suitable for scenarios with a small, fixed number of records.

---

### 2. `llheadonly_main.c`
- **Description**: Processes FASTA records using a linked list with a single head pointer.
- **Key Characteristics**:
  - **Data Structure**: Singly linked list.
  - **Time Complexity**: O(1) for insertion at the beginning.
  - **Performance**: More efficient than arrays for insertion operations but still exhibits growth in processing time as the number of records increases.
- **Use Case**: Effective for applications requiring frequent insertions without specific order.

---

### 3. `llheadtail_main.c`
- **Description**: Utilizes a linked list with both head and tail pointers for efficient insertion at the end.
- **Key Characteristics**:
  - **Data Structure**: Doubly linked list with head and tail pointers.
  - **Time Complexity**: O(1) for end insertions.
  - **Performance**: Offers consistent and improved performance over both arrays and standard linked lists.
- **Use Case**: Ideal for scenarios with high insertion rates and large datasets.

---

## Insights
- **Array-based Storage**:
  - Easy to implement but becomes inefficient as the dataset grows due to the O(n) insertion cost and resizing overhead.
- **Linked Lists**:
  - Provide faster insertion times, with doubly linked lists offering the most efficient performance for dynamic datasets.

## Conclusion
Choosing the right data structure is critical for optimizing the performance of tasks involving FASTA records. Arrays are suitable for small datasets, while linked lists—especially with tail pointers—are better for large, dynamically growing datasets.
