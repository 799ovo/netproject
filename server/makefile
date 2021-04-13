CC=gcc -g
VPATH=.\
     :./database \
	 :./io \
	 :./network \
     :./queue \
	 :./thread \
	 :./logic/user \
     :./logic/answer 

file=server.o \
     database.o \
	 io.o \
	 network.o \
	 queue.o \
	 thread.o \
	 user.o \
     answer.o \
     title.o

all:server remove_file

server: $(file)
	gcc -o server $(file)  -lpthread -lmysqlclient
$(file):

remove_file:
	rm *.o

clean:
	rm server