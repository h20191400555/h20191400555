
#include<fcntl.h>
#include<linux/ioctl.h>


#define MAGIC_NUMBER 'A'
#define ch_SELECT_SEQNO 0x01
#define RESULT_ALIGNMENT_SEQNO 0x02

#define ch_SELECT _IOW(MAGIC_NUMBER, ch_SELECT_SEQNO, unsigned long)
#define RESULT_ALIGNMENT _IOW(MAGIC_NUMBER, RESULT_ALIGNMENT_SEQNO, unsigned long)


unsigned int ch;
unsigned int align;
unsigned int result;

int main(int arg,char *files[])
{
	int D_file, U_file;
	if(arg != 2)
	{
		printf("Usage : copy <oldfile> <newfile>\n");
		exit(1);
	}

	D_file = open(files[1], O_RDONLY);

	if(D_file == -1)
	{
		printf("cannot open source file\n");
		exit(1);
	}

	printf("Enter the Required ch number of ADC(0-7) : \n");
        scanf("%d",&ch);       
        ioctl(D_file, ch_SELECT, &ch); 
 
        printf("Enter the Required Allignment(0/1) : \n");
	scanf("%d",&align); 
        ioctl(D_file, RESULT_ALIGNMENT, &align);
       
 
        printf("Reading value from ADC...\n");
	read(D_file, &result, 2);
	printf("Value read from ADC - %d \n", result);
        close(D_file);
	exit(0);
}

