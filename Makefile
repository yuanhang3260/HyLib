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
TEST_DIR=test

OBJ = $(OBJ_DIR)/Base/Log.o \
			$(OBJ_DIR)/Base/Lock.o \
      $(OBJ_DIR)/Base/Path.o \
      $(OBJ_DIR)/Base/Ptr.o \
      $(OBJ_DIR)/Base/Utils.o \
      $(OBJ_DIR)/IO/File.o \
      $(OBJ_DIR)/IO/FileDescriptorInterface.o \
      $(OBJ_DIR)/IO/FileDescriptor.o \
      $(OBJ_DIR)/IO/FileSystemUtils.o \
      $(OBJ_DIR)/IO/TextPrinter.o \
      $(OBJ_DIR)/Network/Connection.o \
      $(OBJ_DIR)/Network/Socket.o \
      $(OBJ_DIR)/Network/SocketBase.o \
      $(OBJ_DIR)/Network/SecuredSocket.o \
      $(OBJ_DIR)/Network/StunProber.o \
      $(OBJ_DIR)/Strings/Split.o \
      $(OBJ_DIR)/Strings/StringBuilder.o \
      $(OBJ_DIR)/Strings/StringPiece.o \
      $(OBJ_DIR)/Strings/Utils.o \
      $(OBJ_DIR)/Utility/CleanUp.o \
      $(OBJ_DIR)/Utility/BufferedDataReader.o \
      $(OBJ_DIR)/Utility/BufferedDataWriter.o \
      $(OBJ_DIR)/Utility/BufferInterface.o \
      $(OBJ_DIR)/Utility/EventManager.o \
      $(OBJ_DIR)/Utility/FileMonitor.o \
      $(OBJ_DIR)/Utility/InfiniteBuffer.o \
      $(OBJ_DIR)/Utility/RingBuffer.o \
      $(OBJ_DIR)/Utility/StopWatch.o \
      $(OBJ_DIR)/Utility/Timer.o \
      $(OBJ_DIR)/Utility/ThreadPool.o \

TESTOBJ = $(OBJ_DIR)/Base/Lock_test.o \
					$(OBJ_DIR)/Base/Path_test.o \
          $(OBJ_DIR)/Base/Singleton_test.o \
          $(OBJ_DIR)/Base/Utils_test.o \
          $(OBJ_DIR)/IO/TextPrinter_test.o \
          $(OBJ_DIR)/IO/FileSystemUtils_test.o \
          $(OBJ_DIR)/Strings/StringBuilder_test.o \
          $(OBJ_DIR)/Strings/Split_test.o \
          $(OBJ_DIR)/Strings/Utils_test.o \
          $(OBJ_DIR)/Utility/FileMonitor_test.o \
          $(OBJ_DIR)/Utility/InfiniteBuffer_test.o \
          $(OBJ_DIR)/Utility/RingBuffer_test.o \
          $(OBJ_DIR)/Utility/Timer_test.o \

TESTEXE = test/Base/Lock_test.out \
          test/Base/Path_test.out \
          test/Base/Singleton_test.out \
          test/Base/Utils_test.out \
          test/IO/TextPrinter_test.out \
          test/IO/FileSystemUtils_test.out \
          test/Strings/StringBuilder_test.out \
          test/Strings/Split_test.out \
          test/Strings/Utils_test.out \
          test/Utility/FileMonitor_test.out \
          test/Utility/InfiniteBuffer_test.out \
          test/Utility/RingBuffer_test.out \
          test/Utility/Timer_test.out \

all: pre_build library

pre_build:
	mkdir -p $(OBJ_DIR)/Base $(OBJ_DIR)/IO $(OBJ_DIR)/Network $(OBJ_DIR)/Utility $(OBJ_DIR)/Strings
	mkdir -p $(TEST_DIR)/Base $(TEST_DIR)/IO $(TEST_DIR)/Network $(TEST_DIR)/Utility $(TEST_DIR)/Strings

library: pre_build $(OBJ)
	ar cr libhy.a $(OBJ)

test: pre_build $(TESTEXE)

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

$(OBJ_DIR)/Strings/%.o: $(SRC_DIR)/Strings/%.cc $(SRC_DIR)/Strings/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Strings/%.o: $(SRC_DIR)/Strings/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


# Tests
$(TEST_DIR)/Utility/%.out: $(OBJ_DIR)/Utility/%.o library
	$(CC) $(CFLAGS) $< libhy.a $(LFLAGS) -o $@

$(TEST_DIR)/Base/%.out: $(OBJ_DIR)/Base/%.o library
	$(CC) $(CFLAGS) $< libhy.a $(LFLAGS) -o $@

$(TEST_DIR)/Network/%.out: $(OBJ_DIR)/Network/%.o library
	$(CC) $(CFLAGS) $< libhy.a $(LFLAGS) -o $@

$(TEST_DIR)/IO/%.out: $(OBJ_DIR)/IO/%.o library
	$(CC) $(CFLAGS) $< libhy.a $(LFLAGS) -o $@

$(TEST_DIR)/Strings/%.out: $(OBJ_DIR)/Strings/%.o library
	$(CC) $(CFLAGS) $< libhy.a $(LFLAGS) -o $@

clean:
	rm -rf libhy.a
	rm -rf out
	rm -rf output/*
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/Base/*.o
	rm -rf $(OBJ_DIR)/IO/*.o
	rm -rf $(OBJ_DIR)/Network/*.o
	rm -rf $(OBJ_DIR)/Strings/*.o
	rm -rf $(OBJ_DIR)/UnitTest/*.o
	rm -rf $(OBJ_DIR)/Utility/*.o
	rm -rf test/*
	rm -rf data/*