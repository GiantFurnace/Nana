#author:chenzhengqiang
#generate date:2018/09/29 17:18:58


INCLUDE_DIR:=./include
SOURCE_DIR:=./src
LIB_DIR:=./lib

SUFFIX:=cpp
vpath %.h $(INCLUDE_DIR)
vpath %.$(SUFFIX) $(SOURCE_DIR)

TARGET:=nana
CC0:=g++
CC1:=g++
#define the optimize level of compiler
OLEVEL=0
LDCONFIG:=-lpthread
CFLAGS=-pg -g -W -Wall -Wextra -Wconversion -Wshadow -fPIC
OBJS:=main nana 
OBJS:=$(foreach obj,$(OBJS),$(obj).o)
LIB_OBJS=nana.o

INSTALL_DIR:=/usr/local/bin
CONFIG_PATH:=
SERVICE:=
CONFIG_INSTALL_PATH:=
TAR_NAME=$(TARGET)-$(shell date +%Y%m%d)

.PHONEY:clean
.PHONEY:install
.PHONEY:test
.PHONEY:tar
.PHONEY:library

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC1) -o $@ $^ $(LDCONFIG)
$(OBJS):%.o:%.$(SUFFIX)
	$(CC0) -o $@ -c $< -I$(INCLUDE_DIR) $(CFLAGS)

clean:
	-rm -f *.o *.a *.so *.log *core* $(TARGET) *.tar.gz *.cppe *.out

install:
	-mv $(TARGET) $(INSTALL_DIR)
	-cp -f $(SERVICE) /etc/init.d/$(TARGET)
	-rm -rf $(CONFIG_INSTALL_PATH)
	-mkdir $(CONFIG_INSTALL_PATH)
	-cp -f $(CONFIG_PATH)/* $(CONFIG_INSTALL_PATH)
library:
	$(CC1) -shared -o lib$(TARGET).so $(LIB_OBJS)
	-mv lib$(TARGET).so $(LIB_DIR)
test:
	./$(TARGET)
tar:
	tar -cvzf $(TAR_NAME).tar.gz .

