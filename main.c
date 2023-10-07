#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "research_paper.h"

#define MAX_LENGTH 500


void writeResearchPapersToFile(struct ResearchPaper papers[], int numPapers) {
    FILE *file = fopen("research_papers.txt", "w");
    if (file == NULL) {
        printf("\033[31mError opening file for writing.\033[0m\n");
        return;
    }

    for (int i = 0; i < numPapers; i++) {
        fprintf(file, "%s\n%s\n%s\n", papers[i].title, papers[i].authors, papers[i].filePath);
    }

    fclose(file);
}


int readResearchPapersFromFile(struct ResearchPaper papers[]) {
    FILE *file = fopen("research_papers.txt", "r");
    if (file == NULL) {
        printf("\033[31mNo existing data file found.\033[0m\n");
        return 0;
    }

    int numPapers = 0;
    while (numPapers < MAX_PAPERS &&
           fscanf(file, " %[^\n]\n%[^\n]\n%[^\n]",
                  papers[numPapers].title,
                  papers[numPapers].authors,
                  papers[numPapers].filePath) == 3) {
        numPapers++;
    }

    fclose(file);
    return numPapers;
}


void addResearchPaper(struct ResearchPaper papers[], int *numPapers) {
    if (*numPapers < MAX_PAPERS) {
        // Get the title from the user
        printf("Enter title: ");
        scanf(" %[^\n]", papers[*numPapers].title);
        getchar();  // Consume the newline character

        // Create a new file with the title as the name
        char fileName[100];
        sprintf(fileName, "%s.txt", papers[*numPapers].title);
        
        FILE *file = fopen(fileName, "w");
        if (file == NULL) {
            printf("\033[31mError creating file.\033[0m\n");
            return;
        }
        fprintf(file, "Title: %s\n", papers[*numPapers].title);

        // Get and write authors and file path into the file
        printf("Enter authors: ");
        scanf(" %[^\n]", papers[*numPapers].authors);
        getchar();  // Consume the newline character
        fprintf(file, "Authors: %s\n", papers[*numPapers].authors);
        printf("Enter file path: ");
        scanf(" %[^\n]", papers[*numPapers].filePath);
        getchar();  // Consume the newline character
        fprintf(file, "File Path: %s\n", papers[*numPapers].filePath);
        
        // Get and write file content into the file
        printf("Enter file content (end with an empty line):\n");
        char content[500];
        while (1) {
            fgets(content, sizeof(content), stdin);
            if (strcmp(content, "\n") == 0) {
                break;  // Exit the loop if an empty line is entered
            }
            fputs(content, file);
        }

        fclose(file);

        (*numPapers)++;
        printf("\033[32mResearch paper added.\033[0m\n");
    } else {
        printf("Maximum number of papers reached.\n");
    }
}




void viewResearchPapers(struct ResearchPaper papers[], int numPapers) {
    if (numPapers == 0) {
        printf("No research papers available.\n");
        return;
    }

    printf("Research Papers:\n");
    for (int i = 0; i < numPapers; i++) {
        printf("Paper %d:\n", i + 1);
        printf("Title: %s\n", papers[i].title);
        printf("Authors: %s\n", papers[i].authors);
        /*printf("File Path: %s\n", papers[i].filePath);*/
        printf("--------------------------\n");
    }
}


void viewFileContent(const char *filePath, const char *title) {
     char completeFilePath[256];
    snprintf(completeFilePath, sizeof(completeFilePath), "%s/%s.txt", filePath, title);

    //printf("Opening file: %s\n", completeFilePath); // Print the file path
    FILE *file = fopen(completeFilePath, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    printf("File Content:\n");
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}


void deleteResearchPaperFromFile(int index) {
    FILE *file = fopen("research_papers.txt", "r");
    if (file == NULL) {
        printf("\033[31mNo existing data file found.\033[0m\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("\033[31mError creating temporary file.\033[0m\n");
        fclose(file);
        return;
    }

    int currentPaperIndex = 0;
    struct ResearchPaper currentPaper;

    while (fscanf(file, " %[^\n]\n%[^\n]\n%[^\n]",
                  currentPaper.title,
                  currentPaper.authors,
                  currentPaper.filePath) == 3) {
        if (currentPaperIndex != index) {
            fprintf(tempFile, "%s\n%s\n%s\n",
                    currentPaper.title,
                    currentPaper.authors,
                    currentPaper.filePath);
        }
        currentPaperIndex++;
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    remove("research_papers.txt");
    rename("temp.txt", "research_papers.txt");

    printf("Paper at index %d has been deleted.\n", index);
}


void deleteFile(struct ResearchPaper papers[], int deleteIndex) {
    // Concatenate the title with ".txt" to get the complete file name
    char fileName[200]; // Assuming the file name won't exceed 200 characters
    snprintf(fileName, sizeof(fileName), "%s.txt", papers[deleteIndex].title);

    // Delete the file using remove() function
    if (remove(fileName) == 0) {
        printf("File '%s' has been deleted.\n", fileName);

        // Clear the file path and title in the papers array
        strcpy(papers[deleteIndex].title, "");
        strcpy(papers[deleteIndex].authors, "");
        strcpy(papers[deleteIndex].filePath, "");
    } else {
        printf("Error deleting file '%s'.\n", fileName);
    }
}



int main() {
    struct ResearchPaper papers[MAX_PAPERS];
    int numPapers = 0;

    // Load existing data from the file
    numPapers = readResearchPapersFromFile(papers);

    int choice;
    do {
        // Display menu options
        printf("\nMenu:\n");
        printf("1. Add Research Paper\n");
        printf("2. View Research Papers\n");
        printf("3. View File Content\n");
        printf("4. delete\n");
        printf("5.Exit.\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addResearchPaper(papers, &numPapers);
                writeResearchPapersToFile(papers, numPapers); // Save after adding
                break;
            case 2:
                viewResearchPapers(papers, numPapers);
                break;
            case 3: {
                int selectedPaperIndex;
                printf("Enter the index of the paper: ");
                scanf("%d", &selectedPaperIndex);
                /*printf("%d",selectedPaperIndex);*/
                if (selectedPaperIndex-1 >= 0 && selectedPaperIndex-1 < numPapers) {
                    viewFileContent(papers[selectedPaperIndex-1].filePath, papers[selectedPaperIndex-1].title);
                } else {
                    printf("Invalid paper index.\n");
                }
                break;
            }
            case 4: {
                int deleteIndex;
                printf("Enter the index of the paper: ");
                scanf("%d", &deleteIndex);
                deleteFile(papers, deleteIndex);
                deleteResearchPaperFromFile(deleteIndex);
                break;
            }
            case 5:
                writeResearchPapersToFile(papers, numPapers); // Save before exiting
                printf("\033[31mExiting the program....\033[0m\n");
                break;
            default:
                printf("Invalid choice. Please select a valid option.\n");
        }
    } while (choice != 5);

    return 0;
}
