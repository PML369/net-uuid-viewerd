.ifndef USR_SRC
USR_SRC = /usr/src
.endif
.ifndef BINDIR
BINDIR = .
.endif


PROBES = trace_start trace_stop mem_alloc drop socket_create from_socket \
	 to_socket to_subsys fragment layer_arrive layer_depart
COMMANDS = dump_data address_heatmap defined_layers layer_timings
CPPS = $(PROBES:%=Probes/Probe_%.cpp) \
	$(COMMANDS:%=Commands/Command_%.cpp) \
	net-uuid-viewerd.cpp \
	dtrace_import.cpp \
	PacketInfo.cpp \
	RequestHandler.cpp \
	NetUuidData.cpp \
	Probes/ProbeType.cpp \

DEPDIR = .d
INCLUDE = -I $(USR_SRC)/cddl/compat/opensolaris/include/ \
		-I $(USR_SRC)/cddl/contrib/opensolaris/lib/libdtrace/common/ \
		-I $(USR_SRC)/sys/cddl/compat/opensolaris/ \
		-I $(USR_SRC)/sys/cddl/contrib/opensolaris/uts/common/ \
		-I .

DEPFLAGS = -MD

COMPILE.cpp = c++ -std=c++11 -g $(DEPFLAGS) $(INCLUDE) -c
POSTCOMPILE = @mv -f $*.d $(DEPDIR)/$*.d && touch $@


all: net-uuid-viewerd

net-uuid-viewerd: $(CPPS:%.cpp=%.o)
	c++ \
		$(CPPS:%.cpp=%.o) \
		-l dtrace -l proc -l ctf -l elf -l z -l rtld_db -l pthread -l util \
		-o $(BINDIR)/net-uuid-viewerd

$(CPPS:%.cpp=%.o) : $(.PREFIX).cpp $(DEPDIR)/$(.PREFIX).d
	$(COMPILE.cpp) -o $(.TARGET) $<
	$(POSTCOMPILE)

clean:
	rm -f $(CPPS:%.cpp=%.o)
	rm -f $(BINDIR)/net-uuid-viewerd
	rm -f $(BINDIR)/hello_dtrace

.PHONY: test

test:
	$(MAKE) -C test clean
	$(MAKE) -C test run

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d


.for INC in $(CPPS:%.cpp=$(DEPDIR)/%.d)
	.if exists($(INC))
		include $(INC)
	.endif
.endfor
