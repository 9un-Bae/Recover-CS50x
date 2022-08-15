#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check for one argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    // open file and check for error
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Error: File unable to open");
        return 2;
    }
    // store blocks of 512 bytes in array
    unsigned char buffer[512];

    // track number of images
    int count_image = 0;

    // files of recovered images
    FILE *output_file = NULL;

    // char filename
    char *filename = malloc(8 * sizeof(char));

    // read blocks of 512 bytes
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        // check bytes for jpeg
        if (buffer[0] == 0xff && buffer[1]  == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // jpeg filename
            sprintf(filename, "%03i.jpg", count_image);
            // output file for writing
            output_file = fopen(filename, "w");
            // count number of images
            count_image++;
        }
        // check if output is used for input
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }
    free(filename);
    fclose(output_file);
    fclose(input_file);
    return 0;
}