/**
 * ModBus RTU
 * 
 * 
*/

#include "crc16.h"
#include "modbus_general.h"
#include "master.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//----------------------------------------
int main()
{
    //--MASTER--send
    int destination_addres=4;
    char data[]="Hello, hello";
    int n = strlen(data)+1; //magic


    struct raw_packet* raw = pack_data(destination_addres, data, n);

    printf("CRC=0x%X\n",CRC16(data,n));
    printf("data=%s\n",data);

    //--SLAVE--receive

    free(unpack_data(raw->packet, raw->n));
    free_raw(raw);


}
