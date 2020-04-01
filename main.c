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
    unsigned int slave_addres=4;

    printf("~~testing: master_ModBus_Read_Coils~~\n");
     master_ModBus_Read_Coils(slave_addres, 0xdead, 0x03);

    printf("\n~~testing: master_ModBus_Read_Discrete_Inputs~~\n");
     master_ModBus_Read_Discrete_Inputs(slave_addres, 0xcafe, 0x03);
     printf("\n");

    printf("\n~~testing: master_ModBus_Read_Holding_Registers~~\n");
    master_ModBus_Read_Holding_Registers(slave_addres, 0xcafe, 0x03);
    
    printf("\n~~testing: master_ModBus_Read_Input_Registers~~\n");
    master_ModBus_Read_Input_Registers(slave_addres, 0xcafe, 0x03);

    printf("\n~~testing: master_ModBus_Write_Single_Coil~~\n");
    master_ModBus_Write_Single_Coil(slave_addres, 0xcafe, 0x03);

    printf("\n~~testing: master_ModBus_Write_Single_Register~~\n");
    master_ModBus_Write_Single_Register(slave_addres, 0xcafe, 0x03);

    printf("\n~~testing: master_ModBus_Write_Multiple_Coils~~\n");
    unsigned char Outputs_Value[]={0xde,0xad,0xf0,0x0d,0xc0,0x01,0x11};
    master_ModBus_Write_Multiple_Coils(slave_addres, 0xcafe, 0x03, sizeof(Outputs_Value), Outputs_Value);

}
