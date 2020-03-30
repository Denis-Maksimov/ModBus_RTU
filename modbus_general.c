#include "crc16.h"
#include "modbus_general.h"

#define MY_ADDRESS 4


//-- упаковать данные в сырой пакет 
struct raw_packet* pack_data(int destination_addres, char* data, int n)
{
    struct raw_packet* ret=malloc(sizeof(struct raw_packet));
     //-- prepares

    //
    ret->packet=(char*)malloc(n+3);
    //--pack address destination
    ret->packet[0]=destination_addres;

    //--pack data (PDU)
    memcpy(ret->packet+1,data,n);

    //--pack CRC16
    ret->packet[n+1]=(char)((CRC16(data,n)&0xff00)>>8);
    ret->packet[n+2]=(char)(CRC16(data,n)&0xff);
    ret->n=n+1+2;//+ 1 byte addres + 2 byte crc

    return ret;
    
}

//-- распаковать данные 
char* unpack_data(unsigned char* data, int n)
{
    
    //-- check address
    if (data[0]!=MY_ADDRESS){
        return 0;
    }
    data+=1;
    printf("address ok!\n");

    //-- check crc16 
    short crc=(data[n-3]<<8)|data[n-2];
    if(crc!=CRC16(data,n-3))
    {
        printf("data=%s\n",data);
        printf("error crc, 0x%X != 0x%X",crc,CRC16(data,n-3));
        return 0;
    }
    printf("CRC ok!\n");
    char* ret = (char*)malloc(n-3);
    memcpy(ret,data,n-3);
    printf("data=%s\n",ret);
    return ret;
}

void free_raw(struct raw_packet* raw)
{
    free(raw->packet);
    free(raw);
}
