# NES Research

wouldn't it be cool to develop games for NES....

## Assemblers

`apt search 6502`

| Name | Reference | Comment |
|------|-----------|---------|
| 64tass | https://tass64.sourceforge.net/#commandline-options | Docs suggest it's incomplete. Logs a bit. |
| acme   | | "Program counter undefined" |
| cc65   | | Not an assembler! C compiler, outputs 6502 assembly. cooooool... acme doesn't like it, 64tass neither, crasm neither. `ca65`! |
| crasm  | | --help isn't very helpful, and nothing works right off the bat |
| dasm   | | '' same --help, "Unknown Mnemonic" everywhere. Maybe we have to set the dialect somewhere? |
| xa65   | https://www.floodgap.com/retrotech/xa/ | Nice and neat. (command is `xa` not `xa65`). Produces just code, eg "lda $12" output is 2 bytes "a5 12". Actively maintained. |

From this extremely brief survey, xa65 is the clear winner.
cc65/ca65 looks like the most polished package.

## Learning approach

Is there some other 6502 system I can emulate, that might be more amenable to learning assembly?

Can I write one? Like, yoink the 6502 bits from fceu and build some simple like register-monitor thing around that?

## Thunder Kiss 65

It's a White Zombie song. And a bare-bones 6502 runtime, copying just the 6502 emulator from fceu.

- [ ] ldo65: Warning: label '' multiply defined (mid/hello/hello.o and mid/hello/hello.o)
- [ ] file65: mid/hello/hello.o: not an o65 file!
- Sidestepped these by assembling straight from sources to the final output.

`make run` to build Thunderkiss and the ROM file, and run it.
Every memory access will dump to stderr, and the full register state after each instruction.
