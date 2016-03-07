#
# Libraries I wrote.
#
# Hang Yuan <yuanhang3260@gmail.com>
#
CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2 -g
LFLAGS=-pthread
IFLAGS=-Isrc/

SRC_DIR=src
OBJ_DIR=lib

OBJ = $(OBJ_DIR)/Base/Utils.o \
      $(OBJ_DIR)/Base/Log.o \
      $(OBJ_DIR)/Base/StringPiece.o \
      $(OBJ_DIR)/IO/FileDescriptorInterface.o \
      $(OBJ_DIR)/IO/FileDescriptor.o \
      $(OBJ_DIR)/IO/FileSystemUtils.o \
      $(OBJ_DIR)/IO/TextPrinter.o \
      $(OBJ_DIR)/Network/Socket.o \
      $(OBJ_DIR)/Utility/BufferedDataReader.o \
      $(OBJ_DIR)/Utility/BufferedDataWriter.o \
      $(OBJ_DIR)/Utility/EventManager.o \
      $(OBJ_DIR)/Utility/FileMonitor.o \
      $(OBJ_DIR)/Utility/StringBuilder.o \
      $(OBJ_DIR)/Utility/Strings.o \
      $(OBJ_DIR)/Utility/ThreadPool.o \

TESTOBJ = $(OBJ_DIR)/Base/Utils_test.o \
					$(OBJ_DIR)/Base/Singleton_test.o \
					$(OBJ_DIR)/IO/FileSystemUtils_test.o \
					$(OBJ_DIR)/IO/TextPrinter_test.o \
          $(OBJ_DIR)/Utility/Strings_test.o \
          $(OBJ_DIR)/Utility/StringBuilder_test.o \

TESTEXE = test/Utils_test.out \
					test/Singleton_test.out \
					test/FileSystemUtils_test.out \
					test/TextPrinter_test.out \
          test/Strings_test.out \
          test/StringBuilder_test.out \

library: $(OBJ)
	ar cr libhy.a $(OBJ)

test: $(TESTEXE)

$(TESTEXE): $(TESTOBJ) library

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Utility/%.o: $(SRC_DIR)/Utility/%.cc $(SRC_DIR)/Utility/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Utility/%.o: $(SRC_DIR)/Utility/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Base/%.o: $(SRC_DIR)/Base/%.cc $(SRC_DIR)/Base/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Base/%.o: $(SRC_DIR)/Base/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Network/%.o: $(SRC_DIR)/Network/%.cc $(SRC_DIR)/Network/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Network/%.o: $(SRC_DIR)/Network/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/IO/%.o: $(SRC_DIR)/IO/%.cc $(SRC_DIR)/IO/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/IO/%.o: $(SRC_DIR)/IO/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


# Tests
test/%.out: $(OBJ_DIR)/Utility/%.o library
	$(CC) $(CFLAGS) $< libhy.a -o $@

test/%.out: $(OBJ_DIR)/Base/%.o library
	$(CC) $(CFLAGS) $< libhy.a -o $@

test/%.out: $(OBJ_DIR)/Network/%.o library
	$(CC) $(CFLAGS) $< libhy.a -o $@

test/%.out: $(OBJ_DIR)/IO/%.o library
	$(CC) $(CFLAGS) $< libhy.a -o $@

clean:
	rm -rf libhy.a
	rm -rf out
	rm -rf output/*
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/Base/*.o
	rm -rf $(OBJ_DIR)/IO/*.o
	rm -rf $(OBJ_DIR)/Network/*.o
	rm -rf $(OBJ_DIR)/UnitTest/*.o
	rm -rf $(OBJ_DIR)/Utility/*.o
	rm -rf test/*.out