#ifndef MODBUS_GENERAL_H
#define MODBUS_GENERAL_H

struct raw_packet{
    
    unsigned char* packet;   // сырые данные пакета (с CRC16 и шлюхами)
    unsigned int n;          // число байт в телеграмме
};
typedef struct raw_packet tADU;
typedef struct raw_packet tPDU;

//-- pack data
extern struct raw_packet* pack_data(unsigned int destination_addres, unsigned char* data, unsigned int n);

//-- unpack data
extern unsigned char* unpack_data(unsigned char* data, unsigned int n);

//-- kill struct raw_packet
extern void free_raw(struct raw_packet* raw);

//-- print in stdout packet in hex mode
extern void print_raw(struct raw_packet* raw);// __attribute__((deprecated));

//-- Изменить порядок байт в слове
extern unsigned short endian_word(unsigned short word);

// #include <cdefs.h>
struct MB_Read_header{
    unsigned char bytes;
    unsigned char* data;
};

struct MB_Write_header{
    // unsigned char bytes;
    unsigned short start_addr;
    unsigned short n;
};
// __attribute__((pack);

typedef struct MB_Read_header MB_Read_Coils_hdr;
typedef struct MB_Read_header MB_Read_DI_hdr;
typedef struct MB_Read_header MB_Read_Hold_Reg_hdr;
typedef struct MB_Read_header MB_Read_Input_Reg_hdr;

typedef struct MB_Write_header MB_Write_Single_Coil_hdr;
typedef struct MB_Write_header MB_Write_Single_Reg_hdr;
typedef struct MB_Write_header MB_Write_Multi_Coils_hdr;
typedef struct MB_Write_header MB_Write_Multi_Regs_hdr;


#endif // !MODBUS_GENERAL_H
