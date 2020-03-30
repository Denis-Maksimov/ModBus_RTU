#ifndef MODBUS_GENERAL_H
#define MODBUS_GENERAL_H

struct raw_packet{
    int n;          // число байт в телеграмме
    char* packet;   // сырые данные пакета (с CRC16 и шлюхами)
};

//-- pack data
extern struct raw_packet* pack_data(int destination_addres, char* data, int n);

//-- unpack data
extern char* unpack_data(unsigned char* data, int n);

//-- kill struct raw_packet
extern void free_raw(struct raw_packet* raw);



#endif // !MODBUS_GENERAL_H
