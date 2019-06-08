#include <Servo.h>
// conexiones para arduino
// D4 >> DI (TX)
// D5 >> RO (RX)
// Re >> De >> VCC Transmisor
// Usar 5v no MAX485
// Control
#define led PC13
#define ctrl1 PB12
#define ctrl2 PB13
// Control final mensaje
#define cs '<'

//***************** Controle carro ***************//
// Control Motor lado 1
#define RPWM1 PA8
#define LPWM1 PA2
#define REN1 PB14
#define LEN1 PB15

// Control Motor lado 2
#define RPWM2 PB6
#define LPWM2 PA1
#define REN2 PB1
#define LEN2 PB5

//************ Controle camera *********************//
#define sbpin PB0            // SERVO BASE
#define srcpin PA7           // SERVO ROTACION CAMARA
//#define sapin PB9            // SERVO AUXILIAR "SENSOR DE PROXIMIDADE"

// Matrix de pinos
//              0     1       2     3     4       5     6       7     8     9    10
int pinos[] = {led, ctrl1, ctrl2, RPWM1, LPWM1, RPWM2, LPWM2, REN1, LEN1, REN2, LEN2};

// varibles de control del esclavo
// 0 - seleciona opcio > 1=Mover Carro, 2=Mover Camara, 3=Encender Slave, 4=Encender linterna, 5....
// 1 - tipo de movimiento del carro 2=up, 3=dw, 4=rh, 5=lf
// 2 - tipo de PWM que puede ser cotrolado, por causa del driver solo funciona a 255
// 3 - time=0 no hay control de tiempo, mayor que zero hay control de tiempo
int var_ctrl[] = {0, 0, 0, 0};

String data_in = "", data_send = "";

//********* Inicilizar objetos tipo servo *********//
Servo sb;
Servo src;

int zero_sb = 90; // valor inicial
int zero_src = 90; // valor inicial

int vel = 15; // vel de movimento entre angulos

int angT1 = 90, angT2 = 80; // angulos temporales para mover

/**** Para testar ****/
// bool modo_teste = true; // Seleciona esta opcion para comunicacion serial con PC
bool modo_teste_msg = true; // Valor predeterminado mensaje de ejemplos de control
bool modo_teste = false; // Seleciona esta opcion para comunicacion serial con RS485

unsigned long time_parar = 0;
unsigned long time_espera = 2e6;

// Setup
void setup() {
  // Comunicacion serial
  Serial.begin(115200);
  Serial.setTimeout(10);
  // Comunicacion RS485
  Serial1.begin(115200);
  Serial1.setTimeout(10);

  // inicializo pinos
  for (int i = 0; i < 11; i++) {
    pinMode(pinos[i], OUTPUT);
    digitalWrite(pinos[i], 0);
  }

    //******** Servos asignacion de pines *************/
  sb.attach(sbpin);
  src.attach(srcpin);

  sb.writeMicroseconds(map(zero_sb,0,179,400,2400));
  src.writeMicroseconds(map(zero_src,0,179,700,2100));

  para_tudo();

  blink_led(6, 100);

  time_parar = time_espera + micros();

}

// Loop
void loop() {

  if(modo_teste == false){
    ler_serial();
  }else{
    if(modo_teste_msg == true){
      delay(3000);
      Serial.println("<<<<<<<<<<<  Modo teste  >>>>>>>>>>");
      Serial.println("Ejemplos: ");
      Serial.println("1- Mover carro: 1<2<250<2000<"); 
      Serial.println("2- Mover camara: 2<3<5<15<");
      Serial.println("3- Desactivar RL1: 3<0<0<0<");
      Serial.println("4- Desactivar RL2: 4<0<0<0<");
      Serial.println("5- Desactivar RL2: 5<10<100<0<");
      Serial.println("6- Desactivar RL2: 6<0<0<0<"); // Zera para tudo
      modo_teste_msg = false;
    }
    ler_serial_PC();
  }
}// end loop

/**************************************************************************/
/**************************************************************************/
void ler_serial() {
  data_send = "";
  if (Serial1.available() != 0) {

    for (int i = 0; i < 4; i++) {
      data_in = Serial1.readStringUntil(cs);
      data_send = data_send + data_in + cs;
      var_ctrl[i] = data_in.toInt();
    }

    Serial1.readString(); // Limpa el buffer

    // Para selecionar funciones
    // 1<2,3,4,5<255<1000<
    switch (var_ctrl[0]) {
      case 1: //Mover carro
        Serial.println(data_send);
        // direcion, pwm, time solo en pc
        Mover_carro(var_ctrl[1], var_ctrl[2], var_ctrl[3]);
        break;
      case 2: // Mover camara
        Serial.println(data_send);
        // sel motor, 
        Mover_camara(var_ctrl[1], var_ctrl[2], var_ctrl[3]);
        break;
      case 3:
        Serial.print("Activando rele 1: ");
        if (var_ctrl[1] == 1) { // Para activar el relé
          Serial.println("On");
          digitalWrite(pinos[1], 1); // CTRL1 -- >>> Liga
        } else {
          Serial.println("Off");
          digitalWrite(pinos[1], 0); // CTRL1 -- >>> Desliga
        }
        break;
      case 4:
        Serial.print("Activando rele 2: ");
        if (var_ctrl[1] == 1) { // Para activar el relé
          Serial.println("On");
          digitalWrite(pinos[2], 1); // CTRL2 -- >>> Liga
        } else {
          Serial.println("Off");
          digitalWrite(pinos[2], 0); // CTRL2 -- >>> Desliga
        }
        break;
      case 5:
        Serial.println("Blink led");
        blink_led(var_ctrl[1], var_ctrl[2]);
        break;
      case 6:
        para_tudo();
        break;
    } // end switch
    time_parar = time_espera + micros();
  }else{
    // aqui toca hacer la perdida de conexion y para todoen caso de que no haja conexion por un tiempo
    if (micros() < time_parar){
      para_tudo();
    }
  }

}

/**************************************************************************/
/**************************************************************************/
void ler_serial_PC() {
  data_send = "";
  if (Serial.available() != 0) {

    for (int i = 0; i < 4; i++) {
      data_in = Serial.readStringUntil(cs);
      data_send = data_send + data_in + cs;
      var_ctrl[i] = data_in.toInt();
    }

    Serial.readString(); // Limpa el buffer

    // Para selecionar funciones
    // 1<2,3,4,5<255<1000<
    switch (var_ctrl[0]) {
      case 1: //Mover carro
        Serial.println(data_send);
        // direcion, pwm, time solo en pc
        Mover_carro(var_ctrl[1], var_ctrl[2], var_ctrl[3]);
        break;
      case 2: // Mover camara
        Serial.println(data_send);
        // sel motor, 
        Mover_camara(var_ctrl[1], var_ctrl[2], var_ctrl[3]);
        break;
      case 3:
        Serial.print("Activando rele 1: ");
        if (var_ctrl[1] == 1) { // Para activar el relé
          Serial.println("On");
          digitalWrite(pinos[1], 1); // CTRL1 -- >>> Liga
        } else {
          Serial.println("Off");
          digitalWrite(pinos[1], 0); // CTRL1 -- >>> Desliga
        }
        break;
      case 4:
        Serial.print("Activando rele 2: ");
        if (var_ctrl[1] == 1) { // Para activar el relé
          Serial.println("On");
          digitalWrite(pinos[2], 1); // CTRL2 -- >>> Liga
        } else {
          Serial.println("Off");
          digitalWrite(pinos[2], 0); // CTRL2 -- >>> Desliga
        }
        break;
      case 5:
        Serial.println("Blink led");
        blink_led(var_ctrl[1], var_ctrl[2]);
        break;
      case 6:
        para_tudo();
        break;
    }
  }
}


/*************************************************************************/
/***************** Mover carro -- funciones -- ***************************/
void Mover_carro(int dir, int pwm_m, int time_m) {
  switch (dir) {
    case 1:
      para_tudo();
      break;
    case 2:
      frente(pwm_m, time_m);
      break;
    case 3:
      atras(pwm_m, time_m);
      break;
    case 4:
      esquerda(pwm_m, time_m);
      break;
    case 5:
      direita(pwm_m, time_m);
      break;
  }// End Switch
}

/************* Mover camara ******************/
void Mover_camara(int sel_motor, int ang_ss, int vel_ss) {
  switch (sel_motor) {
    case 1:
      servo_speed(sel_motor, ang_ss, vel_ss);
      break;
    case 2:
      servo_speed(sel_motor, ang_ss, vel_ss);
      break;
    case 3:
      servo_speed(1, zero_sb, vel);
      servo_speed(2, zero_src, vel);
      break;
  }
}

void frente(int pwm_c, int tempo) {
  on_enables();
  analogWrite(pinos[3], pwm_c);
  analogWrite(pinos[4], 0);
  analogWrite(pinos[5], pwm_c);
  analogWrite(pinos[6], 0);
  //soft_pwm_mov(1, pinos[3], pinos[5], pwm_c);
  Serial.println("Movendo para frente");
  if(tempo > 0){
    delay(tempo);
    soft_pwm_mov(0, pinos[3], pinos[5], pwm_c);
    para_tudo();
  }
}

void atras(int pwm_c, int tempo) {
  on_enables();
  analogWrite(pinos[3], 0);
  analogWrite(pinos[4], pwm_c);
  analogWrite(pinos[5], 0);
  analogWrite(pinos[6], pwm_c);
  //soft_pwm_mov(1, pinos[4], pinos[6], pwm_c);
  Serial.println("Movendo para atras");
  if(tempo > 0){
    delay(tempo);
    soft_pwm_mov(0, pinos[4], pinos[6], pwm_c);
    para_tudo();
  }
}

void esquerda(int pwm_c, int tempo) {
  on_enables();
  analogWrite(pinos[3], 0);
  analogWrite(pinos[4], pwm_c);
  analogWrite(pinos[5], pwm_c);
  analogWrite(pinos[6], 0);
  //soft_pwm_mov(1, pinos[4], pinos[5], pwm_c);
  Serial.println("Movendo para esquerda");
  if(tempo > 0){
    delay(tempo);
    soft_pwm_mov(0, pinos[4], pinos[5], pwm_c);
    para_tudo();
  }
}

void direita(int pwm_c, int tempo) {
  on_enables();
  analogWrite(pinos[3], pwm_c);
  analogWrite(pinos[4], 0);
  analogWrite(pinos[5], 0);
  analogWrite(pinos[6], pwm_c);
  //soft_pwm_mov(1, pinos[3], pinos[6], pwm_c);
  Serial.println("Movendo para direita");
  if(tempo > 0){
    delay(tempo);
    soft_pwm_mov(0, pinos[3], pinos[6], pwm_c);
    para_tudo();
  }
}

/***********************************************************************/
// Funcion para cerar todas las salidas
/***********************************************************************/
void para_tudo() {
  off_enables();
  for (int i = 3; i < 7; ++i){
    analogWrite(pinos[i], 0);
  }
  Serial.println("Parar todo");
}

/***********************************************************************/
// Funcion deshabilita los enables
/***********************************************************************/
void off_enables() {
  for (int i = 7; i < 11; ++i){
    digitalWrite(pinos[i], 0);
  }
}

/***********************************************************************/
// Funcion habilita los enables
/***********************************************************************/
void on_enables() {
  for (int i = 7; i < 11; ++i){
    digitalWrite(pinos[i], 1);
  }
}

/***********************************************************************/
// funcion mover servo smoot servos
/***********************************************************************/
void servo_speed(int motorS, int angulo, long velocidade) {
  int cp = 0;
  bool salir = 1;

  if (motorS == 1) {
    while (salir == 1) {
      //Serial.print("Valor cp: ");
      //Serial.println(cp);
      // movimenta
      if (angT1 < angulo) {
        cp += 1;
        int cpt = angT1 + cp;
        sb.writeMicroseconds(map(cpt, 0, 179, 400, 2400));
      }

      if (angT1 > angulo) {
        cp += 1;
        int cpt = angT1 - cp;
        sb.writeMicroseconds(map(cpt, 0, 179, 400, 2400));
      }
      
      //para
      if (cp == abs(angT1 - angulo)) {
        salir = 0;
      }
      
      delay(velocidade);
    }
    angT1 = angulo;
  } else {
    
    while (salir == 1) {
      //Serial.print("Valor cp: ");
      //Serial.println(cp);
      
      // movimenta
      if (angT2 < angulo) {
        cp += 1;
        int cpt = angT2 + cp;
        src.writeMicroseconds(map(cpt, 0, 179, 400, 2400));
      }

      if (angT2 > angulo) {
        cp += 1;
        int cpt = angT2 - cp;
        src.writeMicroseconds(map(cpt, 0, 179, 400, 2400));
      }
      //para
      if (cp == abs(angT2 - angulo)) {
        salir = 0;
      }
      delay(velocidade);
    }
    angT2 = angulo;
  }
}

/***********************************************************************/
// Fx blink led
/***********************************************************************/
void blink_led(int qtd, int time_led){
  for (int i = 0; i < qtd; ++i){
    digitalWrite(pinos[0], 1);
    delay(time_led * 2);
    digitalWrite(pinos[0], 0);
    delay(time_led);
  }
}

/***********************************************************************/
// Fx movimiento suave de pwm
/***********************************************************************/
void soft_pwm_mov(int dir, int pino1, int pino2, int val_PWM_Motor){
  if ( dir == 1){
    for (int i = 0; i < val_PWM_Motor; ++i){
      analogWrite(pino1, i);
      analogWrite(pino2, i);
      delay(3);
    }
  }else{
    for (int i = val_PWM_Motor; i > 0; --i){
      analogWrite(pino1, i);
      analogWrite(pino2, i);
      delay(3);
    }
  }
}
