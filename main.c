/**
 * ModBus RTU
 * 
 * 
*/

#include "crc16.h"
#include "modbus_general.h"
#include "master.h"
#include "slave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "./serial/Serial.h"
#define c_new(t)     ((t*)malloc(sizeof(t)))
#define c_new_n(t,n)     ((t*)malloc(sizeof(t)*n))
//----------------------------------------
int main()
{
    tADU** TX_mts_pack=  c_new_n(tADU*,4);
    tADU** RX_stm_pack=  c_new_n(tADU*,4);

    //-------- TEST на запись -----------------

    //--MASTER--
    unsigned char Outputs_Value[2]={0xde,0xad};
    TX_mts_pack[0] = master_ModBus_Write_Multiple_Coils(4,0x0000,16,sizeof(Outputs_Value), Outputs_Value);
    TX_mts_pack[1] = master_ModBus_Write_Single_Register(4,0,0x1234);

    // sendRS485(TX_mts_pack)
    
        //    ---->     //

                    //--SLAVE--
                    RX_stm_pack[0] = slave_receive(TX_mts_pack[0]);
                    RX_stm_pack[1] = slave_receive(TX_mts_pack[1]);
                    // RX_stm_pack[3] = slave_receive(TX_mts_pack[3]);
                    // sendRS485(RX_stm_pack)
                    puts("--Slave--");
                    // printf("master_ModBus_Write_Single_Coil     -> ");print_raw(TX_mts_pack[0]);
                    printf("master_ModBus_Write_Single_Register -> ");print_raw(TX_mts_pack[1]);
                    printf("master_ModBus_Write_Multiple_Coils  -> ");print_raw(TX_mts_pack[0]);
                    // print_raw(TX_mts_pack[3]);

        //    <----     //
        
    //--MASTER--
    // master_unpack_response;
    puts("--Master--");
    // printf("master_ModBus_Write_Single_Coil     <- ");print_raw(RX_stm_pack[0]);
    printf("master_ModBus_Write_Single_Register <- ");print_raw(RX_stm_pack[1]);
    printf("master_ModBus_Write_Multiple_Coils  <- ");print_raw(RX_stm_pack[0]);
    // print_raw(RX_stm_pack[3]);

        free_raw(TX_mts_pack[0]);
        free_raw(RX_stm_pack[0]);
        //-------- TEST на чтение -----------------

    //--MASTER--
    TX_mts_pack[0] = master_ModBus_Read_Coils(4, 0x0000, 16);
    TX_mts_pack[1] = master_ModBus_Read_Discrete_Inputs(4,0x0001,12);
    TX_mts_pack[2] = master_ModBus_Read_Holding_Registers(4,0x0000, 12);
    TX_mts_pack[3] = master_ModBus_Read_Input_Registers(4,0x0001,12);

    // sendRS485(TX_mts_pack)
    
        //    ---->     //

                    //--SLAVE--
                    RX_stm_pack[0] = slave_receive(TX_mts_pack[0]);
                    RX_stm_pack[1] = slave_receive(TX_mts_pack[1]);
                    RX_stm_pack[2] = slave_receive(TX_mts_pack[2]);
                    RX_stm_pack[3] = slave_receive(TX_mts_pack[3]);
                    // sendRS485(RX_stm_pack)
                    puts("--Slave--");
                    printf("master_ModBus_Read_Coils             -> ");print_raw(TX_mts_pack[0]);
                    printf("master_ModBus_Read_Discrete_Inputs   -> ");print_raw(TX_mts_pack[1]);
                    printf("master_ModBus_Read_Holding_Registers -> ");print_raw(TX_mts_pack[2]);
                    printf("master_ModBus_Read_Input_Registers   -> ");print_raw(TX_mts_pack[3]);

        //    <----     //
        
    //--MASTER--
    // master_unpack_response;
    puts("--Master--");
    printf("master_ModBus_Read_Coils             <- ");print_raw(RX_stm_pack[0]);
    printf("master_ModBus_Read_Discrete_Inputs   <- ");print_raw(RX_stm_pack[1]);
    printf("master_ModBus_Read_Holding_Registers <- ");print_raw(RX_stm_pack[2]);
    printf("master_ModBus_Read_Input_Registers   <- ");print_raw(RX_stm_pack[3]);

    for(int i=0;i<4;i++){
        free_raw(TX_mts_pack[i]);
        free_raw(RX_stm_pack[i]);
    }



}
