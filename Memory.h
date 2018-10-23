//

#ifndef TOMASULO_MEMORY_H
#define TOMASULO_MEMORY_H

#include <vector>
using namespace std;
class Memory{
  private:
    vector<int> memory;
    int memory_size;
  public:
    Memory();
    int read_memory(int addr);
    void write_memory(int addr, int value);
};

#endif
