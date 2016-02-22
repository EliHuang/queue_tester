TOOLS_PREFIX :=

CPP_SRCS := \

C_SRCS := \
   queue.c \
   queue_test.c \

INC_PATH :=

CPP_OBJS :=  $(patsubst %.cpp,%.o,$(CPP_SRCS))

C_OBJS :=  $(patsubst %.c,%.o,$(C_SRCS))

DEPS := $(patsubst %.cpp,%.d,$(CPP_SRCS)) \
        $(patsubst %.c,%.d,$(C_SRCS)) \


LOCAL_CFLAGS :=
LIBRARY := -lpthread

BIN_DIR := .
BINARY_NAME := queue_test

.PHONY : all

all : $(BIN_DIR)/$(BINARY_NAME)
-include $(DEPS)

$(BIN_DIR)/$(BINARY_NAME) : $(C_OBJS) $(CPP_OBJS) $(LIBRARY)
	$(TOOLS_PREFIX)gcc $(LOCAL_CFLAGS) $(INC_PATH) -o $(BIN_DIR)/$(BINARY_NAME) $(C_OBJS) $(OBJS) $(LIBRARY)

$(CPP_OBJS) :%.o:%.cpp
	$(TOOLS_PREFIX)g++ -MM $(LOCAL_CFLAGS) $(INC_PATH) $< > $(patsubst %.o,%.d,$@)
	$(TOOLS_PREFIX)g++ $(LOCAL_CFLAGS) -c $(INC_PATH) -o $@ $< $(LOG_OPTION) 

$(C_OBJS) :%.o:%.c
	$(TOOLS_PREFIX)gcc -MM $(LOCAL_CFLAGS) $(INC_PATH) $< > $(patsubst %.o,%.d,$@)
	$(TOOLS_PREFIX)gcc $(LOCAL_CFLAGS) -c $(INC_PATH) -o $@ $< $(LOG_OPTION) 

clean:
	-rm -f $(C_OBJS)
	-rm -f $(CPP_OBJS)
	-rm -f $(DEPS)
	-rm -f $(BIN_DIR)/$(BINARY_NAME)
