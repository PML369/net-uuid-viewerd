CXXTESTGEN = cxxtest/bin/cxxtestgen
CXXTESTINCLUDE = cxxtest/

CPPS = RbtTrieTest.cpp RbtTrieNodeTest.cpp

build: runRbtTrieTest.out

runner.cpp:
	$(CXXTESTGEN) --error-printer --root -o $(.TARGET)

$(CPPS): $(.PREFIX).hpp
	$(CXXTESTGEN) --error-printer --part -o $(.TARGET) $(.PREFIX).hpp

runRbtTrieTest.out: $(CPPS) runner.cpp
	$(info ${TESTS})
	c++ -g -I$(CXXTESTINCLUDE) -I.. -o $(.TARGET) $(.ALLSRC)

run: build
	./runRbtTrieTest.out

clean:
	rm -f ./runRbtTrieTest.out
	rm -f $(CPPS)
	rm -f runner.cpp
