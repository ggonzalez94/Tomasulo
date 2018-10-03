//
// Created by aj on 3/8/16.
//

#ifndef TOMASULO_INSTRUCTION_H
#define TOMASULO_INSTRUCTION_H

#include "operaciones.h"


class Instruction {
    private:
        int rd; // rd <- rs + rt
        int rs;
        int rt;
        operaciones op; // add,sub,mult,div
        int issueClock;
        int executeClockBegin;
        int executeClockEnd;
        int writebackClock;
        int inmediato;
    //**** Class methods
    public:
        Instruction();
        Instruction(int,int,int,int,operaciones);
        int get_operation();
        void set_operation(operaciones);
        int get_issue_clock();
        void set_issue_clock(int);
        int get_execeute_clock_begin();
        void set_execute_clock_begin(int);
        int get_execute_clock_end();
        void set_execute_clock_end(int);
        int get_writeback_clock();
        void set_writeback_clock(int);
        int get_destination_register();
        int get_rs_register();
        int get_rt_register();
        int get_inmediato();
        void set_inmediato(int);
};

#endif //TOMASULO_INSTRUCTION_H
