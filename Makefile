all: questions

questions: qtree.o main.o file.o
	g++ -o $@ $^

%.o: %.cpp qtree.h
	g++ -c -Wall -o $@ $<

.PHONY: clean
clean:
	rm -f questions *.o 
	
#Here, write commands to invoke the executible and verify its behavior
#The first is given as an example with three commands
#  1: run the program with "user input" coming from the file test_scripts/simple_script.txt, and 
#     saving the program output in log.txt
#  2: compare log.txt with the sample output in test_scripts/simple_script.out
#  3: compare the written tree (requested by the user in the script) to the tree file example_trees/pb.txt
test:
	./questions < test_scripts/simple_script.txt > log.out
	diff log.out test_scripts/simple_script.out
	./questions < test_scripts/myScript.txt > myLog.out
	diff myLog.out test_scripts/myResponses.out
	
	
