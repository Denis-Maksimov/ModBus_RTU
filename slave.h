#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

typedef struct raw_packet*(*modbus_func)(void*);
extern struct raw_packet* slave_receive(struct raw_packet* raw);
extern modbus_func funcs[];

#endif //!MODBUS_SLAVE_H