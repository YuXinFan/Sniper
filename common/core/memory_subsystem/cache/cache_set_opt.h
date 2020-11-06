#ifndef CACHE_SET_OPT_H
#define CACHE_SET_OPT_H

#include "cache_set.h"

#include <list>



// an entry of (address, priority)




// store 
class CacheSetInfoOPT : public CacheSetInfo
{   
    public:
        CacheSetInfoOPT(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts);
        virtual ~CacheSetInfoOPT();
        int getPrty(UInt32 index);





    private:
        std::list<Entry> buffer; 
        UInt32 m_associativity;
        UInt8 m_attempts;
        CacheBlockInfo* m_cache_block_info_array;



};

class CacheSetOPT : public CacheSet 
{

    
    public:
        CacheSetOPT(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetInfoOPT* set_info, UInt8 num_attempts);
        virtual ~CacheSetOPT();

        UInt32  getReplacementIndex(CacheCntlr *cntlr);
        void    updateReplacementIndex(UInt32 accessed_index);

        void update(int hit_depth);
        int lookup(UInt64 addr);
        void stackRepair(UInt64 addr, int t);

        int getPrty(UInt32 index);


    
    
    private:
        std::list<Entry> buffer;

        int timer;
        int dummpy_timer;
        std::map<UInt64, bool> isAccessed;
        CacheSetInfoOPT* m_set_info;



};

bool referenced(UInt64);
void rawRepair(std::list<Entry> &buffer, Entry &entry, int true_prty, int depth);
void initBuffer(std::list<Entry> &buffer) ;
#endif