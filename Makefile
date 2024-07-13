# Top-Level Makefile: Poker
# Date Created: 5/21/2024
# Modifications:
# 	5/21/24 ~ Created scaffold for Makefile
#	5/26/24 ~ Updated Makefile for Poker v2024.05.27 (Alpha)

# Default target
all: make

# Goes into src/ directory and calls the make there
make:
	(cd src/; make; cd ..)
	cp src/pokerserver bin/
	cp src/pokerclient bin/
	make gui_images
	@echo ""
	@echo "Note: Please the above instructions. Additonally the top level \"make all\""
	@echo "      main executables are in the bin/ directory."
	@echo ""

# makes all the unit_tests and copys them to bin/
test:
	make test-gui
	@echo ""
	@echo "All unit tests have been complied."
	@echo "To start navigate the bin/ directory and enter./(Executabe Name) into the terminal."
	@echo "Ex:"
	@echo "	Step #1: cd bin/"
	@echo "	Step #2: ./unit_test_gui"
	@echo ""

# makes the gui unit_test and runs it
test-gui:
	make gui_images
	(cd src/; make unit_test_gui; cd ..)
	cp src/unit_test_gui bin/
	(cd bin/; ./unit_test_gui; cd ..)
	@echo ""
	@echo "GUI has been tested."
	@echo ""

# makes the gui unit_test and runs it
test-gamelogic:
	(cd src/; make unit_test_gamelogic; cd ..)
	cp src/unit_test_gamelogic bin/
	(cd src/; ./unit_test_gamelogic; cd ..)
	@echo ""
	@echo "unit_test_gamelogic has been tested."
	@echo ""

# makes the comm unit_test and runs it
test-comm:
	(cd src/; make unit_test_client; make unit_test_server;cd ..)
	cp src/unit_test_client bin/
	cp src/unit_test_server bin/
	(cd src/; ./unit_test_server 10080; cd ..)
	(cd src/; ./unit_test_client 10080; cd ..)
	@echo ""
	@echo "Communication between server and client has been tested."
	@echo ""

#makes the client unit_test and runs it
test-client:
	(cd src/; make unit_test_client; cd ..)
	cp src/unit_test_client bin/
	(cd src/; ./unit_test_client crystalcove 10080; cd ..)
	@echo ""
	@echo "Client side has been tested."
	@echo ""

#makes the server unit_test and runs it
test-server:
	(cd src/; make unit_test_server; cd ..)
	cp src/unit_test_server bin/
	(cd src/; ./unit_test_server 10080; cd ..)
	@echo ""
	@echo "Server side has been tested."
	@echo ""

# --transform 's|^|poker/|' is used to place all the files into a folder called poker
tar:
	gtar cvzf Poker_V1.0.tar.gz --transform 's|^|poker/|' README COPYRIGHT INSTALL bin/ doc/Poker_UserManual.pdf
	gtar cvzf Poker_V1.0_src.tar.gz --transform 's|^|poker/|' README COPYRIGHT INSTALL Makefile bin/ doc/ src/
	@echo ""
	@echo "Tar files have been successfully created."
	@echo ""

# Gets newest src/ images
gui_images:
	rm -rf bin/gui_images
	cp -rf src/gui_images bin/

# Cleans unneeded files in the poker directory
clean:
	(cd src/; make clean; cd ..)
	rm -rf *.tar.gz
	@echo ""
	@echo "Excess files have been cleaned up."
	@echo ""

# Currently don't need this
# (cd bin/; rm -rf *; cd ..)
