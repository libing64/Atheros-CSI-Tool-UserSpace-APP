#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "iwl_structs.h"

#define BUFFERSIZE 4096
FILE* open_file(char* filename, char* spec);
void caught_signal(int sig);
void exit_program(int code);
void exit_program_err(int code, char* func);

int fd;
FILE* fp;

unsigned char buf[BUFFERSIZE];//csi buffer
unsigned char* packet = NULL;

//COMPLEX csi_matrix[3][3][114];
//csi_struct*   csi_status;
iwl_notify *notify = (iwl_notify*)buf; 

int main(int argc, char* argv[])
{
    
    if(argc != 2)
    {
        exit_program_err(-1, "not output filename");
        return -1;
    }

    FILE* fp = open_file(argv[1], "w");

    int fd = open("/dev/CSI_dev", O_RDWR);
    if(fd == -1)
    {
        exit_program_err(-1, "failed to open device");
    }
    
    int count = 0;
    while(1)
    {
        int ret = read(fd, buf, BUFFERSIZE);

        if (ret > 0)
        {
            
            count++;
            notify = (iwl_notify*) buf;

            uint16_t csi_len = notify->csi_len;//csi payload len
            uint16_t payload_len = notify->payload_len; 
            //uint16_t payload_len = *(uint16_t*)( buf + sizeof(iwl_notify) );
            uint16_t total_len = sizeof(iwl_notify) + csi_len + payload_len;

            uint16_t buf_len = *(uint16_t*)( buf + sizeof (iwl_notify) + csi_len + payload_len );

            uint8_t rate = notify->rate;
            printf("rate: %hhu, csi_len: %hu, payload_len: %hu, total_len: %hu, buf_len: %hu\n", rate, csi_len, payload_len, total_len, buf_len);
            //packet = (buf + sizeof(iwl_notify) + csi_len);

            //printf("rate: %hhu, payload_len: %hu\n",  rate, payload_len);
            //printf("rate: %hhu, csi_len: %hu, buf_len: %hu\n",  rate, csi_len, buf_len);

            fwrite(&buf_len, 1, 2, fp);
            fwrite(buf, 1, buf_len, fp);
        }
    }

    fclose(fp);
    close(fd);
    return 0;
}


FILE* open_file(char* filename, char* spec)
{
    FILE* fp = fopen(filename, spec);
    if (!fp) {
        perror("fopen");
        exit_program(1);
    }
    return fp;
}

void caught_signal(int sig)
{
    fprintf(stderr, "Caught signal %d\n", sig);
    exit_program(0);
}

void exit_program(int code)
{
    if (fp) {
        fclose(fp);
        fp = NULL;
    }

    if (fd != -1) {
        close(fd);
        fd = -1;
    }
    exit(code);
}

void exit_program_err(int code, char* func)
{
    perror(func);
    exit_program(code);
}
