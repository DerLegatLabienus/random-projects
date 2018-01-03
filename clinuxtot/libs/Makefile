CC=gcc
LD=gcc
PROJECT_BASE=/home/aviva/random-projects/clinuxtot

SUBDIRS=$(subst /, ,$(sort $(dir $(wildcard */))))

.PHONY: all clobber $(SUBDIRS)
all clobber: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

