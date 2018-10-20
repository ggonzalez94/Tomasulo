//
// Created by aj on 3/8/16.
//

#include "Memory.h"

Memory::Memory(){
  memory_size = 64;
  for (int i=0;i<memory_size;i++){
    memory.push_back(99);
  }
}

int Memory::read_memory(int addr){
  return memory[int(addr)];
}

void Memory::write_memory(int addr,int value){
  memory[int(addr)] = value;
  return;
}
