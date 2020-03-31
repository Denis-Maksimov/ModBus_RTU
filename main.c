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
    int slave_addres=4;

    printf("~~testing: master_ModBus_Read_Coils~~\n");
     master_ModBus_Read_Coils(slave_addres, 0xdead, 0xf00d);

    printf("\n~~testing: master_ModBus_Read_Discrete_Inputs~~\n");
     master_ModBus_Read_Discrete_Inputs(slave_addres, 0xcafe, 0xbabe);
     printf("\n");


}
