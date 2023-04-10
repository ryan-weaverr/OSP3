#include "slab.h"


unsigned char *slab_allocate()
{
  unsigned int slab_index, obj_index;
  unsigned char *addr;

  if(full_mask==0xffff) return NULL;

  // fill in

  return(addr);
}

int slab_release(unsigned char *addr)
{
  unsigned int sanity_check = ((int)(addr - start))>>16;  /* should be 0 */
  unsigned int slab_index = ((int)(addr - start))>>12;
  unsigned int obj_index = (((int)(addr - start))>>8)&15;

  /* bad address */
  if(sanity_check) return(1);
  if(s[slab_index].signature!=0x51ab51ab) return(1);
  /* slab is already marked as free */
  if(s[slab_index].free_mask & (short)(1<<(15-obj_index))) return(2);
  /* bad free count */
  if(s[slab_index].free_count>=15) printf("srel: high error in slab count!\n");

  s[slab_index].free_mask = s[slab_index].free_mask | (short)(1<<(15-obj_index));
  s[slab_index].free_count++;
  if(s[slab_index].free_count==1){
    /* move from full to partial */
    full_mask = full_mask ^ (short)(1<<(15-slab_index));
    partial_mask = partial_mask | (short)(1<<(15-slab_index));
  }else if(s[slab_index].free_count==15){
    /* move from partial to empty */
    partial_mask = partial_mask ^ (short)(1<<(15-slab_index));
    empty_mask = empty_mask | (short)(1<<(15-slab_index));
  }
  return(0);
}
