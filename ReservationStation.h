//
// Created by aj on 3/8/16.
//

#ifndef TOMASULO_RS_H
#define TOMASULO_RS_H

#include "operaciones.h"

class ReservationStation {
    private:
        bool busy;
        int Qj; //estaciones de donde va a provenir el valor(0 si esta disponible)
        int Qk;
        int Vj; //Valores de los operandos
        int Vk;
        int lat;
        operaciones op;
        int result;
        bool resultReady;
        int instNum;
        int ISSUE_Lat;
        int WRITEBACK_Lat;

    //**** Methods
    public:
        ReservationStation();
        ReservationStation(operaciones, int);
        bool is_busy();
        void set_busy();
        void set_free();
        int get_estacion_Qj();
        int get_estacion_Qk();
        void set_estacion_Qj(int);
        void set_estacion_Qk(int);
        int get_Vj();
        int get_Vk();
        void set_Vj(int);
        void set_Vk(int);
        int get_lat();
        void set_lat(int);
        operaciones get_operation();
        void set_operation(operaciones);
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
