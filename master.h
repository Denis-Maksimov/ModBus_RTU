#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

extern void master_ModBus_Read_Coils(char destination_addres, short Starting_Address, short Quantity_of_coils);
extern void master_ModBus_Read_Discrete_Inputs(char slave_addr, short Starting_Address, short Quantity_of_coils);

#endif // !MODBUS_MASTER_H