/*************************************************************************************|
|   1. YOU ARE NOT ALLOWED TO SHARE/PUBLISH YOUR CODE (e.g., post on piazza or online)|
|   2. Fill main.c and memory_hierarchy.c files                                       |
|   3. Do not use any other .c files neither alter main.h or parser.h                 |
|   4. Do not include any other library files                                         |
|*************************************************************************************/
#include "mipssim.h"

/// @students: declare cache-related structures and variables here
#define BLOCK_SIZE 16
int byte_offset;
int index_address;
int tag;
int index_bits;
int tag_bits;
int unified_bits;

struct block{
    int valid;
    int tag;
    int data[4];
};

struct Cache{
    int block_size; 
    int cache_size;
    int numLines;
    struct block* blocks;
        
};
struct Cache cache;

void memory_state_init(struct architectural_state* arch_state_ptr) {
    arch_state_ptr->memory = (uint32_t *) malloc(sizeof(uint32_t) * MEMORY_WORD_NUM);
    memset(arch_state_ptr->memory, 0, sizeof(uint32_t) * MEMORY_WORD_NUM);
    if(cache_size == 0){
        // CACHE DISABLED
        memory_stats_init(arch_state_ptr, 0); // WARNING: we initialize for no cache 0
    }else {
        // CACHE ENABLED
       // assert(0); /// @students: Remove assert(0); and initialize cache

        /// @students: memory_stats_init(arch_state_ptr, X); <-- fill # of tag bits for cache 'X' correctly
       // struct  Cache cache;
        //cache =  (struct block*) malloc( sizeof(struct block) * cache.numLines );
        cache.block_size = BLOCK_SIZE;
        cache.numLines = cache_size/BLOCK_SIZE;
        cache.blocks= (struct block*) malloc( sizeof(struct block) * cache.numLines );
        index_bits = get_log(cache.numLines);
        int offset = 4 ;// 4 words * (4 byte/word) = 16
        tag_bits = 32 - index_bits - offset;
        printf("indexbits %d , tag %d   mffffffffffffffffffffffffffffffffffffffffffff\n",index_bits,tag_bits);
        for(int i = 0; i < cache.numLines; i++)
    {
        cache.blocks[i].valid = 0;
        cache.blocks[i].tag=0;
        for(int j=0;j<4;j++){
            cache.blocks[i].data[j] = 0;
        }
    }
     memory_stats_init(arch_state_ptr, tag_bits);
        

    }
}
 void parseInput(int address){
     byte_offset = get_piece_of_a_word(address,0,4);
     index_address = get_piece_of_a_word(address,4,index_bits);
     tag = get_piece_of_a_word(address,4+index_bits,tag_bits);
     printf("address %d , offset %d , index %d , tag %d  \n",address,byte_offset,index_address,tag);
 }
 int get_log(int a){
     int count =0;
     int x = a;
     while(x>1){
          x/=2;
          count++;
     }
     return count;
 }


// returns data on memory[address / 4]
int memory_read(int address){
    struct block block1;
    arch_state.mem_stats.lw_total++;
    check_address_is_word_aligned(address);

    if(cache_size == 0){
        // CACHE DISABLED
        return (int) arch_state.memory[address / 4];
    }else{
        // CACHE ENABLED
        //assert(0); /// @students: Remove assert(0); and implement Memory hierarchy w/ cache
        parseInput(address);
        block1 = cache.blocks[index_address];
       // printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr valid=%d, block_tag = %d, tag= %d\n", block1.valid, block1.tag, tag);
        if(block1.valid==1 && block1.tag ==tag ) {
            
            arch_state.mem_stats.lw_cache_hits++;
            
            return cache.blocks[index_address].data[byte_offset/4];
            
        }
        else {  
                printf("Cache miss tag %u , valid_bit %d , index_address %d ",block1.tag,block1.valid,block1.data[byte_offset]);
                block1.data[byte_offset/4]  = arch_state.memory[address/4];             
                block1.valid = 1;
                block1.tag = tag;
               // cache.blocks[index_address].data[byte_offset/4] = arch_state.memory[address/4];   
                cache.blocks[index_address].valid = 1;
                cache.blocks[index_address].tag = tag;
                address = 0xFFFFFFF0 & address;
                for(int i=0;i<4;i++){
                cache.blocks[index_address].data[i] = arch_state.memory[address/4+i];
                 } 
                return cache.blocks[index_address].data[byte_offset/4];

                // unifiedData(block3.data[byte_offset/4],block3.data[byte_offset/4+1],block3.data[byte_offset/4+2],block3.data[byte_offset/4+3]);
                // return (int) cache.blocks[index_address].data[byte_offset/4];
                             
                                  

             }
             // return (int) arch_state.memory[address / 4]; 


        
        

        /// @students: your implementation must properly increment: arch_state_ptr->mem_stats.lw_cache_hits
    }
    return 0;
}
 
int getSpecificByte(int a,int count){
    int ret;
    int x = a;
     while(count>=0){
         ret = a%2;
         x=x/2;
         count--;
     }
     return ret;
}
// void unifiedData(int a, int b,int c,int d){
//     unified_bits= 8*a + 4*b + 2 * c + d;
// }

// writes data on memory[address / 4]
void memory_write(int address, int write_data){
    arch_state.mem_stats.sw_total++;
    check_address_is_word_aligned(address);
    struct block block2;

    if(cache_size == 0){
        // CACHE DISABLED
        arch_state.memory[address/ 4] = (uint32_t) write_data;
    }else{
        // CACHE ENABLED
        //assert(0); /// @stgetSpecificBytedents: Remove assert(0); and implement Memory hierarchy w/ cache

        

        /// @students: your getSpecificBytemplementation must properly increment: arch_state_ptr->mem_stats.sw_cache_hits
        parseInput(address);
        block2 = cache.blocks[index_address];
        if(block2.valid==1 && block2.tag == tag){
            arch_state.mem_stats.sw_cache_hits++;
            arch_state.memory[address / 4] = (uint32_t) write_data;
         
             cache.blocks[index_address].data[byte_offset/4] =(uint32_t)write_data; 
            
           // cache.blocks[index_address].data[byte_offset/4] =(uint32_t) write_data;
        }
        else {
           
            arch_state.memory[address / 4] = (uint32_t) write_data;
        }

    }
}
