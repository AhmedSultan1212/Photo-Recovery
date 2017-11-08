#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cs50.h>
typedef uint8_t  BYTE;

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    FILE *read = fopen(argv[1], "r");
    if (read == NULL)
    {
        fprintf(stderr, "Cannot open %s\n", argv[1]);
        return 2;
    }
    BYTE bits[512];
    int file = 0;
    int blocks = 0;
    int block_num = 1;
    
    while (!feof(read))
    {
        fseek(read, blocks * 512, SEEK_SET);
        for (int i = 0; i < 512; i++)
        {
            fread(&bits[i], sizeof(BYTE), 1, read);
        }
            if (bits[0] == 0xff && bits[1] == 0xd8 && bits[2] == 0xff && (bits[3] >= 0xe0 && bits[3] < 0xf0))
            {
                char files[50];
            sprintf(files, "%03d", file);
            strcat(files, ".jpg");
            file++;
            FILE *output = fopen(files, "w");
            if (output == NULL)
            {
                fprintf(stderr, "Cannot write to file\n");
                return 2;
            }
                while (!feof(read))
                {
                    for (int i = 0; i < 512; i++)
                    {
                        fread(&bits[i], sizeof(BYTE), 1, read);
                    }
                    if (bits[0] == 0xff && bits[1] == 0xd8 && bits[2] == 0xff && bits[3] >= 14)
                        break;
                    else if (!feof(read))
                        block_num++;
                }
                fseek(read, blocks * 512, SEEK_SET);
                for (int j = 0; j < block_num; j++)
                {
            for (int i = 0; i < 512; i++)
            {
                fread(&bits[i], sizeof(BYTE), 1, read);
                fwrite(&bits[i], sizeof(BYTE), 1, output);
            }
            }
            fclose(output);
            blocks += block_num;
            block_num = 1;
            }
            else
                blocks++;
        
    }
    fclose(read);
    return 0;
}