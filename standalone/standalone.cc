#include "simulator.h"
#include "handle_args.h"
#include "config.hpp"
#include "trace_manager.h"
#include "magic_client.h"
#include "logmem.h"
#include "exceptions.h"
#include "sim_api.h"
#include <iostream>
#include <fstream>


std::list<Entry> lookahead_list = std::list<Entry>();
UInt64 curr_core_access = 0;
std::unordered_map<UInt64, std::list<int>*> address2count = std::unordered_map<UInt64, std::list<int>*>();
int access_offset = 0;


template <typename T> 
bool dumpList2File(std::list<T> dates, String file_path);
void readListFromFile(std::list<Entry> &l,String file_path);


void initLookaheadBuffer(std::list<Entry> &buffer);
void rawRepair(std::list<Entry> &buffer, Entry &entry, int true_prty, int depth);


bool find(std::list<Entry> buffer, UInt64 addr, int depth);
bool referenced(UInt64 key);


// void lookupAndUpdate(std::list<Entry> & lookahead_buffer, UInt64 addr);
// int lookup(std::list<Entry> & lookahead_buffer,UInt64 addr);
// void update(std::list<Entry> & lookahead_buffer,int hit_depth);


// template <typename T> 
// bool dumpList2File(std::list<T> dates, String file_path) {
//    std::ofstream outfile;
//    outfile.open(file_path.c_str());
//    for (auto it = dates.begin(); it != dates.end(); it++) {
//       outfile << ((*it)) << std::endl;
//    }
//    outfile.close();

//    return true;
// }

// void readListFromFile(std::list<Entry> &l,String file_path) {
//    std::ifstream infile;
//    infile.open(file_path.c_str());
//    String buffer;
//    int prty = 0;
//    while (std::getline(infile, buffer)) {
//       l.push_back({UInt64(buffer.c_str()), prty});
//       prty--;
//    }
//    infile.close();
   
// }

// void rawRepair(std::list<Entry> &buffer, Entry &entry, int true_prty, int depth) {
//     int dummy_prty = entry.prty;
//     entry.prty = true_prty;
//     auto entry_ptr = buffer.begin();
//     while (entry_ptr->addr != true_prty) {
//         if ( entry_ptr->prty < 0 
//             && entry_ptr->prty > dummy_prty) {
//                 if (entry_ptr->prty < entry.prty) {
//                     auto temp = entry;
//                     entry = *entry_ptr; 
//                     *entry_ptr = temp; 
//                 }
//         }
//         entry_ptr++;
//     }
// }
// void initLookaheadBuffer(std::list<Entry> &buffer) {
//     int prty = (uint32_t)-1;
//     int iteration_cnt = 0;
//     for (auto it = buffer.begin(); it != buffer.end(); it++) {
//         iteration_cnt++;
//         if (referenced(it->addr)) {
//             if (find(buffer, it->addr, iteration_cnt)) {
//                 it->prty = prty;
//                 prty--;
//             }else {
//                 rawRepair(buffer, (*it), prty, iteration_cnt);
//                 prty--;
//             }
//         }
//         if (!referenced(it->addr)) {
//            referenced_map[it->addr] = true;
//         }

//         if (iteration_cnt %100 == 0 ) {
//            std::cout << "initLookaheadBuffer:" << iteration_cnt << std::endl;
//         }
//     }
// }

// bool find(std::list<Entry> lookahead_buffer, UInt64 addr, int depth) {
//     int iteration_cnt = 0;
//     for (auto it = lookahead_buffer.begin(); it != lookahead_buffer.end(); it++) {
//         if ( it->addr == addr ) {
//             return true;
//         }   
//         iteration_cnt++;
//         if (iteration_cnt >= depth) {
//             break;
//         }
//     }
//     return false;
// }

// bool referenced(UInt64 key) {
//    if (referenced_map.find(key) != referenced_map.end()) {
//       return true;
//    }
//    return false;
// }

// void lookupAndUpdate(std::list<Entry> & lookahead_buffer, UInt64 addr){
//    int depth = lookup(lookahead_buffer, addr);
//    update(lookahead_buffer, depth);
// }

// int lookup(std::list<Entry> &lookahead_buffer, UInt64 addr) {
//     int hit_depth = -1;
//     int cnt = 0;
//     for(auto it = lookahead_buffer.begin(); it != lookahead_buffer.end(); it++) {
//         if ((*it).addr == addr) {
//             hit_depth = cnt;
//             break;
//         }
//         cnt++;
//     }
//     return hit_depth;
// }

// void update(std::list<Entry> &lookahead_buffer, int hit_depth){
//     if (hit_depth == 0) {
//         return ;
//     }
//     auto deleted_line = lookahead_buffer.front();
//     int cnt = 0;
//     for (auto it = lookahead_buffer.begin()++; it != lookahead_buffer.end(); it++){
//         int deleted_prty = deleted_line.prty;
//         int current_prty = (*it).prty;
//         if (deleted_prty > current_prty) {
//             auto temp = *it;
//             *it = deleted_line;
//             deleted_line = temp; 
//         }
//         cnt++;
//         it++;
//         if ( cnt = hit_depth ) {
//             break;
//         }
//     }
// }


int main (int argc, char* argv[]) {

   // Set thread name for Sniper-in-Sniper simulations
   SimSetThreadName("main");

   // To make sure output shows up immediately, make stdout and stderr line buffered
   // (if we're writing into a pipe to run-graphite, or redirected to a file by the job runner, the default will be block buffered)
   setvbuf(stdout, NULL, _IOLBF, 0);
   setvbuf(stderr, NULL, _IOLBF, 0);

   const char *ld_orig = getenv("SNIPER_SCRIPT_LD_LIBRARY_PATH");
   if (ld_orig)
      setenv("LD_LIBRARY_PATH", ld_orig, 1);

   registerExceptionHandler();

   string_vec args;

   // Set the default config path if it isn't
   // overwritten on the command line.
   String config_path = "carbon_sim.cfg";

   parse_args(args, config_path, argc, argv);

   config::ConfigFile *cfg = new config::ConfigFile();
   cfg->load(config_path);

   handle_args(args, *cfg);

   Simulator::setConfig(cfg, Config::STANDALONE);
   
   auto root = cfg->getRoot();
   String cfg_tree = cfg->showTree(root);
   //std::cout << cfg_tree << std::endl;

   Simulator::allocate();
   Sim()->start();

   // config::Config shouldn't be called outside of init/fini
   // With Sim()->hideCfg(), we let Simulator know to complain when someone does call Sim()->getCfg()
   Sim()->hideCfg();

   //std::cout << "[standalone]: Start" << std::endl;
   LOG_ASSERT_ERROR(Sim()->getTraceManager(), "In standalone mode but there is no TraceManager!");


   Sim()->getTraceManager()->run();
   //std::cout << "[standalone]: Single Run" << std::endl;

   // Iterate over a number of application runs. This can allow for a warmup pass before running an application
   for (int i = 1 ; i < Sim()->getCfg()->getInt("traceinput/num_runs") ; i++)
   {
      Sim()->getTraceManager()->cleanup();
      Sim()->getTraceManager()->setupTraceFiles(i);
      Sim()->getTraceManager()->init();
      Sim()->getTraceManager()->run();
   }


   Simulator::release();
   delete cfg;

   //dumpList2File(record_for_lookahead, "./lookahead");
   return 0;
}
