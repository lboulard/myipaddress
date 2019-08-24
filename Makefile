CFLAGS = -Wall -Werror -Wunused
CFLAGS += -std=c99

CPPFLAGS = -D_DEFAULT_SOURCE

all: myipaddress allipaddr

myipaddress: myipaddress.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@

allipaddr: allipaddr.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@

show: myipaddress allipaddr
	@echo "MyIPAddress: $$(./myipaddress)"
	@echo "All IP Addr: $$(./allipaddr)"

clean:
	$(RM) myipaddress allipaddr

.PHONY: clean run
