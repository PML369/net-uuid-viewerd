.ifndef USR_SRC
USR_SRC = /usr/src
.endif

all: hello_dtrace.out dtrace_import.out

hello_dtrace.out:
	cc \
		-I $(USR_SRC)/cddl/compat/opensolaris/include/ \
		-I $(USR_SRC)/cddl/contrib/opensolaris/lib/libdtrace/common/ \
		-I $(USR_SRC)/sys/cddl/compat/opensolaris/ \
		-I $(USR_SRC)/sys/cddl/contrib/opensolaris/uts/common/ \
		hello_dtrace.c \
		-l dtrace -l proc -l ctf -l elf -l z -l rtld_db -l pthread -l util \
		-o $(BINDIR)/hello_dtrace

dtrace_import.out:
	cc \
		-I $(USR_SRC)/cddl/compat/opensolaris/include/ \
		-I $(USR_SRC)/cddl/contrib/opensolaris/lib/libdtrace/common/ \
		-I $(USR_SRC)/sys/cddl/compat/opensolaris/ \
		-I $(USR_SRC)/sys/cddl/contrib/opensolaris/uts/common/ \
		dtrace_import.c \
		-l dtrace -l proc -l ctf -l elf -l z -l rtld_db -l pthread -l util \
		-o $(BINDIR)/dtrace_import
