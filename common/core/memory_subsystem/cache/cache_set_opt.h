#ifndef CACHE_SET_OPT_H
#define CACHE_SET_OPT_H

#include "cache_set.h"

#include <list>

// an entry of (address, priority)
struct Entry {
    UInt64 addr;
    int prty;
};

// store 
class CacheSetInfoOPT : public CacheSetInfo
{   
    public:
        struct Entry {
            UInt64 addr;
            int prty;
        };

    public:
        CacheSetInfoOPT(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts);
        virtual ~CacheSetInfoOPT();





    private:
        std::list<Entry> buffer;


};

class CacheSetOPT : public CacheSet 
{

    
    public:
        CacheSetOPT(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetInfoOPT* set_info, UInt8 num_attempts);
        virtual ~CacheSetOPT();

        UInt32  getReplacementIndex(CacheCntlr *cntlr);
        void    updateReplacementIndex(UInt32 accessed_index);

        void lookaheadOnce(UInt64 addr);
        void update(int hit_depth);
        int lookup(UInt64 addr);
        void stackRepair(UInt64 addr, int t);
        bool isExecuted(UInt64 addr);


    
    
    private:
        std::list<Entry> buffer;

        int timer;
        int dummpy_timer;
        std::map<UInt64, bool> isAccessed;
        CacheSetInfoOPT* m_set_info;



};
#endif