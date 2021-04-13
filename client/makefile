CC=gcc
VPATH=.\
     :./user \
     :./answer
file=client.o  user.o  answer.o get_info.o

all:client remove_file

client: $(file)
$(file):

remove_file:
	rm *.o

clean:
	rm client
