#include "vmsim.h"

void setup() {
  Serial.begin(9600);
  delay(5000);
}

void loop(){
    Serial.println(".");
    //String IMEI = String(vm_sim_has_card(1));
    Serial.println(vm_sim_max_card_count());
    delay(1000);
}
