all: comm_progs

comm_progs:
	g++ -o prueba.o main.cpp -lcurl
	./prueba.o

# docker run -it --rm -v "/c/Users/windows/Documents/Estructuras-Caso3:/home" gcc bash
# docker run -it --rm -v "D:/System/Documents/GitHub/Caso-3---Estructuras:/home" gcc bash
# docker run -it --rm -v "C:\Users\led_2\Documents\GitHub\Caso-3---Estructuras:/home" gcc bash
# apt update -y
# apt install curl

