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
//----------------------------------------
int main()
{
    //--MASTER--
    tADU* TX_mts_pack = master_ModBus_Read_Coils(4, 0x01, 12);
    // sendRS485(TX_mts_pack)
    
        //    ---->     //

                    //--SLAVE--
                    tADU* RX_stm_pack = slave_receive(TX_mts_pack);
                    // sendRS485(RX_stm_pack)
                    puts("--Slave--");
                    print_raw(TX_mts_pack);

        //    <----     //
        
    //--MASTER--
    // master_unpack_response;
    puts("--Master--");
    print_raw(RX_stm_pack);

}
