#include  "../header/flash.h"    // Include the header for flash memory operations (private library)
#include  "../header/halGPIO.h"    // Include the header for GPIO hardware abstraction layer (private library)
#include  "string.h"              // Include the standard string library for string manipulation functions



Files file;   // Define a global variable `file` of type `Files` to manage file operations


void update_file_size(void)
{
    // Function to set the size of the current file in the `file` structure
    file.file_size[file.files_amount - 1] = strlen(Rec_data) - 1;
}


void transfer_file_to_flash(void)
{
    // Function to write data to flash memory
    char *temp_pointer;    // Pointer to the location in flash memory where data will be written
    int l;

    temp_pointer = file.file_ptr[file.files_amount - 1];  // Initialize the pointer to the current file's flash memory location

    // Prepare for flash memory erase operation
    FCTL1 = FWKEY + ERASE;
    FCTL3 = FWKEY;

    // Erase the flash segment where data will be written
    *temp_pointer = 0;
    FCTL1 = FWKEY + WRT;     // Set the WRT bit to enable writing to flash memory


    for (l = 0; l < file.file_size[file.files_amount - 1]; l++)
    {
        if (Rec_data[l] == 0x0A || Rec_data[l] == 0x0D ){ // Skip carriage return (0x0D) and line feed (0x0A) characters
            continue;
        }
        // Write the data byte to flash memory and increment the pointer
        *temp_pointer++ = Rec_data[l];
    }

    memset(RxData, 0, strlen(RxData)); // Clear the received data buffer after writing to flash memory

    FCTL1 = FWKEY;           // Disable write operation (reset to default state)
    FCTL3 = FWKEY + LOCK;
}
