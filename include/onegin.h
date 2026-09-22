#include <stdlib.h>

// **********************
//       Structures
// **********************

struct OneginLine {
    size_t line_size;
    char* line_pointer;
};


struct OneginFile {
    char* buffer;
    OneginLine* index;
    size_t buffer_size;
    unsigned number_of_lines;
};

// **********************
//         Main
// **********************

void PrintUsage (const char* argv_0);
unsigned CountNumberOfLines (const OneginFile file);
void DivideIntoLines (OneginFile file);

// **********************
//         Sort
// **********************

void ExchangeElements (void* a, void* b, size_t SzElem);
unsigned Divide (void* data, unsigned left, unsigned right, size_t SzElem, int (*CompareElements)(const void* a, const void* b));
void MyQuickSort (void* data, size_t size, unsigned left, unsigned right, size_t SzElem, int (*CompareElements) (const void* a, const void* b));

// **********************
//         Reader
// **********************

int StatIsFileExists (const char* filename);
size_t StatFileSize (int fd);
ssize_t ReadFileToBuf (int fd, char** buffer_ptr, size_t buffer_size);
size_t ReadFileAndRealloc (int fd, char** buffer_ptr, size_t orig_buffer_size);

// **********************
//      Comparators
// **********************

int NormalComparator (const void* a_ptr, const void* b_ptr);
int InverseComparator (const void* a_ptr, const void* b_ptr);