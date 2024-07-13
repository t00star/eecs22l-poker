# Makefile: Poker
# Date Created: 5/21/2024
# Modifications:
# 	5/21/24 ~ Created Makefile and added make for unit_test_gui
#	5/26/24 ~ Updated Makefile for Poker v2024.05.27 (Alpha)

# GTK 2.0 Makefile Variables
GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`

# Variables
CC = gcc
DEBUG = -g -DDEBUG
# DEBUG = -O2
CFLAGS = -Wall -std=c11 -pedantic $(DEBUG) $(GTKINC) -c
LFLAGS = -Wall -std=c11 -pedantic $(DEBUG) $(GTKLIBS) -lm -lpthread

# Default target
all: pokerclient pokerserver
	@echo ""
	@echo "Main executables have been compiled."
	@echo "To start connect the pokerserver to a port and connect a pokerclient on a separate server."
	@echo "Note: Current GUI opens on pokerclient before the client connects to the server."
	@echo "Ex:"
	@echo "	Step #1: Run \"./pokerserver 10080\" on the server crystalcove"
	@echo "	Step #2: Run \"./pokerclient crystalcove 10080\" on a different server"
	@echo ""

# Generates all the Unit Tests
test: unit_test_gui unit_test_client unit_test_server unit_test_gamelogic
	@echo ""
	@echo "All unit tests have been compiled."
	@echo "To start enter ./(Executable Name) into the terminal."
	@echo "Note: All unit test executables begin with \"unit_test_\""
	@echo "	Ex: ./unit_test_gui"
	@echo ""

# Target for clean-up
clean:
	rm -rf *.o pokerclient pokerserver unit_test_gui unit_test_client unit_test_server unit_test_gamelogic clienttest server
	@echo ""
	@echo "Excess files have been cleaned up."
	@echo ""

# Main Executables

# Target for pokerclient
pokerclient: pokerclient.o pokerclient_core.o gui.o
	$(CC) $(LFLAGS) pokerclient.o pokerclient_core.o gui.o -o pokerclient

# Target for pokerserver
pokerserver: pokerserver.o pokerserver_core.o
	$(CC) $(LFLAGS) pokerserver.o pokerserver_core.o -o pokerserver

# GUI Unit Test

# Target for gui.o
gui.o: gui.c gui.h
	$(CC) $(CFLAGS) gui.c -o gui.o

# Target for GUI Unit unit_test_gui
unit_test_gui.o: unit_test_gui.c gui.h
	$(CC) $(CFLAGS) unit_test_gui.c -o unit_test_gui.o

# Target for GUI Unit unit_test_gui
unit_test_gui: unit_test_gui.o gui.o
	$(CC) $(LFLAGS) unit_test_gui.o gui.o -o unit_test_gui

# Client Unit Test

# Target for pokerclient_core.o
pokerclient_core.o: pokerclient_core.c
	$(CC) $(CFLAGS) pokerclient_core.c -o pokerclient_core.o

# Target for unit_test_client.o
unit_test_client.o: unit_test_client.c
	$(CC) $(CFLAGS) unit_test_client.c -o unit_test_client.o

# Target for unit_test_client executable
unit_test_client: unit_test_client.o pokerclient_core.o
	$(CC) $(LFLAGS) unit_test_client.o pokerclient_core.o -o unit_test_client

# Server Unit Test

# Target for pokerserver_core.o
pokerserver_core.o: pokerserver_core.c
	$(CC) $(CFLAGS) pokerserver_core.c -o pokerserver_core.o

# Target for unit_test_server.o
unit_test_server.o: unit_test_server.c
	$(CC) $(CFLAGS) unit_test_server.c -o unit_test_server.o

# Target for unit_test_server executable
unit_test_server: unit_test_server.o pokerserver_core.o
	$(CC) $(LFLAGS) unit_test_server.o pokerserver_core.o -o unit_test_server

# gamelogic Unit Test

# Target for gamelogic.o
gamelogic.o: gamelogic.c gamelogic.h
	$(CC) $(CFLAGS) gamelogic.c -o gamelogic.o

# Target for gamelogic unit_test_gamelogic
unit_test_gamelogic.o: unit_test_gamelogic.c gamelogic.h
	$(CC) $(CFLAGS) unit_test_gamelogic.c -o unit_test_gamelogic.o

# Target for gamelogic unit_test_gamelogic
unit_test_gamelogic: unit_test_gamelogic.o gamelogic.o
	$(CC) $(LFLAGS) unit_test_gamelogic.o gamelogic.o -o unit_test_gamelogic
