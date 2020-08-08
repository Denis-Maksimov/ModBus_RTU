#############################################################

TARGET:= ./run

##############################################################
# Исходники
##############################################################

SRC := main.c
SRC += master.c
SRC += modbus_general.c
SRC += slave.c
SRC += crc16.c



INC := inc/

##############################################################
# GTK+
##############################################################

#CFLAGS+= `pkg-config --cflags --libs gtk+-3.0`


#LIBS:= -lgtk-3 
#LIBS+= -lgobject-2.0
#LIBS+= -lcairo 
#LIBS+= -lcairo-gobject 
#LIBS+= -lgdk-3 
#LIBS+= -lpangocairo-1.0 
#LIBS+= -lpango-1.0 
#LIBS+= -latk-1.0 
#LIBS+= -lgdk_pixbuf-2.0
#LIBS+= -lgio-2.0
#LIBS+= -lglib-2.0
#LIBS+=-ldl 
#LIBS+=-lpthread
#LIBS+= -lm

##############################################################
#flags
CFLAGS:= -O2 -Wall -g $(addprefix -I,$(INC) )
#-g -rdynamic 

##############
# GTK+
##############
#CFLAGS+= -I/usr/include/glib-2.0 
#CFLAGS+= -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
#CFLAGS+= -I/usr/include/gtk-3.0
#CFLAGS+= -I/usr/include/pango-1.0
#CFLAGS+= -I/usr/include/cairo
#CFLAGS+= -I/usr/include/gdk-pixbuf-2.0
#CFLAGS+= -I/usr/include/atk-1.0
#CFLAGS+= -I/usr/include/at-spi2-atk/2.0
#CFLAGS+= -I/lib/x86_64-linux-gnu/glib-2.0/include

##############################################################




# для порядка в блобах:

OBJS :=$(SRC:%.c=obj/%.o)
#OBJDIRS:=$(dir $(OBJS))


#скрыть весь ужас
#.SILENT:




###############################################################################	
# Сборка исполняемого файла
###############################################################################
$(TARGET): $(OBJS)
	gcc $(OBJS) $(LIBS) -o run
	echo "Компиляция $(TARGET) завершена"
	size ./run
	
###############################################################################	
# Шаблон компиляции как из *.с получить obj/*.о 
###############################################################################		

#FIXME
#$(OBJDIRS):
#	mkdir -p $@ 

obj/%.o: %.c ./obj#$(OBJDIRS)
	gcc -c $<  $(CFLAGS) -o $@

./obj:
	mkdir ./obj
###############################################################################	
# Чистка
###############################################################################
clean:
	rm -frv ./obj

