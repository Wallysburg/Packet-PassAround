#
# author: bjr
# created: 31 jan 2015
# last update: 
#
#echo "implement functionality"                                                             
        #ls /here-to-create-an-error    

LOCALHOST = localhost
REMOTEHOST = shiloh

passaround: passaround.c passaround.h mynetfunctions.o 
	cc -Wall -o $@ $<  mynetfunctions.o

all:
	make passaround
	make passaround-test
		
passaround-test: passaround-test.c passaround.h mynetfunctions.o
	cc -Wall -o $@ $<  mynetfunctions.o

test-one:
	echo run test-s1 on shiloh, test-s2 on antietam

test-s1:
	./passaround -vd antietam.cs.miami.edu 4444 

test-s2:
	./passaround -vdz -p 4444 shiloh.cs.miami.edu 9945

test-c:
	./passaround-test -c 12 -v shiloh.cs.miami.edu 3333


mynetfunctions.o: mynetfunctions.c passaround.h
	cc -Wall -c $<

test: passaround passaround-test
	-killall passaround
	(./passaround -vd localhost 3333 > test.out &)
	./passaround-test -c 2 localhost 3333
	diff test.out test.ref
	-killall passaround

my-test: passaround passaround-test
	  -killall passaround
	 (./passaround -vd shiloh.cs.miami.edu 3333 > test.out &)
	  ./passaround-test -c 3 shiloh.cs.miami.edu 3333
	  -killall passaround

submit:
	svn commit -m "submitted for grade"

clean:
	-rm mynetfunctions.o passaround passaround-test test.out


