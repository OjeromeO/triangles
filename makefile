CC = g++

drawtri : main.o mainframe.o dialogs.o openglcanvas.o triangle.o
	$(CC) -o drawtri main.o mainframe.o dialogs.o openglcanvas.o triangle.o `wx-config --libs --gl-libs`

main.o : main.cpp 
	$(CC) `wx-config --cxxflags` -c main.cpp -o main.o

mainframe.o : mainframe.cpp
	$(CC) `wx-config --cxxflags` -c mainframe.cpp -o mainframe.o

dialogs.o : dialogs.cpp
	$(CC) `wx-config --cxxflags` -c dialogs.cpp -o dialogs.o

openglcanvas.o : openglcanvas.cpp
	$(CC) `wx-config --cxxflags` -c openglcanvas.cpp -o openglcanvas.o

triangle.o : triangle.cpp
	$(CC) `wx-config --cxxflags` -c triangle.cpp -o triangle.o


clean : 
	rm -f *.o drawtri 
	
