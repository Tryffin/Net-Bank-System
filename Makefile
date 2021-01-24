all : TCPclient TCPserveur UDPclient UDPserveur
.PHONY : clean

TCPclient: TCPclient.c
	gcc -o TCPclient TCPclient.c

TCPserveur:TCPserveur.c TCPserveur.h
	gcc -o TCPserveur TCPserveur.c

UDPclient: UDPclient.c
	gcc -o UDPclient UDPclient.c

UDPserveur:UDPserveur.c UDPserveur.h
	gcc -o UDPserveur UDPserveur.c
clean : 
	rm    TCPclient
	rm    TCPserveur
	rm    UDPclient
	rm    UDPserveur