#include "cache_set_opt.h"
#include "log.h"
#include "stats.h"

extern std::list<Entry> lookahead_list;

/*
CacheSetInfoOPT
*/
CacheSetInfoOPT::CacheSetInfoOPT(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts) 
    : m_associativity(associativity)
    {

}

CacheSetInfoOPT::~CacheSetInfoOPT() {

}



/*
CacheSetOPT
*/
CacheSetOPT::CacheSetOPT(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetInfoOPT* set_info, UInt8 num_attempts)
    :CacheSet(cache_type, associativity, blocksize) {
    m_set_info = set_info;
}

CacheSetOPT::~CacheSetOPT() {

}


int CacheSetOPT::getPrty(UInt32 index) {
    IntPtr addr = m_cache_block_info_array[index]->getAddress();
    std::cout << "cache to access Prty, address" <<  addr << std::endl;
    for (auto it = lookahead_list.begin(); it != lookahead_list.end(); it++) {
        if ((*it).addr == addr) {
            return (*it).prty;
        }
    }

    return 0x7fffffff;
}


UInt32 CacheSetOPT::getReplacementIndex(CacheCntlr *cntlr) {
    // First try to find an invalid block
   for (UInt32 i = 0; i < m_associativity; i++)
   {
      if (!m_cache_block_info_array[i]->isValid())
      {
         return i;
      }
   }

    int selected_index = 0;
    int selected_prty = getPrty(0);
    for (int i = 1; i < m_associativity; i++) {
        UInt64 block_prty = getPrty(i);     
        std::cout << "when cache victim, prty: " <<    block_prty << std::endl;
        if (block_prty > selected_prty) {
            selected_index = i;
            selected_prty = block_prty;
        }
   }
   return selected_index;
}

// accessed_index is the index of the associativity
void CacheSetOPT::updateReplacementIndex(UInt32 accessed_index) {
    //tagToAddress(m_cache_block_info_array[accessed_index]);
    //LOG_PRINT_ERROR("Memory access not in lookahead buffer");
}








