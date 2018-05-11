CXXTESTGEN = cxxtest/bin/cxxtestgen
CXXTESTINCLUDE = cxxtest/

TESTS = RbTst RbTstInternal RbTstNode
CPPS = $(TESTS:%=%Test.cpp)

DEPDIR = .d
INCLUDE = -I$(CXXTESTINCLUDE) -I..

DEPFLAGS = -MD

COMPILE.cpp = c++ -g $(DEPFLAGS) $(INCLUDE) -std=c++11 -c
POSTCOMPILE = @mv -f $*.d $(DEPDIR)/$*.d && touch $@

build: runRbTstTest.out

runner.cpp:
	$(CXXTESTGEN) --error-printer --root -o $(.TARGET)

$(CPPS): $(.PREFIX).hpp
	$(CXXTESTGEN) --error-printer --part -o $(.TARGET) $(.PREFIX).hpp


$(TESTS:%=%Test.o) : $(.PREFIX).cpp $(DEPDIR)/$(.PREFIX).d
	$(COMPILE.cpp) -o $(.TARGET) $<
	$(POSTCOMPILE)

runner.o : runner.cpp
	$(COMPILE.cpp) -o $(.TARGET) $<
	$(POSTCOMPILE)

runRbTstTest.out: $(CPPS:%.cpp=%.o) runner.o
	c++ -o $(.TARGET) $(.ALLSRC)

run: build
	./runRbTstTest.out

clean:
	rm -f ./runRbTstTest.out
	rm -f $(CPPS)
	rm -f $(TESTS:%=%Test.o)
	rm -f runner.cpp
	rm -f runner.o

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d


.for INC in $(TESTS:%=$(DEPDIR)/%Test.d)
	.if exists($(INC))
		include $(INC)
	.endif
.endfor
