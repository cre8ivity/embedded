PROGS_DAGGER_OBJS := test.o
PROGS_DAGGER_OBJS := $(PROGS_DAGGER_OBJS:%=$(TDIR)/test/%)
ALL_OBJS += $(PROGS_DAGGER_OBJS)

$(TDIR)/bin/test : $(TSTART) $(PROGS_DAGGER_OBJS) $(TLIBC)