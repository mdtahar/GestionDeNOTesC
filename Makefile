CC=gcc 
CFLAGS=-g -Wall 

EXEC=union select tri moyenne_ponderee fusion formatage 
all: $(EXEC)

union: myunion.c union.c formater.c structure.c
	$(CC) $(CFLAGS) $^ -o $@
select: select.c myunion.c formater.c structure.c
	$(CC) $(CFLAGS) $^ -o $@
tri: tri.c mytri.c formater.c structure.c myunion.c 
	$(CC) $(FLAGS) $^ -o $@
moyenne_ponderee: moyenne_ponderee.c mymoyenne.c myunion.c structure.c formater.c 
	$(CC) $(FLAGS) $^ -o $@
fusion: fusion.c myfusion.c myunion.c structure.c formater.c mytri.c 
	$(CC) $(FLAGS) $^ -o $@
formatage: formatage.c myformatage.c myunion.c structure.c formater.c mytri.c
	$(CC) $(FLAGS) $^ -o $@	

		
clean: 
		rm -rf *.o $(EXEC) *~ *.dSYM
