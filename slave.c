


/***********************************************************************************************************
 * 01 (0x01) Read CoilsThis  function  code  is  used  to  read  from  1  to  2000  contiguous  status  of  coils  
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
#include "crc16.h"
#include "modbus_general.h"

#include "master.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct raw_packet*(*modbus_func)(void*);

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

//------------------------------------
char IO_read_coil(short Starting_Address)
{
    //TODO!!!
    return 0x01;
}
///TODO: check address 
struct raw_packet* ModBus_Read_Coils(void* args)
{   
    short* _args = args;
    short Starting_Address=_args[0];
    short Quantity_of_coils=_args[1];

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    int _n=0;
    int bytes=0;
    (Quantity_of_coils&0xff)?(bytes=Quantity_of_coils+1):(bytes=Quantity_of_coils);
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_COILS;
    PDU->packet[1]=bytes;
    int count_of_coils = 0;

    //-- Заполняем значения катушек (флагов, битов)
    for (int i=2;i< bytes+2;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        for(int b=0;b<8;b++){
            
            PDU->packet[i] |= (IO_read_coil(Starting_Address)|(1<<b));
            Starting_Address++;
            count_of_coils++;
            if(count_of_coils==Quantity_of_coils) return PDU;
        }
        
    }
    
    return PDU;
}
//------------------------------------
char IO_read_Discrete_Inputs(short Starting_Address)
{
    //TODO!!!
    return 0x01;
}
///TODO: check address 
struct raw_packet* ModBus_Read_Discrete_Inputs(void* args)
{   
    short* _args = args;
    short Starting_Address=_args[0];
    short Quantity_of_coils=_args[1];

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    int _n=0;
    int bytes=0;
    (Quantity_of_coils&0xff)?(bytes=Quantity_of_coils+1):(bytes=Quantity_of_coils);
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_DISCRETE_INPUTS;
    PDU->packet[1]=bytes;
    int count_of_coils = 0;

    //-- Заполняем значения катушек (флагов, битов)
    for (int i=2;i< bytes+2;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        for(int b=0;b<8;b++){
            
            PDU->packet[i] |= (IO_read_Discrete_Inputs(Starting_Address)|(1<<b));
            Starting_Address++;
            count_of_coils++;
            if(count_of_coils==Quantity_of_coils) return PDU;
        }
        
    }
    
    return PDU;
}
//------------------------------------
short IO_read_reg(short Starting_Address)
{
    //TODO!!!
    return 0x01;
}

struct raw_packet* ModBus_Read_Holding_Registers(void* args)
{   
    short* _args = args;
    short Starting_Address=_args[0];
    short Quantity_of_regs=_args[1];

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    int _n=0;
    int bytes = Quantity_of_regs*2;
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_HOLDING_REGISTERS;
    PDU->packet[1]=bytes;
    int count_of_coils = 0;

    //-- Заполняем значения регистров
    short* reg=(&(PDU->packet)+2);
    for (int i=0;i< Quantity_of_regs;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        reg[i] = IO_read_reg(Starting_Address+i);
    }
    return PDU;
}



//---------------------------------------------------------

short IO_read_in_reg(short Starting_Address)
{
    //TODO!!!
    return 0x01;
}

struct raw_packet* ModBus_Read_Input_Registers(void* args)
{   
    short* _args = args;
    short Starting_Address=_args[0];
    short Quantity_of_regs=_args[1];

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    int _n=0;
    int bytes = Quantity_of_regs*2;
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_HOLDING_REGISTERS;
    PDU->packet[1]=bytes;
    int count_of_coils = 0;

    //-- Заполняем значения регистров
    short* reg=(&(PDU->packet)+2);
    for (int i=0;i< Quantity_of_regs;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        reg[i] = IO_read_in_reg(Starting_Address+i);
    }
    return PDU;
}
//-------------------------------------------------------------
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








modbus_func funcs[] =
{
    0,                              //0 reserved
    ModBus_Read_Coils, 	            //1 Чтение значений нескольких регистров флагов
    ModBus_Read_Discrete_Inputs,    //2 Чтение значений нескольких дискретных входов
    ModBus_Read_Holding_Registers,  //3	Чтение значений нескольких регистров хранения
    ModBus_Read_Input_Registers,    //4 Чтение значений нескольких регистров ввода
    ModBus_Write_Single_Coil,       //5	Запись одного регистра флагов
    ModBus_Write_Single_Register,   //6	Запись одного регистра (ввода или хранения)
    0,                              //7 reserved
    0,                              //8 reserved
    0,                              //9 reserved
    0,                              //10 reserved
    0,                              //11 reserved
    0,                              //12 reserved
    0,                              //13 reserved
    0,                              //14 reserved
    ModBus_Write_Multiple_Coils,    //15 Запись нескольких регистров флагов
    ModBus_Write_Multiple_Register, //16 Запись нескольких регистров (ввода или хранения) 
};
