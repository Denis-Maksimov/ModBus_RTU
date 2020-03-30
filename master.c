/**
 * ModBus RTU
 * 
 * 
*/
#include "crc16.h"
#include "modbus_general.h"

#include "master.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


enum modbus_cmd_t
{
    MODBUS_READ_COILS=1,            //1 Чтение значений нескольких регистров флагов
    MODBUS_READ_DISCRETE_INPUTS,    //2 Чтение значений нескольких дискретных входов
    MODBUS_READ_HOLDING_REGISTERS,  //3	Чтение значений нескольких регистров хранения
    MODBUS_READ_INPUT_REGISTERS,    //4 Чтение значений нескольких регистров ввода
    MODBUS_WRITE_SINGLE_COIL,       //5	Запись одного регистра флагов
    MODBUS_WRITE_SINGLE_REGISTER,   //6	Запись одного регистра (ввода или хранения)
    MODBUS_WRITE_MULTIPLE_COILS=15, //15 Запись нескольких регистров флагов
    MODBUS_WRITE_MULTIPLE_REGISTER, //16 Запись нескольких регистров (ввода или хранения) 
};




//----------------------------------------

/***********************************************************************************************************
 * 01 (0x01) Read Coils
 * 
 * This  function  code  is  used  to  read  from  1  to  2000  contiguous  status  of  coils  
 * in  a  remote device.  The  Request  PDU  specifies  the  starting  address,  i.e.  the  address  of  the  first 
 * coil specified, and the number of coils. In the PDU Coils are addressed startingat zero. Therefore coils numbered 
 * 1-16 are addressed as 0-15.The  coils  in  the  response  message  are  packed  as  one  coil  per  bit  of  the  data  field.  
 * Status  is indicated as 1= ON and 0= OFF. The LSB of the first data byte contains the output addressed in the query. 
 * The  other coils follow toward the  high order  end  of this byte,  and from low  order to high order in subsequent bytes. 
 * If  the  returned  output  quantity  is  not  a  multiple  of  eight,  the  remaining  bits  
 * in  the  final  data byte  will  be  padded  with  zeros  (toward  the  high  order  end  of  the  byte).  
 * The  Byte  Count  field specifies the quantity of complete bytes of data. 
 * -----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции чтения n флагов с устройства
 * ***********************************************************************************************************/
struct raw_packet* ModBus_Read_Coils(short Starting_Address, short Quantity_of_coils)
{       
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_COILS;
    short* ptr_tmp=PDU->packet+1;
    ptr_tmp[0]=Starting_Address;
    ptr_tmp[1]=Quantity_of_coils;
    return PDU;
}

//-----------------------------------------------------------------------------------------------


/***********************************************************************************************************
 * 02 (0x02) Read Discrete Inputs
 * 
 * This  function  code  is  used  to  read  from  1  to  2000  contiguous  status  of  discrete  inputs  in  a remote  device.  
 * The  Request  PDU  specifies  the  starting  address,  i.e.  the  address  of  the  first input  specified,  
 * and  the  number  of  inputs.  In  the  PDU  Discrete  Inputs  are  addressed  starting at zero. 
 * Therefore Discrete inputs numbered 1-16 are addressed as 0-15.
 * The discrete inputs in the response message are packed as one input per bit of the data field. 
 * Status  is  indicated  as  1=  ON;  0=OFF.  The  LSB  of  the  first  data  byte  contains  the  input addressed  in  the  query.  
 * The  other  inputs  follow  toward  the  high  order  end  of  this  byte,  and from low order to high order in subsequent bytes. 
 * If the returned input quantity is not a multiple of eight, the remaining bits in the final data byte will 
 * be  padded  with  zeros  (toward  the  high  order  end  of  the  byte).  
 * The  Byte  Count  field specifies the quantity of complete bytes of data.
 * -----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции чтения n входов с устройства
 * ***********************************************************************************************************/
struct raw_packet* ModBus_Read_Discrete_Inputs(short Starting_Address, short Quantity_of_inputs)
{
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_DISCRETE_INPUTS;
    short* ptr_tmp=PDU->packet+1;
    ptr_tmp[0]=Starting_Address;
    ptr_tmp[1]=Quantity_of_inputs;
    return PDU;
}
//-----------------------------------------------------------------------------------------------

/***********************************************************************************************************
 * 03 (0x03) Read Holding Registers
 * 
 * This function code is used to read  the contents of a contiguous block of holding registers  in  a remote  device.  
 * The  Request  PDU  specifies  the  starting  register  address  and  the  number  of registers.  
 * In  the  PDU  Registers  are  addressed  starting  at  zero.  Therefore  registers  numbered 1-16 are addressed as 0-15.
 * The  register  data  in  the  response  message  are  packed  as  two  bytes  per  register,  with  the binary contents  
 * right  justified  within  each  byte.  For  each  register,  
 * the  first  byte  contains  the high order bits and the second contains the low order bits.
 * 
 *  -----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции чтения n регистров с устройства
 * ***********************************************************************************************************/
struct raw_packet* ModBus_Read_Holding_Registers(short Starting_Address, short Quantity_of_regs)
{
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_DISCRETE_INPUTS;
    short* ptr_tmp=PDU->packet+1;
    ptr_tmp[0]=Starting_Address;
    ptr_tmp[1]=Quantity_of_regs;
    return PDU;
}

/***********************************************************************************************************
 * 04 (0x04) Read Input Registers
 * 
 * This function code is used to read from 1 to 125 contiguous input registers in a remote device. 
 * The  Request  PDU  specifies  the  starting  register  address  and  the  number  of  registers.  
 * In  the PDU  Registers  are  addressed  starting  at  zero.  
 * Therefore  input  registers  numbered  1-16  are addressed as 0-15.
 * The  register  data  in  the  response  message  are  packed  as  two  bytes  per  register,  
 * with  the binary  contents  right  justified  within  each  byte.  For  each  register,  
 * the  first  byte  contains  the high order bits and the second contains the low order bits.
 * 
 *-----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции чтения n регистров с устройства
 * ***********************************************************************************************************/
struct raw_packet* ModBus_Read_Input_Registers(short Starting_Address, short Quantity_of_regs)
{
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_DISCRETE_INPUTS;
    short* ptr_tmp=PDU->packet+1;
    ptr_tmp[0]=Starting_Address;
    ptr_tmp[1]=Quantity_of_regs;
    return PDU;
}

unsigned char  ModBus_Write_Single_Coil(void*a)
{
    //TODO
    return MODBUS_WRITE_SINGLE_COIL;
}
unsigned char  ModBus_Write_Single_Register(void*a)
{
    //TODO
    return MODBUS_WRITE_SINGLE_REGISTER;
}
unsigned char  ModBus_Write_Multiple_Coils(void*a)
{
    //TODO
    return MODBUS_WRITE_MULTIPLE_COILS;
}
unsigned char  ModBus_Write_Multiple_Register(void*a)
{
    //TODO
    return MODBUS_WRITE_MULTIPLE_REGISTER;
}



void prepare_PDU(enum modbus_cmd_t cmd)
{

}


