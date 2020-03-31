#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

extern void master_ModBus_Read_Coils(char destination_addres, short Starting_Address, short Quantity_of_coils);
extern void master_ModBus_Read_Discrete_Inputs(char slave_addr, short Starting_Address, short Quantity_of_coils);
extern void master_ModBus_Read_Holding_Registers(char slave_addr, short Starting_Address, short Quantity_of_coils);
extern void master_ModBus_Read_Input_Registers(char slave_addr, short Starting_Address, short Quantity_of_regs);

extern void master_ModBus_Write_Single_Coil(char slave_addr, short Address, short Value);
extern void master_ModBus_Write_Single_Register(char slave_addr, short Address, short Value);
extern void master_ModBus_Write_Multiple_Coils(char slave_addr, short Starting_Address,short Quantity_of_Outputs, char byte_count, char* Outputs_Value);

#endif // !MODBUS_MASTER_H
