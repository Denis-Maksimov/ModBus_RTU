#ifndef MB_SLAVE_IFCE
#define MB_SLAVE_IFCE


//-- здесь задаём размеры регистров

#define MODBUS_MAX_COILS       20  //rw
#define MODBUS_MAX_DIGITS      20  //ro
#define MODBUS_MAX_HOLDING     20  //rw
#define MODBUS_MAX_REG         20  //r


/*********************************************************************
** Через регистры ниже ведомое устройство общается с мастером смены **
**********************************************************************/
 
// биты
short coils[MODBUS_MAX_COILS]={0};                 //rw
short discrete_inputs[MODBUS_MAX_DIGITS]={0};      //ro

// регистры
short holding_registers[MODBUS_MAX_HOLDING]={0};   //rw
short ro_registers[MODBUS_MAX_REG]={0};            //ro




#endif
