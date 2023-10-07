// research_tracker.h

#ifndef RESEARCH_TRACKER_H
#define RESEARCH_TRACKER_H

// Maximum number of research papers
#define MAX_PAPERS 20

// Structure to represent the research paper
struct ResearchPaper {
    char title[100];
    char authors[100];
    char filePath[100];
};

// Function prototypes
void writeResearchPapersToFile(struct ResearchPaper papers[], int numPapers);
int readResearchPapersFromFile(struct ResearchPaper papers[]);
void addResearchPaper(struct ResearchPaper papers[], int *numPapers);
void viewResearchPapers(struct ResearchPaper papers[], int numPapers);
void viewFileContent(const char *filePath, const char *title);
void deleteResearchPaperFromFile(int deleteIndex);
void deleteFile(struct ResearchPaper papers[], int deleteIndex);

#endif // RESEARCH_TRACKER_H

