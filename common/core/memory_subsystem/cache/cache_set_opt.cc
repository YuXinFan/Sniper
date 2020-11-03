#include "cache_set_opt.h"
#include "log.h"
#include "stats.h"

UInt32 CacheSetOPT::getReplacementIndex(CacheCntlr *cntlr) {

}

// accessed_index is the index of the associativity
void CacheSetOPT::updateReplacementIndex(UInt32 accessed_index) {
    //tagToAddress(m_cache_block_info_array[accessed_index]);
}


void CacheSetOPT::lookaheadOnce(UInt64 addr) {
    if (isAccessed.at(addr)) {
        if (isExecuted(addr)) {
            buffer.push_back({addr, dummpy_timer});
            dummpy_timer--;
            stackRepair(addr, timer);
        }else {
            buffer.push_back({addr,timer});
            timer++;
        }
    }
}

void CacheSetOPT::stackRepair(UInt64 addr, int timer) {

    for (int i = 0; i < buffer.size(); i++) {
        Entry deleted_entry = buffer.front();


    }
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

bool CacheSetOPT::isExecuted(UInt64 addr) {
    bool r = true;
    for(auto it = buffer.begin(); it != buffer.end(); it++) {
        if ((*it).addr == addr) {
            r = false;
            break;
        }
    }
    return r;
}