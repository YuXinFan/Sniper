#include "cache_set_opt.h"
#include "log.h"
#include "stats.h"

extern std::list<Entry> lookahead_list;
/*
Help Function 
*/






/*
CacheSetInfoOPT
*/
CacheSetInfoOPT::CacheSetInfoOPT(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts) 
    : m_associativity(associativity)
   , m_attempts(NULL) {

}

CacheSetInfoOPT::~CacheSetInfoOPT() {

}

int CacheSetInfoOPT::getPrty(UInt32 index) {
    // from cache line index find data address \
    then find address's prty in lookahead; 
    return 0;
}

int CacheSetOPT::getPrty(UInt32 index) {
    IntPtr addr = m_cache_block_info_array[index]->getAddress();

    for (auto it = lookahead_list.begin(); it != lookahead_list.end(); it++) {
        if ((*it).addr == addr) {
            return (*it).prty;
        }
    }

    return 0;
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
        if (block_prty == 0) {
           selected_index = i;
           break;
       }
        
        if (block_prty > selected_prty) {
            selected_index = i;
        }
   }
   return selected_index;
}

// accessed_index is the index of the associativity
void CacheSetOPT::updateReplacementIndex(UInt32 accessed_index) {
    //tagToAddress(m_cache_block_info_array[accessed_index]);
    //LOG_PRINT_ERROR("Memory access not in lookahead buffer");
}


void CacheSetOPT::stackRepair(UInt64 addr, int timer) {

}

void CacheSetOPT::update(int hit_depth){
    if (hit_depth == 0) {
        return ;
    }
    auto deleted_line = buffer.front();
    int cnt = 0;
    for (auto it = buffer.begin()++; it != buffer.end(); it++){
        int deleted_prty = deleted_line.prty;
        int current_prty = (*it).prty;
        if (deleted_prty > current_prty) {
            auto temp = *it;
            *it = deleted_line;
            deleted_line = temp; 
        }
        cnt++;
        it++;
        if ( cnt = hit_depth ) {
            break;
        }
    }
}

int CacheSetOPT::lookup(UInt64 addr) {
    int hit_depth = -1;
    int cnt = 0;
    for(auto it = buffer.begin(); it != buffer.end(); it++) {
        if ((*it).addr == addr) {
            hit_depth = cnt;
            break;
        }
        cnt++;
    }
    return hit_depth;
}





