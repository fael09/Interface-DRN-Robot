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
///-----
/*#define ENA PB11         // ENABLE MOTOR IZQ - EN1
#define ENB PB10        // ENABLE MOTOR DER - EN2

#define PWM1 PA8         // ENABLE MOTOR IZQ - EN1
#define PWM2 PB6        // ENABLE MOTOR DER - EN2

///----- MOTOR der
#define IN1 PB14 // CCW1
#define IN2 PB15 // CC1

///----- MOTOR izq
#define IN3 PB1  // CCW2
#define IN4 PB5  // CC2 */

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
int pinos[] = {led, ctrl1, ctrl2, RPWM1, LPWM1, RPWM2, LPWM2, REN1, LEN1, REN2, LEN2, sbpin, srcpin};

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

// Setup
void setup() {
  // Comunicacion serial
  Serial.begin(115200);
  Serial.setTimeout(10);
  // Comunicacion RS485
  Serial1.begin(115200);
  Serial1.setTimeout(10);

  // inicializo pinos
  for (int i = 0; i < 13; i++) {
    pinMode(pinos[i], OUTPUT);
    digitalWrite(pinos[i], 0);
  }

  //******** Servos asignacion de pines *************/
  sb.attach(sbpin);
  src.attach(srcpin);

  sb.writeMicroseconds(map(zero_sb,0,179,400,2400));
  src.writeMicroseconds(map(zero_src,0,179,700,2100));

}

// Loop
void loop() {
  ler_serial();
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
        Mover_carro(var_ctrl[1], var_ctrl[2], var_ctrl[3]);
        break;
      case 2: // Mover camara
        Serial.println(data_send);
        Mover_camara(var_ctrl[1], var_ctrl[2], var_ctrl[3]);
        break;
      case 3:
        if (var_ctrl[1] == 1) { // Para activar el relé
          digitalWrite(pinos[1], 1); // CTRL1 -- >>> Liga
        } else {
          digitalWrite(pinos[1], 0); // CTRL1 -- >>> Desliga
        }
        break;
      case 4:
        if (var_ctrl[1] == 1) { // Para activar el relé
          digitalWrite(pinos[2], 1); // CTRL2 -- >>> Liga
        } else {
          digitalWrite(pinos[2], 0); // CTRL2 -- >>> Desliga
        }
        break;
      case 5:
        para_tudo(0);
        break;
    }

  }

}


/*************************************************************************/
/***************** Mover carro -- funciones -- ***************************/
void Mover_carro(int dir, int pwm_m, int time_m) {
  switch (dir) {
    case 1:
      para_tudo(0);
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
  pwm_control(pwm_c);
  digitalWrite(pinos[5], 1);
  digitalWrite(pinos[6], 0);
  digitalWrite(pinos[7], 1);
  digitalWrite(pinos[8], 0);
  Serial.println("Movendo para frente");
}

void atras(int pwm_c, int tempo) {
  pwm_control(pwm_c);
  digitalWrite(pinos[5], 0);
  digitalWrite(pinos[6], 1);
  digitalWrite(pinos[7], 0);
  digitalWrite(pinos[8], 1);
  Serial.println("Movendo para atras");
}

void esquerda(int pwm_c, int tempo) {
  pwm_control(pwm_c);
  digitalWrite(pinos[5], 0);
  digitalWrite(pinos[6], 1);
  digitalWrite(pinos[7], 1);
  digitalWrite(pinos[8], 0);
  Serial.println("Movendo para esquerda");
}

void direita(int pwm_c, int tempo) {
  pwm_control(pwm_c);
  digitalWrite(pinos[5], 1);
  digitalWrite(pinos[6], 0);
  digitalWrite(pinos[7], 0);
  digitalWrite(pinos[8], 1);
  Serial.println("Movendo para direita");
}

void para_tudo(int pwm_c) {
  pwm_control(pwm_c);
  digitalWrite(pinos[3], 0);
  digitalWrite(pinos[4], 0);
  digitalWrite(pinos[5], 0);
  digitalWrite(pinos[6], 0);
  digitalWrite(pinos[7], 0);
  digitalWrite(pinos[8], 0);
  Serial.println("Parar todo");
}

void pwm_control(int pwm) {
  analogWrite(pinos[11], pwm);
  analogWrite(pinos[12], pwm);
  analogWrite(pinos[11], pwm);
  analogWrite(pinos[12], pwm);
}

void off_enables() {
  digitalWrite(pinos[11], 0);
  digitalWrite(pinos[11], 0);
  digitalWrite(pinos[11], 0);
  digitalWrite(pinos[11], 0);
}

void on_enables() {
  digitalWrite(pinos[11], 0);
  digitalWrite(pinos[11], 0);
  digitalWrite(pinos[11], 0);
  digitalWrite(pinos[11], 0);
}




// funcion mover servo smoot servos
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