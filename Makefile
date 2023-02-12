CC = g++
CFLAGS = -Wall -g
LIBS = -l mosquittopp
all: mqttclient
mqttclient: main.cpp mqtt.cpp mqtt.h
	$(CC) $(CFLAGS) -o mqttclient  main.cpp mqtt.cpp $(LIBS)
debugbuild: mqttclient
	$(CC) $(CFLAGS) -o mqttclient  main.cpp mqtt.cpp $(LIBS) -DDEBUG
clean:
	rm mqttclient
