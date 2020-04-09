CC=gcc
BIN_DIR=bin
SRC_DIR=src

# The names of the .c files to compile
FILES = $(SRC_DIR)/main.c $(SRC_DIR)/util.c $(SRC_DIR)/simpledu.c $(SRC_DIR)/logger.c $(SRC_DIR)/ui.c $(SRC_DIR)/process_handler.c # separated by space

make: ${FILES}
	@mkdir -p $(BIN_DIR)
	@$(CC) -Wall -o $(BIN_DIR)/simpledu ${FILES}