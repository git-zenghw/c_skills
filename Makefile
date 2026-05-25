
# 命令
CC := $(CROSS_COMPILE)gcc
RM:=rm -rf
CP:=cp -r
MKDIR:=mkdir -p

# 后缀
C_SUFFIX := c
OBJ_SUFFIX := o

# 当前目录
TOP_DIR := .
APP_DIR := $(TOP_DIR)/app
PRT_DIR := $(TOP_DIR)/prt
DRV_DIR	:= $(TOP_DIR)/drv
OPS_DIR := $(TOP_DIR)/ops
STA_DIR := $(TOP_DIR)/state
LIST_DIR := $(TOP_DIR)/list
PROJECT_DIR := . $(OPS_DIR) $(STA_DIR) $(LIST_DIR) $(APP_DIR) $(PRT_DIR) $(DRV_DIR)

# c源文件
C_SRC_DIR := $(PROJECT_DIR)
C_SRC := $(foreach dir, $(C_SRC_DIR), $(wildcard ${dir}/*.$(C_SUFFIX)))
vpath %.$(C_SUFFIX) $(C_SRC_DIR)

# 去路径
NOT_DIR_C_SRC := $(notdir $(C_SRC))
EXCLUDE_SRC := 

C_SRC := $(filter-out $(EXCLUDE_SRC), $(NOT_DIR_C_SRC))

# obj文件
OBJS_DIR := $(TOP_DIR)/objs
C_BUILD_OBJS := $(patsubst %.$(C_SUFFIX), $(OBJS_DIR)/%.$(OBJ_SUFFIX), $(C_SRC))

# 头文件
INC_DIR := -I. \
	   -I$(OPS_DIR) \
	   -I$(LIST_DIR) \
	   -I$(STA_DIR) \
	   -I$(APP_DIR) \
	   -I$(PRT_DIR) \
	   -I$(DRV_DIR)

C_FLAGS += -fpic -Wall

# 库
LIBS :=  -lpthread -lrt
LIBS_DIR := -L$(TOP_DIR) 

.PHONY: all  clean 
TARGET := c_skills

all: $(TARGET)
objs: $(C_BUILD_OBJS)
$(TARGET): $(C_BUILD_OBJS) 
	@echo 'linking $@'
	$(CC) $(C_FLAGS) -o $@ $^ $(LIBS) $(LIBS_DIR) $(LLFLAGS)
	@echo 'finish linking $@'
	@echo ' '

$(C_BUILD_OBJS): $(OBJS_DIR)/%.$(OBJ_SUFFIX) : $(TOP_DIR)/%.$(C_SUFFIX)
	$(MKDIR) $(OBJS_DIR)
	@echo 'building: $<'
	$(CC) -c $< -o $@ $(C_FLAGS) ${INC_DIR}
	@echo 'finish building: $@'
	@echo ' '

clean:
	-$(RM)  $(TARGET) $(C_BUILD_OBJS) 
	-$(RM)  ${OBJS_DIR}
	@echo ' '

