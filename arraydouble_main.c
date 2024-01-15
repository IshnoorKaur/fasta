#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "fasta.h" // Include the header file
#include "fasta_read.c"

// Function to process FASTA records from a file and store them in an array
int processFasta(char *filename, double *timeTaken)
{
    FILE *fp;
    FASTArecord fRecord;
    int totalRecords = 0;
    int recordsProcessed = 0;
    int eofSeen = 0;
    int arraySize = 1; // Initial array size
    FASTArecord *records = malloc(arraySize * sizeof(FASTArecord));

    if (records == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        return -1;
    }

    clock_t startTime, endTime;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failure opening %s: %s\n", filename, strerror(errno));
        free(records); // Free the allocated records array
        return -1;
    }

    // Record the time now, before we do the work
    startTime = clock();

    do
    {
        // Print a '.' every 10,000 records so we know something is happening
        if ((recordsProcessed % 10000) == 0)
        {
            printf(".");
            fflush(stdout);
        }

        fastaInitializeRecord(&fRecord);

        int status = fastaReadRecord(fp, &fRecord);
        if (status == 0)
        {
            eofSeen = 1;
        }
        else if (status > 0)
        {
            // storing information from structure into array
            records[recordsProcessed].description = strdup(fRecord.description);
            records[recordsProcessed].sequence = strdup(fRecord.sequence);
            records[recordsProcessed].id = fRecord.id;

            totalRecords++;
            recordsProcessed++;

            // Check if we need to expand the array
            if (recordsProcessed >= arraySize)
            {
                arraySize *= 2;
                FASTArecord *newRecords = realloc(records, arraySize * sizeof(FASTArecord));
                if (newRecords == NULL)
                {
                    fprintf(stderr, "Memory reallocation error\n");
                    fclose(fp);
                    free(records);
                    return -1;
                }
                records = newRecords;
            }

            // Perform your desired operations on the record here
            // fastaPrintRecord(stdout, &fRecord);
            fastaClearRecord(&fRecord);
        }
        else
        {
            fprintf(stderr, "Error: failure while processing '%s'\n", filename);
            fclose(fp);
            free(records);
            return -1;
        }

    } while (!eofSeen);

    // Check if 100,000 records have been processed
    if ((recordsProcessed % 100000) == 0)
    {
        double wastePercentage = ((double)arraySize - recordsProcessed) / arraySize * 100;
        printf(" %d FASTA records -- %d allocated (%.3lf%% waste)\n", recordsProcessed, arraySize, wastePercentage);
    }
    fastaClearRecord(&fRecord);

    // Print a specific record from the records array
    /*int recordIndexToPrint = 1;

    if (recordsProcessed > 0 && recordIndexToPrint >= 0 && recordIndexToPrint < recordsProcessed)
    {
        printf("Printing record at index %d:\n", recordIndexToPrint);
        printf("Description: %s\n", records[recordIndexToPrint].description);
        printf("Sequence: %s\n", records[recordIndexToPrint].sequence);
        printf("ID: %ld\n", records[recordIndexToPrint].id);
    }
    else
    {
        printf("Record at index %d does not exist.\n", recordIndexToPrint);
    } */

    // Record the time now, when the work is done, and calculate the difference
    endTime = clock();

    (*timeTaken) = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    fclose(fp);
    // Free dynamically allocated memory for description and sequence
    for (int i = 0; i < recordsProcessed; i++)
    {
        free(records[i].description);
        free(records[i].sequence);
    }
    free(records);

    return totalRecords;
}

int processFastaRepeatedly(char *filename, long repeatsRequested)
{
    double totalTimeInSeconds = 0;
    int status;
    long i;

    for (i = 0; i < repeatsRequested; i++)
    {
        double timeThisIterationInSeconds;
        status = processFasta(filename, &timeThisIterationInSeconds);
        if (status < 0)
        {
            // Free the records array if an error occurs
            printf("Ending!\n");
            return -1;
        }
        totalTimeInSeconds += timeThisIterationInSeconds;
    }

    printf("%.3lf seconds taken for processing total\n", totalTimeInSeconds);

    totalTimeInSeconds /= (double)repeatsRequested;

    int minutesPortion = (int)(totalTimeInSeconds / 60);
    totalTimeInSeconds = totalTimeInSeconds - (60 * minutesPortion);

    printf("On average: %d minutes, %.3lf seconds per run\n", minutesPortion, totalTimeInSeconds);

    return status;
}

/**
 * Program mainline
 */
int main(int argc, char **argv)
{
    // If there is an insufficient number of command line arguments
    if (argc < 2)
    {
        // No file names provided, print usage message to stderr
        fprintf(stderr, "Usage: %s <file> [<file>...]\n", argv[0]);
        return 1;
    }

    long repeatsRequested = 1;
    int totalProcessedRecords = 0;
    FASTArecord *records = NULL;
    //FASTArecord *records = malloc(sizeof(FASTArecord));

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-') // Check for command-line options
        {
            if (argv[i][1] == 'R') // Check for the -R option
            {
                if (i + 1 < argc)
                {
                    if (sscanf(argv[i + 1], "%ld", &repeatsRequested) != 1)
                    {
                        fprintf(stderr, "Error: Invalid repeat count argument.\n");
                        return 1;
                    }
                    i++;
                }
                else
                {
                    fprintf(stderr, "Error: Missing argument for -R option.\n");
                    return 1;
                }
            }
            else
            {
                fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
                return 1;
            }
        }
        else
        {
            int status = processFastaRepeatedly(argv[i], repeatsRequested);

            if (status < 0)
            {
                fprintf(stderr, "Error: Processing '%s' failed -- exiting\n", argv[i]);
            // Free the dynamically allocated records array
                /*for (int j = 0; j < totalProcessedRecords; j++)
                {
                    free(records[j].description);
                    free(records[j].sequence);
                }
                free(records);   */             
                return 1;
            }

            totalProcessedRecords += status;
        }
    }

    // After processing, you can free the records array
    free(records);
    printf("Total records processed: %d\n", totalProcessedRecords);

    return 0;
}
