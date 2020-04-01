#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

extern void master_ModBus_Read_Coils(unsigned char destination_addres, unsigned short Starting_Address, unsigned short Quantity_of_coils);
extern void master_ModBus_Read_Discrete_Inputs(unsigned char slave_addr,unsigned short Starting_Address,unsigned short Quantity_of_coils);
extern void master_ModBus_Read_Holding_Registers(unsigned char slave_addr,unsigned short Starting_Address, unsigned short Quantity_of_coils);
extern void master_ModBus_Read_Input_Registers(unsigned char slave_addr,unsigned short Starting_Address,unsigned short Quantity_of_regs);

extern void master_ModBus_Write_Single_Coil(unsigned char slave_addr,unsigned short Address, unsigned short Value);
extern void master_ModBus_Write_Single_Register(unsigned char slave_addr,unsigned short Address,unsigned short Value);
extern void master_ModBus_Write_Multiple_Coils(unsigned char slave_addr,unsigned short Starting_Address,unsigned short Quantity_of_Outputs,unsigned char byte_count, unsigned char* Outputs_Value);

#endif // !MODBUS_MASTER_H
