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
  int rc, i;

  init_slabs();

  for( i = 1; i <= 242; i++ ){
    address[i] = slab_allocate();
    if( address[i] == NULL ){
      printf("allocation %d returned a NULL pointer\n",i);
    }
  }
  printf("empty/partial/full = %04x %04x %04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  rc = slab_release(address[239]);
  printf("return code is %d, expecting 0\n",rc);
  slab_inspect(address[240]);
  rc = slab_release(address[239]);
  printf("return code is %d, expecting 2\n",rc);
  rc = slab_release(0);
  printf("return code is %d, expecting 1\n",rc);
  rc = slab_release(start-4096);
  printf("return code is %d, expecting 1\n",rc);
  rc = slab_release(start+65536);
  printf("return code is %d, expecting 1\n",rc);

  for( i = 1; i <= 240; i++ ) rc = slab_release(address[i]);

  printf("empty/partial/full = %04x/%04x/%04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  for( i = 1; i <= 240; i++ ) address[i] = slab_allocate();
  for( i = 1; i <= 240; i++ ) rc = slab_release(address[i]);

  printf("empty/partial/full = %04x/%04x/%04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);

  address[1] = slab_allocate();
  s[0].signature = 0;
  rc = slab_release(address[1]);
  printf("return code is %d, expecting 1\n",rc);

  printf("empty/partial/full = %04x/%04x/%04x\n", (unsigned int)empty_mask,
    (unsigned int)partial_mask, (unsigned int)full_mask);
  
  return 0;
}
