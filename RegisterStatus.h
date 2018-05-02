//
// Created by aj on 3/8/16.
//

#ifndef TOMASULO_REGSTATUS_H
#define TOMASULO_REGSTATUS_H


class RegisterStatus {
    private:
        bool busy;
        int Qi; //La estacion de reserva de la cual provendra el resultado
    // Methods
    public:
        RegisterStatus();
        RegisterStatus(int);
        bool is_busy();
        int get_estacion();
        void set_estacion(int estacion);
};


#endif //TOMASULO_REGSTATUS_H
