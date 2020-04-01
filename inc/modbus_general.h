#ifndef MODBUS_GENERAL_H
#define MODBUS_GENERAL_H

struct raw_packet{
    unsigned int n;          // число байт в телеграмме
    unsigned char* packet;   // сырые данные пакета (с CRC16 и шлюхами)
};

//-- pack data
extern struct raw_packet* pack_data(unsigned int destination_addres, unsigned char* data, unsigned int n);

//-- unpack data
extern unsigned char* unpack_data(unsigned char* data, unsigned int n);

//-- kill struct raw_packet
extern void free_raw(struct raw_packet* raw);

//-- print in stdout packet in hex mode
extern void print_raw(struct raw_packet* raw);

//-- Изменить порядок байт в слове
extern unsigned short endian_word(unsigned short word);

#endif // !MODBUS_GENERAL_H
