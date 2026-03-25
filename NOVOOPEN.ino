int PONTE_H[2] = {5,6}; //PINO 5 É O RPWM GIRANDO PARA A DIREITA E O PINO 6 LPWM GIRANDO PARA A ESQUERDA
int PONTE_H_H[2] = {7,8};
int actponteh[2] = {A0,A1};
int actmr_f[2] = {A2,A3};
int actmr_r[2] = {A4,A5};
int i = 0;
int ACELERADOR = 9;
  int AceState = 0;
  int AcePState = 0;
  int Ace = 0;
  int eps = 6;
  int mov = 0;
  int aux = 0;
int SWITCH = 10;
int Sw = 0;
byte SwState = 0;


void setup() {

  Serial.begin(9600);

  for(i=0;i<2;i++){
    pinMode(PONTE_H[i], OUTPUT);
    pinMode(PONTE_H_H[i], OUTPUT);
    pinMode(actponteh[i], OUTPUT);
    pinMode(actmr_f[i], OUTPUT);
    pinMode(actmr_r[i], OUTPUT);

  }

  pinMode(ACELERADOR, INPUT_PULLUP);
  pinMode(SWITCH, INPUT_PULLUP);
//  pinMode(POT, INPUT_PULLUP);
}

void loop() {
//  potenc();
  logica();
  if(aux != 0 ){
   // S_RPM();
    //DistSonar();
    //Tensao_Corrente();
  }

}

//SWITCH QUE DANIFICOU
int SW(){
  Sw = map(constrain(pulseIn(SWITCH, HIGH), 1000, 1978), 1000, 2000, 0, 255);
  //Serial.print("SW : ");
  //Serial.println(Sw);
  if(pulseIn(SWITCH,HIGH)<1200){
    SwState = 0;
    //Serial.println("SW POS1");
  }
  else if((Sw >= 90) && (Sw < 210)){
   SwState = 127; 
  }
  else if(pulseIn(SWITCH,HIGH)>1400){
    SwState = 255; 
   // Serial.println("SW POS2");
  }
  return SwState;
}


int logica(){

  byte SwS;
  SwS = SW();
  Acelerador();
  if((SwState == 0) && (aux != 0)){
        digitalWrite(PONTE_H_H[0],LOW);
        digitalWrite(PONTE_H_H[1],LOW);
        digitalWrite(actponteh[0],LOW);
        digitalWrite(actponteh[1],LOW);
        digitalWrite(actmr_f[0],LOW);
        digitalWrite(actmr_f[1],LOW);
        digitalWrite(actmr_r[0],LOW);
        digitalWrite(actmr_r[1],LOW);

        Serial.println("ENTROU CONDIÇÃO 0");
        aux = 0;        
  }
  else if((SwState == 127) && (aux != 127)){
        digitalWrite(PONTE_H_H[0],HIGH);
        digitalWrite(PONTE_H_H[1],HIGH);
        digitalWrite(actponteh[0],LOW);
        digitalWrite(actponteh[1],LOW);
        digitalWrite(actmr_f[0],LOW);
        digitalWrite(actmr_f[1],LOW);
        digitalWrite(actmr_r[0],LOW);
        digitalWrite(actmr_r[1],LOW);
      Serial.println("ENTROU CONDIÇÃO 1");
      aux = 127;
  }
    else if(SwState == 255){
      if((mov == 0) && (aux !=0)){ 
        digitalWrite(PONTE_H_H[0],LOW);
        digitalWrite(PONTE_H_H[1],LOW);
        digitalWrite(actponteh[0],HIGH);
        digitalWrite(actponteh[1],HIGH);
        digitalWrite(actmr_f[0],LOW);
        digitalWrite(actmr_f[1],LOW);
        digitalWrite(actmr_r[0],LOW);
        digitalWrite(actmr_r[1],LOW);
        Serial.println("ENTROU CONDIÇÃO 2");
        aux = 0;
      }
      else if((mov == 1) && (aux != 255)){ 
        digitalWrite(PONTE_H_H[0],LOW);
        digitalWrite(PONTE_H_H[1],LOW);
        digitalWrite(actponteh[0],HIGH);
        digitalWrite(actponteh[1],HIGH);
        digitalWrite(actmr_f[0],HIGH);
        digitalWrite(actmr_f[1],HIGH);
        digitalWrite(actmr_r[0],LOW);
        digitalWrite(actmr_r[1],LOW);
        Serial.println("ENTROU CONDIÇÃO 3");
        aux = 255;  
      }
      else if((mov == -1) && (aux != -255)){ 
        digitalWrite(PONTE_H_H[0],LOW);
        digitalWrite(PONTE_H_H[1],LOW);
        digitalWrite(actponteh[0],HIGH);
        digitalWrite(actponteh[1],HIGH);
        digitalWrite(actmr_f[0],LOW);
        digitalWrite(actmr_f[1],LOW);
        digitalWrite(actmr_r[0],HIGH);
        digitalWrite(actmr_r[1],HIGH);
        Serial.println("ENTROU CONDIÇÃO 4");
        aux = -255;  
      }
  }
}


int Acelerador() {  
  Ace = pulseIn(ACELERADOR,HIGH);
 // Serial.print("ace: ");
  //Serial.println(Ace);
  AceState = constrain(map(Ace,989,1974,-255,255),-255,255);
  Serial.print("AceState: ");
  Serial.println(AceState);
      //COMO TEM VALORES QUE OSCILAM RAPIDO ENTRE eps*mult*2 UNIDADES E O VALOR ACABA VARIANDO EM +/-12 O IF É PARA DEIXAR UMA ZONA MORTA NO MEIO DO ACELERADOR
      if (abs(AceState) <= eps*2){
        AceState = 0; 
        mov = 0;
      }
  
      //AS VEZES O VALOR NÃO ALCANÇA O -255, ESSE IF É PARA FORÇAR O -255 POR CONTA DAS OSCILAÇÕES
      else if (AceState <= -255 + (eps)){
        AceState = -255; 
        mov = -1; 
      }
  
      
      //AS VEZES O VALOR NÃO ALCANÇA O 255, ESSE IF É PARA FORÇAR O 255 POR CONTA DAS OSCILAÇÕES
      else if (AceState >= 255 - (eps+2)){
        AceState = 255; 
        mov = 1; 
      }
  if (abs(AceState - AcePState)> eps){
    //Serial.print("ACESTATE: ");
    //Serial.println(AceState);
    ponteh();
    AcePState = AceState;
  }
}

void ponteh(){
      if (abs(AceState) <= 16){
        Serial.println("ENTROU NO 0 ");
        analogWrite(PONTE_H[1], 0);
        analogWrite(PONTE_H[0], 0);
      }
      else if (AceState < -16){
       Serial.println("ENTROU RE"); 
        analogWrite(PONTE_H[1], abs(AceState));
        analogWrite(PONTE_H[0], 0);
      }
      else if (AceState > 16){
        Serial.println("ENTROU AVANTE ");
        analogWrite(PONTE_H[0], AceState);
        analogWrite(PONTE_H[1], 0);
      }
 }
