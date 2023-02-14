#include "tk.h"
#include "x6502.h"

ARead_fn ARead[0x10000]={0};
BWrite_fn BWrite[0x10000]={0};
uint8_t RAM[0x800]={0};

// Auto-halt after so many instructions if nonzero.
static int ttl=4;

static uint8_t *rom=0;
static int romc=0;

static uint8_t read_dummy(unsigned int p) {
  fprintf(stderr,"READ 0x%04x => 0x00\n",p);
  return 0x00;
}
static void write_dummy(unsigned int p,uint8_t v) {
  fprintf(stderr,"WRITE 0x%04x = 0x%02x, DISCARDING\n",p,v);
}
static uint8_t read_ram_3000(unsigned int p) {
  uint16_t ramp=p-0x3000;
  fprintf(stderr,"READ 0x%04x (RAM 0x%04x) => 0x%02x\n",p,ramp,RAM[ramp]);
  return RAM[ramp];
}
static void write_ram_3000(unsigned int p,uint8_t v) {
  uint16_t ramp=p-0x3000;
  fprintf(stderr,"WRITE 0x%04x = 0x%02x (RAM 0x%04x)\n",p,v,ramp);
  RAM[ramp]=v;
}
static uint8_t read_rom(unsigned int p) {
  fprintf(stderr,"READ 0x%04x (ROM) => %02x\n",p,rom[p]);
  return rom[p];
}
static void write_rom(unsigned int p,uint8_t v) {
  fprintf(stderr,"WRITE 0x%04x = 0x%02x (ROM)\n",p,v);
  rom[p]=v; // the "o" in "rom" stands or "Or whatever"
}
static uint8_t read_irqv(unsigned int p) {
  uint8_t v=0;
  switch (p) {
    case 0xfffc: v=0x00; break; // initial PC, LSB
    case 0xfffd: v=0x00; break; // initial PC, MSB
    case 0xfffe: v=0x00; break; // interrupt PC, LSB (interrupt? it jumps here when we execute opcode 0x00)
    case 0xffff: v=0x00; break; // interrupt PC, MSB
  }
  fprintf(stderr,"READ 0x%04x => 0x%02x (irqv)\n",p,v);
  return v;
}

static void dump_state() {
  fprintf(stderr,
    "PC=%04x A=%02x X=%02x Y=%02x S=%02x P=%02x\n",
    X.PC,X.A,X.X,X.Y,X.S,X.P
  );
}

int main(int argc,char **argv) {
  if ((argc!=2)||(argv[1][0]=='-')) {
    fprintf(stderr,"Usage: %s FILE\n",argv[0]);
    return 1;
  }
  if ((romc=tk_file_read(&rom,argv[1]))<0) {
    fprintf(stderr,"%s: Failed to read file.\n",argv[1]);
    return 1;
  }
  fprintf(stderr,"%s: ok read %d bytes\n",argv[1],romc);
  if (romc>0x3000) {
    fprintf(stderr,"%s:WARNING: ROM exceeds 0x3000. only the first 12 kB will be available\n",argv[1]);
    romc=0x3000;
  }
  
  int i=0x0000;
  for (;i<romc;i++) {
    ARead[i]=read_rom;
    BWrite[i]=write_rom;
  }
  for (;i<0x3000;i++) {
    ARead[i]=read_dummy;
    BWrite[i]=write_dummy;
  }
  for (;i<0x3800;i++) {
    ARead[i]=read_ram_3000;
    BWrite[i]=write_ram_3000;
  }
  for (;i<0x10000;i++) {
    ARead[i]=read_dummy;
    BWrite[i]=write_dummy;
  }
  ARead[0xfffc]=read_irqv;
  ARead[0xfffd]=read_irqv;
  ARead[0xfffe]=read_irqv;
  ARead[0xffff]=read_irqv;
  
  X6502_Init();
  // If we call X6502_Power, it simulates an initial IRQ, and reads the initial PC from 0xfffc.
  // That's probably how it works in real life, but for this experiment I'd rather start at 0x0000 always.
  // Which is what happens when we run on a plain Init.
  //X6502_Power(); // fwiw Power contains a Reset
  
  dump_state();
  if (ttl) {
    while (ttl--) {
      X6502_Run();
      dump_state();
    }
    fprintf(stderr,"Auto-abort due to TTL\n");
  } else {
    for (;;) {
      X6502_Run();
      dump_state();
    }
  }
  
  return 0;
}
