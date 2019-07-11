#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int echo(char *filename)
{
	int c;
	int counter = 0;
	
	FILE *ifp = fopen(filename,"r");

    if (ifp == 0)
        printf("Could not open file\n");

	c = fgetc(ifp); 

    printf("\n");

    while (c != EOF) 
    { 
        printf ("%c", c);
        counter++;

        if (counter == 80)
        {
        	counter = 0;
        	printf("\n");
        }

        c = fgetc(ifp);
    } 


    return 0;
}

char *convert_to_string(char *filename)
{
    char *buffer = malloc(sizeof(char) * (1024));

    int i = 0;
    int u;
    char c;

    for (u = 0; u < 1024; u++)
    {
        buffer[u] = '\0';
    }


    FILE *ifp = fopen(filename,"r");

    if (ifp == 0)
        return NULL;

    c = fgetc(ifp);

    while (c != '\n') 
    { 
        buffer[i] = c;
        c = fgetc(ifp); 
        i++;
    }


    buffer[i++] = '\n';

    fclose(ifp);

    return buffer;
}

void checksum_get_8(char *buffer, int checkSumSize)
{
    int i, j;
    long unsigned int checksum_8;

    int characterCnt = strlen(buffer);

    if (checkSumSize == 8)
    {
        for(i = 0; i < characterCnt; i++)
        {
            checksum_8 += (unsigned char) buffer[i];
        }

        checksum_8 = (checksum_8 & 0xFF);

        printf("%2d bit checksum is %8lx for all %4d chars\n", checkSumSize, checksum_8, characterCnt);
    }
}

void checksum_get_16(char *buffer, int checkSumSize)
{
    int i, j;
    short unsigned int checksum_16;

    int characterCnt = strlen(buffer);

    if ((characterCnt % 2) != 0)
    {
        buffer[characterCnt++] = 'X';
        printf("%c\n", 'X');
    }
    
    for(i = 0; i < characterCnt; i++)
    {
        checksum_16 += (unsigned short) buffer[i] * 256;
        i++;
        checksum_16 += (unsigned short) buffer[i];
    }

    checksum_16 = (checksum_16 & 0xFFFF);

    printf("%2d bit checksum is %8hx for all %4d chars\n", checkSumSize, checksum_16, characterCnt);
    
}

void checksum_get_32(char *buffer, int checkSumSize)
{
    int i, j;
    unsigned int checksum_32;

    int characterCnt = strlen(buffer);


        while((characterCnt % 4) != 0)
        {
            buffer[characterCnt++] = 'X';
            printf("%c", 'X');
        }

        printf("\n");

        for(i = 0; i < characterCnt; i++)
        {
            checksum_32 += ((unsigned int) buffer[i] * (256 * 256 * 256));
            i++;
            checksum_32 += ((unsigned int) buffer[i] * (256 * 256));
            i++;
            checksum_32 += ((unsigned int) buffer[i] * (256));
            i++;
            checksum_32 += (unsigned int) buffer[i];
        }
       
        printf("%2d bit checksum is %8x for all %4d chars\n", checkSumSize, checksum_32, characterCnt);

    return;
}

int checksum_check(int checksum)
{

	if (checksum != 8 && checksum != 16 && checksum != 32)
	{	
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char** argv)
{
	char *filename = argv[1];
	int checksum = atoi(argv[2]);
	
	if ((checksum_check(checksum)) == 1)
		return 0;

	echo(filename);

    if (checksum == 8)
    {
        printf("\n");
        checksum_get_8(convert_to_string(filename), checksum);
    }

    if (checksum == 16)
    {
        checksum_get_16(convert_to_string(filename), checksum);        
    }

    if (checksum == 32)
    {
        checksum_get_32(convert_to_string(filename), checksum);        
    }

    return 0;
}