#include "Loader.h"

Loader::Loader(){
    busy = false;
    lat = 0;
    result = 0;
    resultReady = false;
    dir = 0;
    valor = 0
    instNum = 100000;
    ISSUE_Lat = 0;
    WRITEBACK_Lat = 0;
}

bool Loader::is_busy(){
  return busy;
}

void Loader::set_busy(){
  busy = true;
}

void Loader::set_free(){
  busy = false;
}

int Loader::get_dir(){
  return dir;
}

void Loader::set_dir(int direccion){
  dir = direccion;
}

int Loader::get_valor(){
  return valor;
}

void Loader::set_valor(int new_valor){
  valor = new_valor;
}

int Loader::get_lat(){
  return lat;
}

void Loader::set_lat(int latencia){
   lat = latencia;
}

operaciones Loader::get_operation(){
  return op;
}

void Loader::set_operation(operaciones operacion){
  op = operacion;
}

int Loader::get_result(){
  return result;
}

void Loader::set_result(int resultado){
  result = resultado;
}

bool Loader::is_result_ready(){
  return resultReady;
}

void Loader::set_result_ready(bool ready){
  resultReady = ready;
}

int Loader::get_instruction_number(){
  return instNum;
}

void Loader::set_instruction_number(int number){
  instNum = number;
}

int Loader::get_issue_latency(){
  return ISSUE_Lat;
}

void Loader::set_issue_latency(int latency){
  ISSUE_Lat = latency;
}

int Loader::get_writeback_latency(){
  return WRITEBACK_Lat;
}

void Loader::set_writeback_latency(int latency){
  WRITEBACK_Lat = latency;
}

void Loader::increase_latency(){
  lat = lat+1;
}

void Loader::increase_issue_latency(){
  ISSUE_Lat = ISSUE_Lat+1;
}

void Loader::increase_writeback_latency(){
  WRITEBACK_Lat = WRITEBACK_Lat+1;
}
