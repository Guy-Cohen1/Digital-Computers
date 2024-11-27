#ifndef HEADER_FLASH_H_
#define HEADER_FLASH_H_

#include  <msp430g2553.h>          // MSP430x2xx


extern void update_file_size(void);
extern void transfer_file_to_flash(void);
typedef struct Files{
    short files_amount;
    char file_name[11];
    int* file_ptr[3];
    int file_size[3];
}Files;
extern Files file;
#endif
