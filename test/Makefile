CXXTESTGEN = cxxtest/bin/cxxtestgen
CXXTESTINCLUDE = cxxtest/

CPPS = RbtTrieTest.cpp RbtTrieNodeTest.cpp

build: runRbtTrieTest.out

RbtTrieTest.cpp: $(.PREFIX).hpp
	$(CXXTESTGEN) --error-printer -o $(.TARGET) $(.PREFIX).hpp

$(CPPS): $(.PREFIX).hpp
	$(CXXTESTGEN) --error-printer --part -o $(.TARGET) $(.PREFIX).hpp

runRbtTrieTest.out: $(CPPS)
	$(info ${TESTS})
	c++ -g -I$(CXXTESTINCLUDE) -I.. -o $(.TARGET) $(.ALLSRC)

run: build
	./runRbtTrieTest.out

clean:
	rm -f ./runRbtTrieTest.out
	rm -f $(CPPS)
