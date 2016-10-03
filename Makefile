ifdef SYSTEMROOT
	MAKENAME = win.mk
else
	ifeq ($(shell uname), Linux)
		MAKENAME = linux.mk
	endif
endif


sword:
	make -f $(MAKENAME) $@

%.o:
	make -f $(MAKENAME) $@

debug:
	make -f $(MAKENAME) $@

clear:
	make -f $(MAKENAME) $@