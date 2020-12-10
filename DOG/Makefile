dog: dog.o subroutines.o
	g++ -std=c++11 -Wall -Wextra -pedantic -o $@ $^

dog.o: dog.cc 
	g++ -c -std=c++11 -Wall -Wextra -pedantic -o $@ $^

subroutines.o: subroutines.cc
	g++ -c -std=c++11 -Wall -Wextra -pedantic -o $@ $^

clean:
	rm *.exe
