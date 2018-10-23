//

#include "RegisterStatus.h"

RegisterStatus::RegisterStatus(){
    busy = false;
    Qi = 0;
}
RegisterStatus::RegisterStatus(int RegStatusEmpty) {
    Qi = RegStatusEmpty;
}
bool RegisterStatus::is_busy(){
  return busy;
}
int RegisterStatus::get_estacion(){
  return Qi;
}
void RegisterStatus::set_estacion(int estacion){
  Qi = estacion;
  return;
}
