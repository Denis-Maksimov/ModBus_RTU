


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

//#include "master.h"

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
    printf("ModBus_Read_Coils\n");
    unsigned short* _args =(unsigned short*) args;
    unsigned short Starting_Address=endian_word(_args[0])&0xFFFF;
    unsigned short Quantity_of_coils=endian_word(_args[1])&0xFFFF;
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    unsigned int _n=0;
    unsigned int bytes=0;
    (Quantity_of_coils&0b111)?(bytes=(Quantity_of_coils>>3)+1):(bytes=(Quantity_of_coils>>3));
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_COILS;
    PDU->packet[1]=bytes;
    unsigned int count_of_coils = 0;

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
    printf("ModBus_Read_Discrete_Inputs\n");
    unsigned short* _args =(unsigned short*) args;
    unsigned short Starting_Address=endian_word(_args[0]);
    unsigned short Quantity_of_coils=endian_word(_args[1]);

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    unsigned int _n=0;
    unsigned int bytes=0;
    (Quantity_of_coils&0b111)?(bytes=(Quantity_of_coils>>3)+1):(bytes=(Quantity_of_coils>>3));
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
unsigned short IO_read_reg(unsigned short Starting_Address)
{
    //TODO!!!
    return 0x1234;
}

struct raw_packet* ModBus_Read_Holding_Registers(void* args)
{   
    printf("ModBus_Read_Holding_Registers\n");
    unsigned short* _args = (unsigned short*)args;
    unsigned short Starting_Address=endian_word(_args[0]);
    unsigned short Quantity_of_regs=endian_word(_args[1]);

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    unsigned int _n=0;
    unsigned int bytes = Quantity_of_regs*2;
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_HOLDING_REGISTERS;
    PDU->packet[1]=bytes;

    //-- Заполняем значения регистров
    unsigned short* reg=(unsigned short*)((PDU->packet)+2);
    for (unsigned short i=0;i< Quantity_of_regs;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        reg[i] = endian_word(IO_read_reg(Starting_Address+i));
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
    printf("ModBus_Read_Input_Registers\n");
    unsigned short* _args = args;
    unsigned short Starting_Address=endian_word(_args[0]);
    unsigned short Quantity_of_regs=endian_word(_args[1]);

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    unsigned int _n=0;
    unsigned int bytes = Quantity_of_regs*2;
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_INPUT_REGISTERS;
    PDU->packet[1]=bytes;

    //-- Заполняем значения регистров
    unsigned short* reg=(unsigned short*)((PDU->packet)+2);
    for (unsigned int i=0;i< Quantity_of_regs;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        reg[i] = endian_word(IO_read_in_reg(Starting_Address+i));
    }
    return PDU;
}
//-------------------------------------------------------------

short IO_write_coil(short Address,short Value)
{
    //TODO!!!
    return Value;
}
struct raw_packet* ModBus_Write_Single_Coil(void* args)
{
    printf("ModBus_Write_Single_Coil\n");
    unsigned short* _args = (unsigned short*)args;
    unsigned short Address=endian_word(_args[0]);
    unsigned short Value=endian_word(_args[1]);

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    unsigned int _n=0;

    _n += 1; //code func
    _n += 2; //Address
    _n += 2; //Value

    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_WRITE_SINGLE_COIL;
    PDU->packet[1]=Address;


    //-- Заполняем значения регистров
    unsigned short* reg=(unsigned short*)((PDU->packet)+1);
    reg[0] = endian_word(Address);
    reg[1] = endian_word(IO_write_coil(Address,Value));

    return PDU;
}
//--------------------------------------------------------------
unsigned short IO_write_reg(unsigned short Address, unsigned short Value)
{
    //TODO!!!
    return Value;
}
struct raw_packet* ModBus_Write_Single_Register(void* args)
{
    printf("ModBus_Write_Single_Register\n");
    unsigned short* _args = args;
    unsigned short Address=endian_word(_args[0]);
    unsigned short Value=endian_word(_args[1]);

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    unsigned int _n=0;

    _n += 1; //code func
    _n += 2; //Address
    _n += 2; //Value

    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_WRITE_SINGLE_REGISTER;
    PDU->packet[1]=Address;
 

    //-- Заполняем значения регистров
    unsigned short* reg=(unsigned short*)((PDU->packet)+1);
    reg[0] = endian_word(Address);
    reg[1] = endian_word(IO_write_reg(Address,Value));

    return PDU;

}
//--------------------------------------------------------------
void IO_write_multi_coils(unsigned short start_Address,unsigned short Quantity_of_Outputs,unsigned char byte_count,unsigned char* Outputs_Value)
{
    for (size_t i = 0; i < Quantity_of_Outputs; ++i)
    {
       
        IO_write_coil(start_Address,Outputs_Value[i-1]);
        
        (i&7)?(start_Address):(start_Address=+1);
    }
    
}
struct raw_packet* ModBus_Write_Multiple_Coils(void* args)
{
    printf("ModBus_Write_Multiple_Coils\n");
    // |0 1|2 3|4|5 6
    // |0  |1  |2 
    unsigned short* _word    = (unsigned short*)args;
    unsigned char* _bytes    = (unsigned char*) args;
    //---------------------------------
    unsigned short start_Address   =  endian_word(_word[0]);
    unsigned short Quantity_of_Outputs =  endian_word(_word[1]);
    
    unsigned char byte_count =  _bytes[4];

    unsigned char* Outputs_Value = (unsigned char*)malloc(byte_count);
    _bytes+=5; //offset
    for (size_t i = 0; i < byte_count; i++)
    {
        Outputs_Value[i]=_bytes[i];
    }
    //---------------------------------
    IO_write_multi_coils(start_Address, Quantity_of_Outputs, byte_count, Outputs_Value);

    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));

    PDU->n=1+2+2+1+byte_count;
    PDU->packet= malloc(PDU->n);

    PDU->packet[0]=MODBUS_WRITE_MULTIPLE_COILS;
    unsigned short* ptr_tmp=(unsigned short*)(PDU->packet+1);
    ptr_tmp[0]=endian_word(start_Address);
    ptr_tmp[1]=endian_word(Quantity_of_Outputs);
    PDU->packet[5]=byte_count;
    for(int i=6;i<byte_count+6;i++)// #offset=6
    {
        PDU->packet[i]=Outputs_Value[i-6];
    }
    free(Outputs_Value);
    return PDU;
}

struct raw_packet* ModBus_Write_Multiple_Register(void*a)
{
    //TODO
    return (struct raw_packet*)MODBUS_WRITE_MULTIPLE_REGISTER;
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
    0,                              //7 Read Exception Status (Serial Line only)
    0,                              //8 Diagnostics (Serial Line only)
    0,                              //9 reserved
    0,                              //10 reserved
    0,                              //11 Get Comm Event Counter(Serial Line only)
    0,                              //12 Get Comm Event Log (Serial Line only)
    0,                              //13 reserved
    0,                              //14 reserved
    ModBus_Write_Multiple_Coils,    //15 Запись нескольких регистров флагов
    ModBus_Write_Multiple_Register, //16 Запись нескольких регистров (ввода или хранения) 
                                    //17 Report ServerID (Serial Line only)
                                    //20 Read File Record
                                    //21 Write File Record
                                    //22 Mask Write Register
                                    //23 Read/Write Multiple registers
                                    //24 Read FIFO Queue
                                    //43 Encapsulated Interface Transport
                                    //43/13 CANopen General Reference Request and Response PDU
                                    //43 / 14 (0x2B / 0x0E) Read Device Identification

};


struct raw_packet* slave_receive(struct raw_packet* raw)
{
    unsigned char* PDU=unpack_data(raw->packet, raw->n);
    unsigned int n=PDU[0]&0xff;
    if(funcs[n])
    {
        return funcs[n](PDU+1);
    }
    return 0;
}


