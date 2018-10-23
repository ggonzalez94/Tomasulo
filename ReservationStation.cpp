//

#include "ReservationStation.h"

ReservationStation::ReservationStation(){
    busy = false;
    op = AddOp;
    lat = 0;
    result = 0;
    resultReady = false;
    Qj = 1;
    Qk = 1;
    Vj = 0;
    Vk = 0;
    instNum = 100000;
    ISSUE_Lat = 0;
    WRITEBACK_Lat = 0;
}
ReservationStation::ReservationStation(operaciones OP, int RSoperandAvailable){
    busy = false;
    op = OP;
    lat = 0;
    result = 0;
    resultReady = false;
    Qj = RSoperandAvailable;
    Qk = RSoperandAvailable;
    Vj = 0;
    Vk = 0;
    instNum = 100000;
    ISSUE_Lat = 0;
    WRITEBACK_Lat = 0;

}

bool ReservationStation::is_busy(){
  return busy;
}

void ReservationStation::set_busy(){
  busy = true;
}

void ReservationStation::set_free(){
  busy = false;
}

int ReservationStation::get_estacion_Qj(){
  return Qj;
}

int ReservationStation::get_estacion_Qk(){
  return Qk;
}

void ReservationStation::set_estacion_Qj(int estacion){
  Qj = estacion;
}

void ReservationStation::set_estacion_Qk(int estacion){
  Qk = estacion;
}

int ReservationStation::get_Vj(){
  return Vj;
}

int ReservationStation::get_Vk(){
  return Vk;
}

void ReservationStation::set_Vj(int valor){
  Vj = valor;
}

void ReservationStation::set_Vk(int valor){
  Vk = valor;
}

int ReservationStation::get_lat(){
  return lat;
}

void ReservationStation::set_lat(int latencia){
   lat = latencia;
}

operaciones ReservationStation::get_operation(){
  return op;
}

void ReservationStation::set_operation(operaciones operacion){
  op = operacion;
}

int ReservationStation::get_result(){
  return result;
}

void ReservationStation::set_inmediato(int inmediato){
  this->inmediato = inmediato;
}

int ReservationStation::get_inmediato(){
  return inmediato;
}

void ReservationStation::set_result(int resultado){
  result = resultado;
}

bool ReservationStation::is_result_ready(){
  return resultReady;
}

void ReservationStation::set_result_ready(bool ready){
  resultReady = ready;
}

int ReservationStation::get_instruction_number(){
  return instNum;
}

void ReservationStation::set_instruction_number(int number){
  instNum = number;
}

int ReservationStation::get_issue_latency(){
  return ISSUE_Lat;
}

void ReservationStation::set_issue_latency(int latency){
  ISSUE_Lat = latency;
}

int ReservationStation::get_writeback_latency(){
  return WRITEBACK_Lat;
}

void ReservationStation::set_writeback_latency(int latency){
  WRITEBACK_Lat = latency;
}

void ReservationStation::increase_latency(){
  lat = lat+1;
}

void ReservationStation::increase_issue_latency(){
  ISSUE_Lat = ISSUE_Lat+1;
}

void ReservationStation::increase_writeback_latency(){
  WRITEBACK_Lat = WRITEBACK_Lat+1;
}
