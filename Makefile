all:
.SILENT:
.SECONDARY:
PRECMD=echo "  $(@F)" ; mkdir -p $(@D) ;

CC:=gcc -c -MMD -O3 -Werror -Wimplicit
LD:=gcc
LDPOST:=

AS65:=xa -c
LD65:=xa
# ldo65?

SRCFILES:=$(shell find src -type f)

TK_CFILES:=$(filter src/thunderkiss/%.c,$(SRCFILES))
TK_OFILES:=$(patsubst src/%.c,mid/%.o,$(TK_CFILES))
-include $(TK_OFILES:.o=.d)
mid/thunderkiss/%.o:src/thunderkiss/%.c;$(PRECMD) $(CC) -o $@ $<

TK_EXE:=out/thunderkiss
all:$(TK_EXE)
$(TK_EXE):$(TK_OFILES);$(PRECMD) $(LD) -o $@ $^ $(LDPOST)

HELLO_SFILES:=$(filter src/hello/%.s,$(SRCFILES))
#HELLO_OFILES:=$(patsubst src/%.s,mid/%.o,$(HELLO_SFILES))
#TODO Can xa produce Makefiles like `gcc -MMD`?
#mid/hello/%.o:src/hello/%.s;$(PRECMD) $(AS65) -o $@ $<
#...fuck it, this isn't working. Just do all the assembly in one call.

HELLO_ROM:=out/hello
all:$(HELLO_ROM)
$(HELLO_ROM):$(HELLO_SFILES);$(PRECMD) $(LD65) -o $@ $^
#TODO compose rom file...

clean:;rm -rf mid out

run:$(TK_EXE) $(HELLO_ROM);$(TK_EXE) $(HELLO_ROM)
