# 定义编译器
CXX = g++

# 定义编译选项
CXXFLAGS = -g -Wall

# 定义源文件目录
SRC_DIR = .
MYSQL_DIR = ./mysql
SER_DIR = ./ser
THREAD_DIR = ./thread

# 定义源文件列表
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(MYSQL_DIR)/*.cpp $(SER_DIR)/*.cpp $(THREAD_DIR)/*.cpp)

# 定义可执行文件名
TARGET = server

# 默认目标
all: $(TARGET)

# 直接从源文件链接生成可执行文件
$(TARGET): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread -lmysqlclient -lboost_system

# 清理生成的文件
clean:
	rm -f $(TARGET)