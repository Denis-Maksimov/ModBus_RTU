


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

#include "slave.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define OWN_ADDRESS 4

#define c_new(t)     ((t*)malloc(sizeof(t)))
#define c_new_n(t,n)     ((t*)malloc(sizeof(t)*n))


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
//----------------------------------------------
#define MAX_COILS       20  //rw
#define MAX_DIGITS      20  //ro
#define MAX_HOLDING     20  //rw
#define MAX_REG         20  //r

short coils[MAX_COILS]={0};                 //rw
short discrete_inputs[MAX_DIGITS]={0};      //ro
short holding_registers[MAX_HOLDING]={0};   //rw
short ro_registers[MAX_REG]={0};            //ro
//-------------- read ----------------------

//считать софтовый бит по адресу как битбэнд
char IO_read_coil(short Address)
{
    //TODO!!!
    unsigned int start_byte=Address/16;//начальный байт
    unsigned short start_bit=Address%16;

    if(start_byte>=MAX_COILS)return 0;

    if(coils[start_byte]&(0x01<<start_bit))
        return 0x01;
    else
        return 0x00;
    
}

//считать железный бит по адресу как битбэнд
char IO_read_Discrete_Inputs(short Starting_Address)
{
    //TODO!!!
    unsigned int start_byte=Starting_Address/16;//начальный байт
    unsigned short start_bit=Starting_Address%16;
    if(start_byte>=MAX_DIGITS)return 0;
    if(discrete_inputs[start_byte]&(0x01<<start_bit))
        return 0x01;
    else
        return 0x00;
    
}

//прочитать софтовое слово в 2 байта по адресу
unsigned short IO_read_hold_reg(unsigned short Starting_Address)
{
    //TODO!!!
    if(Starting_Address>=MAX_HOLDING)return 0;
    return holding_registers[Starting_Address];
}

//прочитать хардварное слово в 2 байта по адресу
short IO_read_in_reg(short Starting_Address)
{
    //TODO!!!
    if(Starting_Address>=MAX_REG)return 0;
    return ro_registers[Starting_Address];
}
//---------------- write -----------------------





int IO_write_multi_coils(  unsigned short start_bit,       //адрес начала записи (побитовое смещение)
                            unsigned short Quantity_of_Outputs, //число битов для записи
                            unsigned short* Outputs_Values,       //значения для записи
                            unsigned char size_of_Values){        /*sizeof(Outputs_Values)*/

    
    int success = 0;
    int start_byte=start_bit/16;//начальный байт
    start_bit=start_bit%16;

    for (size_t i = 0; i < Quantity_of_Outputs; i++)
    {
       
        // IO_write_coil(start_Address,Outputs_Value[i]);
        if(start_byte+i/16>=MAX_COILS)return success;
        

        if(Outputs_Values[i/16]&(1<<(i%16))){
            coils[start_byte+i/16]|=(1<<((i+start_bit)%16));
        }else{
            coils[start_byte+i/16]&=~(1<<((i+start_bit)%16));
        }
        success++;
        
    }
    return success;
    
}
short IO_write_coil(short Address,short Value)
{
    int start_byte=Address/16;//начальный байт
    short start_bit=Address%16;      //начальный бит
    if(start_byte>=MAX_COILS)return 0;
    if(Value){
            coils[start_byte]|=(1<<start_bit);
        }else{
            coils[start_byte]&=~(1<<start_bit);
        }
    return Value;
}


unsigned short IO_write_reg(unsigned short Address, unsigned short Value)
{
    //TODO!!!
    if(Address>=MAX_REG)return 0;
    holding_registers[Address]=Value;
    return Value;
}

#undef MAX_COILS
#undef MAX_DIGITS
#undef MAX_HOLDING 
#undef MAX_REG 
//--------------------------------------------------------------------------------

static tADU* ModBus_Read_Coils(void* args)
{   
    printf("ModBus_Read_Coils\n");
    unsigned short* _args =(unsigned short*) args;
    unsigned short Starting_Address=endian_word(_args[0])&0xFFFF;
    unsigned short Quantity_of_coils=endian_word(_args[1])&0xFFFF;
    tPDU* PDU = c_new(tPDU);
    unsigned int _n=0;
    unsigned int bytes=0;

    //превращаем биты в байты
    (Quantity_of_coils&0b111)?(bytes=(Quantity_of_coils>>3)+1):(bytes=(Quantity_of_coils>>3));
    
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils

    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_COILS;
    PDU->packet[1]=bytes;
    unsigned int count_of_coils = 0;//iterator
    tADU* ADU;
    //-- Заполняем значения катушек (флагов, битов)
    for (int i=2;i< bytes+2;i++) //+2 это смещение для кода функции и кол-ва байтов
    {//для каждого байта
        //заполняем биты
        for(int b=0;b<8;b++){
            
            PDU->packet[i] |= (IO_read_coil(Starting_Address)<<b);
            Starting_Address++;
            count_of_coils++;
            if(count_of_coils==Quantity_of_coils) 
                {
                    goto l_ModBus_one;//выход из двойного цикла
                }
        }
        
    }

l_ModBus_one:
    ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;
}
//------------------------------------

// 
static tADU* ModBus_Read_Discrete_Inputs(void* args)
{   
    printf("ModBus_Read_Discrete_Inputs\n");
    unsigned short* _args =(unsigned short*) args;
    unsigned short Starting_Address=endian_word(_args[0]);
    unsigned short Quantity_of_coils=endian_word(_args[1]);

    tPDU* PDU = c_new(tPDU);
    unsigned int _n=0;
    unsigned int bytes=0;

    //превращаем биты в байты
    (Quantity_of_coils&0b111)?(bytes=(Quantity_of_coils>>3)+1):(bytes=(Quantity_of_coils>>3));
    
    _n += 1; //code func
    _n += 1; //Quantity_of_coils
    _n += bytes; //status coils
    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_READ_DISCRETE_INPUTS;
    PDU->packet[1]=bytes;
    int count_of_coils = 0;// итератор
    tADU* ADU;
    //-- Заполняем значения катушек (флагов, битов)
    for (int i=2;i< bytes+2;i++) //+2 это смещение для кода функции и кол-ва байтов
    {//для каждого байта
        //заполняем биты
        for(int b=0;b<8;b++){
            
            PDU->packet[i] |= (IO_read_Discrete_Inputs(Starting_Address)<<b);
            Starting_Address++;
            count_of_coils++;
            if(count_of_coils==Quantity_of_coils) goto l_ModBus2;
        }
        
    }
l_ModBus2:
    ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;
}
//------------------------------------

static tADU* ModBus_Read_Holding_Registers(void* args)
{   
    printf("ModBus_Read_Holding_Registers\n");
    unsigned short* _args = (unsigned short*)args;
    unsigned short Starting_Address=endian_word(_args[0]);
    unsigned short Quantity_of_regs=endian_word(_args[1]);

    tPDU* PDU = c_new(tPDU);
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
    unsigned short* reg=(unsigned short*)(&PDU->packet[2]);
    for (unsigned short i=0;i< Quantity_of_regs;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        reg[i] = endian_word(IO_read_hold_reg(Starting_Address+i));
    }
    tADU* ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;
}


//---------------------------------------------------------


static tADU* ModBus_Read_Input_Registers(void* args)
{   
    printf("ModBus_Read_Input_Registers\n");
    unsigned short* _args = args;
    unsigned short Starting_Address=endian_word(_args[0]);
    unsigned short Quantity_of_regs=endian_word(_args[1]);

    tPDU* PDU = c_new(tPDU);
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
    unsigned short* reg=(unsigned short*)(&PDU->packet[2]);
    for (unsigned int i=0;i< Quantity_of_regs;i++) //+2 это смещение для кода функции и кол-ва байтов
    {
        reg[i] = endian_word(IO_read_in_reg(Starting_Address+i));
    }
    tADU* ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;
}
//-------------------------------------------------------------


static tADU* ModBus_Write_Single_Coil(void* args)
{
    printf("ModBus_Write_Single_Coil\n");
    unsigned short* _args = (unsigned short*)args;
    unsigned short Address=endian_word(_args[0]);
    unsigned short Value=endian_word(_args[1]);

    tPDU* PDU = c_new(tPDU);
    unsigned int _n=0;

    _n += 1; //code func
    _n += 2; //Address
    _n += 2; //Value

    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_WRITE_SINGLE_COIL;
    PDU->packet[1]=Address;


    //-- Заполняем значения регистров
    unsigned short* reg=(unsigned short*)(&PDU->packet[2]);
    reg[0] = endian_word(Address);
    reg[1] = endian_word(IO_write_coil(Address,Value));

    tADU* ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;
}
//--------------------------------------------------------------

static tADU* ModBus_Write_Single_Register(void* args)
{
    printf("ModBus_Write_Single_Register\n");
    unsigned short* _args = args;
    unsigned short Address=endian_word(_args[0]);
    unsigned short Value=endian_word(_args[1]);

    tPDU* PDU = c_new(tPDU);
    unsigned int _n=0;

    _n += 1; //code func
    _n += 2; //Address
    _n += 2; //Value

    PDU->n=_n;
    PDU->packet= malloc(_n);
    PDU->packet[0]=MODBUS_WRITE_SINGLE_REGISTER;
    PDU->packet[1]=Address;
 

    //-- Заполняем значения регистров
    unsigned short* reg=(unsigned short*)(&PDU->packet[2]);
    reg[0] = endian_word(Address);
    reg[1] = endian_word(IO_write_reg(Address,Value));

    tADU* ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;

}
//--------------------------------------------------------------

static tADU* ModBus_Write_Multiple_Coils(void* args)
{
    printf("ModBus_Write_Multiple_Coils\n");
    // |0 1|2 3|4|5 6
    // |0  |1  |2 
    unsigned short* _word    = (unsigned short*)args;
    unsigned char* _bytes    = (unsigned char*) args;
    //---------------------------------
    unsigned short start_Address   =  endian_word(_word[0]);
    unsigned short Quantity_of_Outputs =  endian_word(_word[1]);
    
    unsigned char byte_count =  _bytes[4]; //sizeof(Outputs_Value)

    unsigned char* Outputs_Value = c_new_n(unsigned char,byte_count);
    _bytes+=5; //offset
    for (size_t i = 0; i < byte_count; i++)
    {
        Outputs_Value[i]=_bytes[i];
    }
    //---------------------------------
    int succes=IO_write_multi_coils(start_Address, Quantity_of_Outputs,   (unsigned short*)Outputs_Value,byte_count);

    tPDU* PDU = c_new(tPDU);

    PDU->n= 1   //code func
            +2  //start address
            +2;  //количество записанных бит

    PDU->packet= malloc(PDU->n);

    PDU->packet[0]=MODBUS_WRITE_MULTIPLE_COILS;
    unsigned short* ptr_tmp=(unsigned short*)(PDU->packet+1);
    ptr_tmp[0]=endian_word(start_Address);
    ptr_tmp[1]=endian_word(succes);
    // PDU->packet[5]=byte_count;
    // for(int i=6;i<byte_count+6;i++)// #offset=6
    // {
    //     PDU->packet[i]=Outputs_Value[i-6];
    // }
    free(Outputs_Value);

    tADU* ADU = pack_data(OWN_ADDRESS, PDU->packet, PDU->n);
    free_raw(PDU);
    return ADU;
}

static tADU* ModBus_Write_Multiple_Register(void*a)
{
    //TODO
    return (tPDU*)MODBUS_WRITE_MULTIPLE_REGISTER;
}

//==============================================================



//----------------------------------------------------------------






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


tADU* slave_receive(tADU* raw)
{
    unsigned char* PDU=unpack_data(raw->packet, raw->n);
    if(!PDU) return 0;
    unsigned int n=PDU[0]&0xff;
    if(funcs[n])
    {
        return funcs[n](PDU+1);
    }
    return 0;
}

#undef c_new   
#undef c_new_n
