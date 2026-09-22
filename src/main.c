#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <debug52.h>

#include "onegin.h"

int main (int argc, char** argv) {
    int fd = -1;

    if ((argc >= 4) || 
        (argc >= 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "--help"))) ||
        (argc >= 3 && (strcmp(argv[2], "-h") || strcmp(argv[2], "--help"))) ||
        (argc >= 3 && !strcmp(argv[2], "-C"))) 
    {
        PrintUsage(argv);
        exit(0);
    }

    if (argc == 1) {
        fd = open ("onegin.txt", O_RDONLY);
    }
    else {
        if (StatIsFileExists(argv[1]) == 0) {
            printf ("Файл %s не существует. Завершение\n", argv[1]);
            exit(1);
        }

        fd = open (argv[1], O_RDONLY);
    }

    assert(fd != -1);

    OneginFile file_obj = {};

    file_obj.buffer_size = StatFileSize (fd);

    file_obj.buffer_size++;

    file_obj.buffer = (char*) calloc (file_obj.buffer_size, sizeof(char));

    file_obj.buffer_size = ReadFileAndRealloc (fd, &file_obj.buffer, file_obj.buffer_size);
    
    file_obj.number_of_lines = CountNumberOfLines (file_obj);

    file_obj.index = (OneginLine*)calloc (file_obj.number_of_lines, sizeof (OneginLine));

    DivideIntoLines (file_obj);

    MyQuickSort (file_obj.index, file_obj.number_of_lines, 0, file_obj.number_of_lines - 1, sizeof (OneginLine), NormalComparator);

    FILE* outfile = 0;

    if (argc == 3 && !strcmp (argv[2], "-C"))
        outfile = stdout;
    else    
        outfile = fopen ("./output.txt", "w");

    assert (outfile != 0);

    fprintf (outfile, "*****************************************************************************\n");
    fprintf (outfile, "                                  NORMALLY SORTED\n");
    fprintf (outfile, "*****************************************************************************\n");

    for (unsigned i = 0; i < file_obj.number_of_lines; i++) {
        fprintf (outfile, "%s\n", file_obj.index[i].line_pointer);
    }
    
    fprintf (outfile, "*****************************************************************************\n");
    fprintf (outfile, "                                  INVERSE SORTED\n");
    fprintf (outfile, "*****************************************************************************\n");

    qsort (file_obj.index, file_obj.number_of_lines, sizeof(OneginLine), InverseComparator);

    for (unsigned i = 0; i < file_obj.number_of_lines; i++) {
        fprintf (outfile, "%s\n", file_obj.index[i].line_pointer);
    }

    fprintf (outfile, "*****************************************************************************\n");
    fprintf (outfile, "                                 IMMORTAL ORIGINAL\n");
    fprintf (outfile, "*****************************************************************************\n");

    char* buffer_output_ptr = file_obj.buffer;
    for(unsigned i = 0; i < file_obj.number_of_lines; i++){
        int transmitted = fprintf (outfile, "%s\n", buffer_output_ptr);
        buffer_output_ptr += transmitted;
    }

    close (fd);
    fclose (outfile);

    free (file_obj.index);
    free (file_obj.buffer);
}

void PrintUsage (char** argv) {
    printf ("Использование: %s [FILE.txt] [-C] \n\n", argv[0]);
    printf ("[FILE.txt] - Файл для анализа и сортировки\n");
    printf ("[-C] - Вывод в stdout вместо выходного файла output.txt\n");
}

unsigned CountNumberOfLines (const OneginFile file) {
    unsigned result = 0;
    char* ptr = file.buffer;
    while (ptr != 0 && ((uintptr_t)ptr - (uintptr_t)file.buffer) < file.buffer_size) {
        ptr = strchr(ptr, '\n');
        if (ptr != 0) {
            ptr++;
            result++;
        }
    }

    return result + 1;
}

void DivideIntoLines (const OneginFile file) {
    char* ptr = file.buffer;
    unsigned i = 0;

    OneginLine* index = file.index;

    index[i].line_pointer = ptr;
    i++;

    while (ptr = strchr(ptr, '\n'), ptr != 0) {
        *ptr = '\0';
        assert (i < file.number_of_lines);
        index[i].line_pointer = ptr + 1;
        index[i - 1].line_size = (size_t)(index[i].line_pointer - index[i - 1].line_pointer);
        ptr++;
        i++;
    }

    index[i - 1].line_size = (size_t)(file.buffer + file.buffer_size - (i >= 1 ? index[i - 1].line_pointer : 0) + 1);
}