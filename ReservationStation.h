//
// Created by aj on 3/8/16.
//

#ifndef TOMASULO_RS_H
#define TOMASULO_RS_H

#include "operaciones.h"

class ReservationStation {
    public:
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
};


#endif //TOMASULO_RS_H
