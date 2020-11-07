#ifndef CACHE_SET_OPT_H
#define CACHE_SET_OPT_H

#include "cache_set.h"

#include <list>


// store 
class CacheSetInfoOPT : public CacheSetInfo
{   
    public:
        CacheSetInfoOPT(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts);
        virtual ~CacheSetInfoOPT();
    private:
        std::list<Entry> buffer; 
        UInt32 m_associativity;

};

class CacheSetOPT : public CacheSet 
{
    public:
        CacheSetOPT(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetInfoOPT* set_info, UInt8 num_attempts);
        virtual ~CacheSetOPT();

        UInt32  getReplacementIndex(CacheCntlr *cntlr);
        void    updateReplacementIndex(UInt32 accessed_index);

        int getPrty(UInt32 index);

    private:
        CacheSetInfoOPT* m_set_info;



};
#endif