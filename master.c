/**
 * ModBus RTU
 * 
 * 
*/
#include "crc16.h"
#include "modbus_general.h"



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
static struct raw_packet* _ModBus_Read_Coils(short Starting_Address, short Quantity_of_coils)
{       
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_COILS;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Starting_Address);
    ptr_tmp[1]=endian_word(Quantity_of_coils);
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
static struct raw_packet* _ModBus_Read_Discrete_Inputs(short Starting_Address, short Quantity_of_inputs)
{
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_DISCRETE_INPUTS;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Starting_Address);
    ptr_tmp[1]=endian_word(Quantity_of_inputs);
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
static struct raw_packet* _ModBus_Read_Holding_Registers(short Starting_Address, short Quantity_of_regs)
{
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_HOLDING_REGISTERS;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Starting_Address);
    ptr_tmp[1]=endian_word(Quantity_of_regs);
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
static struct raw_packet* _ModBus_Read_Input_Registers(short Starting_Address, short Quantity_of_regs)
{
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_READ_INPUT_REGISTERS;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Starting_Address);
    ptr_tmp[1]=endian_word(Quantity_of_regs);
    return PDU;
}


/***********************************************************************************************************
 * 05 (0x05) Write Single CoilThis function code is used to write a single output to either ON or OFF in a remote device.
 * The requested ON/OFF state is specified by a constant in the request data field. A value of FF 00  hex requests the output to be ON. 
 * A  value of 00 00 requests it to  be OFF. All other  values are illegal and will not affect the output.
 * The  Request  PDU  specifies  the  address  of  the  coil  to  be  forced.  Coils  are  addressed  starting at  zero.  
 * Therefore  coil  numbered  1  is  addressed  as  0.  
 * The  requested  ON/OFF  state  is specified  by  a  constant  in  the  Coil  Value  field.  
 * A  value  of  0XFF00  requests  the  coil  to  be  ON. A valueof 0X0000 requests the coil to be off. 
 * All other values are illegal and will not affect the coil.
 * 
 *-----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции чтения n регистров с устройства
 * ***********************************************************************************************************/
static struct raw_packet* _ModBus_Write_Single_Coil(short Address, short Value)
{
    (Value)?(Value=0xFF00):(Value=0);
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_WRITE_SINGLE_COIL;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Address);
    ptr_tmp[1]=endian_word(Value);
    return PDU;
}
/***********************************************************************************************************
 * 06 (0x06) Write Single Register
 * 
 * This function code is used to write a single holding register in a remote device.
 * The Request PDU specifies the address of the register to be written. 
 * Registers are addressed starting at zero. 
 * Therefore register numbered 1 is addressed as 0.
 * The normal response is an echo of the request, 
 * returned after the register contents have been written.
 * 
 *-----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции записи регистра в устройстве
 * ***********************************************************************************************************/
static struct raw_packet* _ModBus_Write_Single_Register(short Address, short Value)
{
   // (Value)?(Value=0xFF00):(Value=0);
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2;
    PDU->packet= malloc(5);

    PDU->packet[0]=MODBUS_WRITE_SINGLE_REGISTER;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Address);
    ptr_tmp[1]=endian_word(Value);
    return PDU;
}
/***********************************************************************************************************
 * 15 (0x0F) Write Multiple Coils
 * 
 * This  function  code  is  used  to  force  each  coil  in  a  sequence  of  coils to  either  ON  or  OFF in  a remote  device.  
 * The  Request  PDU  specifies  the  coil  references  to  be  forced.  Coils  are addressed starting at zero. 
 * Therefore coil  numbered 1 is addressed as 0.
 * The requested ON/OFF  states  are specified  by contents of the request data field.  
 * A logical '1' in a bit position of the field requests the corresponding output to be ON. 
 * A logical '0' requests it to be OFF.
 * The normal response returns the function code, starting address, and quantity of coils forced.
 * 
 *-----------------------------------------------------------------------------------------------------------
 * готовит фрейм PDU для функции груповой записи флагов в устройстве
 * ***********************************************************************************************************/
static struct raw_packet* _ModBus_Write_Multiple_Coils(short Starting_Address, short Quantity_of_Outputs, char byte_count, char* Outputs_Value)
{
    // //-- считаем число байтов
    // int byte_count=0;
    // (Quantity_of_Outputs&0b111)?(byte_count=(Quantity_of_Outputs>>3)+1):(byte_count=(Quantity_of_Outputs>>3));
    struct raw_packet* PDU = malloc(sizeof(struct raw_packet));
    PDU->n=1+2+2+1+byte_count;
    PDU->packet= malloc(PDU->n);

    PDU->packet[0]=MODBUS_WRITE_MULTIPLE_COILS;
    short* ptr_tmp=(short*)((PDU->packet)+1);
    ptr_tmp[0]=endian_word(Starting_Address);
    ptr_tmp[1]=endian_word(Quantity_of_Outputs);
    PDU->packet[5]=byte_count;
    for(int i=6;i<byte_count+6;i++)// #offset=6
    {
        PDU->packet[i]=Outputs_Value[i-6];
    }
    return PDU;
}

static struct raw_packet* _ModBus_Write_Multiple_Register(void*a)
{
    //TODO
    return (struct raw_packet*)MODBUS_WRITE_MULTIPLE_REGISTER;
}



//----------------------------------------------------------------
static void assertion(struct raw_packet* raw){
    printf("raw: "); print_raw(raw);
    char* s=unpack_data(raw->packet, raw->n);
    printf("unpacking PDU: ");
    for(int i=0;i<raw->n-3;i++)
    {
        (i)?(printf("-")):(i);
        printf("0x%02X",s[i]&0xff);
        fflush(stdout);
    }
    free(s);
    printf("\n");
}
//----------------------------------------------------------------

void master_ModBus_Read_Coils(char destination_addres, short Starting_Address, short Quantity_of_coils)
{
    struct raw_packet* PDU = _ModBus_Read_Coils(Starting_Address, Quantity_of_coils);
    struct raw_packet* raw = pack_data(destination_addres, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);

    free_raw(raw);
    free_raw(PDU);
    
}

//----------------------------------------------------------------

void master_ModBus_Read_Discrete_Inputs(char slave_addr, short Starting_Address, short Quantity_of_coils)
{
    struct raw_packet* PDU = _ModBus_Read_Discrete_Inputs(Starting_Address, Quantity_of_coils);
    struct raw_packet* raw = pack_data(slave_addr, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);

    free_raw(raw);
    free_raw(PDU);

}

void master_ModBus_Read_Holding_Registers(char slave_addr, short Starting_Address, short Quantity_of_regs)
{
    struct raw_packet* PDU = _ModBus_Read_Holding_Registers(Starting_Address, Quantity_of_regs);
    struct raw_packet* raw = pack_data(slave_addr, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);
    
    free_raw(raw);
    free_raw(PDU);

}

void master_ModBus_Read_Input_Registers(char slave_addr, short Starting_Address, short Quantity_of_regs)
{
    struct raw_packet* PDU = _ModBus_Read_Input_Registers(Starting_Address, Quantity_of_regs);
    struct raw_packet* raw = pack_data(slave_addr, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);
    
    free_raw(raw);
    free_raw(PDU);

}

void master_ModBus_Write_Single_Coil(char slave_addr, short Address, short Value)
{
    struct raw_packet* PDU = _ModBus_Write_Single_Coil(Address, Value);
    struct raw_packet* raw = pack_data(slave_addr, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);
    
    free_raw(raw);
    free_raw(PDU);

}

void master_ModBus_Write_Single_Register(char slave_addr, short Address, short Value)
{
    struct raw_packet* PDU = _ModBus_Write_Single_Register(Address, Value);
    struct raw_packet* raw = pack_data(slave_addr, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);
    
    free_raw(raw);
    free_raw(PDU);

}

void master_ModBus_Write_Multiple_Coils(char slave_addr, short Starting_Address,short Quantity_of_Outputs, char byte_count, char* Outputs_Value)
{
    struct raw_packet* PDU = _ModBus_Write_Multiple_Coils(Starting_Address, Quantity_of_Outputs,byte_count,Outputs_Value);
    struct raw_packet* raw = pack_data(slave_addr, PDU->packet, PDU->n);
    // send( rs485, raw );
    printf("PDU: ");print_raw(PDU);
    assertion(raw);
    
    free_raw(raw);
    free_raw(PDU);

}

// static struct raw_packet* _ModBus_Write_Multiple_Coils(short Starting_Address, short Quantity_of_Outputs, char byte_count, char* Outputs_Value)
// static struct raw_packet* _ModBus_Write_Single_Register(short Address, short Value)
// _ModBus_Write_Single_Coil(short Address, short Value)
// static struct raw_packet* _ModBus_Read_Input_Registers(short Starting_Address, short Quantity_of_regs)
//struct raw_packet* _ModBus_Read_Holding_Registers(short Starting_Address, short Quantity_of_regs)
