#include "slab.h"

slab_t __attribute__ ((aligned (65536))) s[16];
unsigned char *start;
short unsigned int empty_mask, partial_mask, full_mask;

void init_slabs(){
  int i;
  empty_mask = 0xffff;
  partial_mask = full_mask = 0;
  for(i=0;i<16;i++){
    s[i].free_count = 15;
    s[i].free_mask = 0x7fff;
    s[i].signature = 0x51ab51ab;
  }
  start = (unsigned char *)s;
}

void slab_inspect( unsigned char *addr ){
  unsigned int sanity_check = ((unsigned int)(addr - start))>>16;
  unsigned int slab_index = ((unsigned int)(addr - start))>>12;
  // unsigned int obj_index = (((unsigned int)(addr - start))>>8)&15;
  if(sanity_check)
    printf("range check error in inspect address!\n");
  else if(s[slab_index].signature!=0x51ab51ab)
    printf("signature check error in inspect address!\n");
  else
    printf("slab %d has free count of %d and free mask of %04x\n",
      slab_index,
      s[slab_index].free_count,
      (unsigned int)s[slab_index].free_mask);
}


int main(){
  unsigned char *address[256];
  int i;

  init_slabs();

  printf("empty/partial/full = %04x %04x %04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  for( i = 1; i <= 240; i++ ) address[i] = slab_allocate();

  printf("empty/partial/full = %04x %04x %04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  for( i = 1; i <= 240; i++ ) slab_release(address[i]);

  printf("empty/partial/full = %04x %04x %04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  for( i = 1; i <= 240; i++ ) address[i] = slab_allocate();

  slab_release( address[ 2] );
  slab_release( address[ 3] );
  slab_release( address[19] );
  slab_release( address[35] );
  slab_release( address[51] );
  slab_release( address[67] );
  slab_release( address[83] );
  slab_release( address[99] );
  slab_release( address[115] );
  slab_release( address[131] );
  slab_release( address[147] );
  slab_release( address[163] );
  slab_release( address[179] );
  slab_release( address[195] );
  slab_release( address[211] );
  slab_release( address[227] );

  address[241] = slab_allocate();

  printf("empty/partial/full = %04x %04x %04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  slab_inspect(address[1]);
  slab_inspect(address[16]);
  slab_inspect(address[31]);
  slab_inspect(address[46]);
  slab_inspect(address[61]);
  slab_inspect(address[76]);
  slab_inspect(address[91]);
  slab_inspect(address[106]);
  slab_inspect(address[121]);
  slab_inspect(address[136]);
  slab_inspect(address[151]);
  slab_inspect(address[166]);
  slab_inspect(address[181]);
  slab_inspect(address[196]);
  slab_inspect(address[211]);
  slab_inspect(address[226]);

  return 0;
}
