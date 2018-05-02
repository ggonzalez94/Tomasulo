//
// Created by aj on 3/8/16.
//

#include "Instruction.h"

Instruction::Instruction() {
    rd = 0;
    rs = 0;
    rt = 0;
    op = AddOp;
    issueClock = 0;
    executeClockBegin = 0;
    executeClockEnd = 0;
    writebackClock = 0;
}

Instruction::Instruction(int RD,int RS, int RT, operaciones OP) {
    rd = RD;
    rs = RS;
    rt = RT;
    op = OP;
    issueClock = 0;
    executeClockBegin = 0;
    executeClockEnd = 0;
    writebackClock = 0;
}

int Instruction::get_operation(){
  return op;
}

int Instruction::get_issue_clock(){
  return issueClock;
}

int Instruction::get_execeute_clock_begin(){
  return executeClockBegin;
}

int Instruction::get_execute_clock_end(){
  return executeClockEnd;
}

int Instruction::get_writeback_clock(){
  return writebackClock;
}

int Instruction::get_destination_register(){
  return rd;
}

int Instruction::get_rs_register(){
  return rs;
}

int Instruction::get_rt_register(){
  return rt;
}

void Instruction::set_operation(operaciones op){
  this->op = op;
}

void Instruction::set_issue_clock(int issueClock){
  this->issueClock= issueClock;
}

void Instruction::set_writeback_clock(int writebackClock){
  this->writebackClock= writebackClock;
}

void Instruction::set_execute_clock_begin(int executeClockBegin){
  this->executeClockBegin= executeClockBegin;
}

void Instruction::set_execute_clock_end(int executeClockEnd){
  this->executeClockEnd= executeClockEnd;
}
