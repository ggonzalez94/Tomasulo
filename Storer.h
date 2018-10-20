#ifndef TOMASULO_STORER_H
#define TOMASULO_STORER_H

#include "operaciones.h"

class Storer {
    private:
        bool busy;
        int dir;
        int Qi;
        int Vi;
        int lat;
        int result;
        bool resultReady;
        int instNum;
        int ISSUE_Lat;
        int WRITEBACK_Lat;

    //**** Methods
    public:
        Storer();
        bool is_busy();
        void set_busy();
        void set_free();
        int get_dir();
        int get_Qi();
        int get_Vi();
        void set_dir(int);
        void set_Qi(int);
        void set_Vi(int);
        int get_lat();
        void set_lat(int);
        int get_result();
        void set_result(int);
        bool is_result_ready();
        void set_result_ready(bool);
        int get_instruction_number();
        void set_instruction_number(int);
        int get_issue_latency();
        void set_issue_latency(int);
        int get_writeback_latency();
        void set_writeback_latency(int);
        void increase_latency();
        void increase_issue_latency();
        void increase_writeback_latency();

};


#endif //TOMASULO_RS_H
