// Cobnexiones del joystick
#include <SoftwareSerial.h>
// Control
#define led 13
#define rele_ctrl 2
#define sw_jt 3
// Leds controle
#define up A1
#define dw A3
#define rh A0
#define lf A2
// Cobnexiones del joystick
#define inx A5
#define iny A4

// caracter de separacion
#define cs '<'

// Cantidd de offset para valores de calibracion
const int vlc = 40;

// variables de posicion
int valx = 0, valy = 0;

bool car_cam = 1; // control del switch para pasar de control de carro/camara

// cantidad de muestras para filtrar y de tiempo de espera entre muestras
const int ctda = 5;

// valores de limites de posicion del joystick
int vxi = 0, vxs = 0, vyi = 0, vys = 0;

// indica el valor de posicion del array para activar los leds de dor
const int led_up = 2, led_dw = 3, led_rh = 4, led_lf = 5, off_all_leds = 0;

// Valores predeterminados para indicacion
const int carro = 1, camara = 2, para_todo = 5;

// Valores para identificacion de Vx y Vy del joystick
const int captura_calibracion = 1, captura_filtrado = 2;

// Selecionar Servo motor
const int servo_base = 1, servo_camara = 2;

/* lista de pinos */
int pinos[] = {led, rele_ctrl, up, dw, rh, lf, inx, iny, sw_jt};

// Valores iniciales de los angulos de los servos
int val_sb = 90; // servo base esq<->dir
int val_src = 80; // servo rotacion de la camara ari<->aba
int paso_cam = 3; // paso de angulos rotacion
int val_cam = 5;  // tempo de delay (ms) entre pasos
const int lim_ADC = 1023;
const int max_PWM = 252;

// controle envio de parada
bool enviar_parar = true;

// valores para poder enviar a cada cierto tiempo la parada
unsigned long time_parar = 0;
unsigned long time_espera = 5e5;

// creo un objeto para comunicacion
SoftwareSerial serial_slave = SoftwareSerial(5, 4);

void setup() {
  // Comunicacion master computador
  Serial.begin(115200);
  Serial.setTimeout(10);

  // comunicacion con el slave
  serial_slave.begin(115200);
  serial_slave.setTimeout(10);

  // inicializo pinos de entrada y salida
  for (int i = 0; i < 9; i++) {
    if (i < 6) {
      pinMode(pinos[i], OUTPUT);
      digitalWrite(pinos[i], 0);
    } else {
      pinMode(pinos[i], INPUT_PULLUP);
    }
  }

  // Blinco leds de direcion
  blink_leds(10, 50);

  // Enciendo led 13 indicador de funcion de selecion
  switch_joy();

  // Calibro posicion del joystick - valores iniciales inf y sup para X y Y
  cal_pos(captura_calibracion, ctda);

  time_parar = time_espera + micros();
}


/***************************************************************************/
/********************************* LOOP ************************************/
void loop() {

  if (valx < vxs && valx > vxi && valy < vys && valy > vyi) {
    /*Serial.print("Val X: ");
    Serial.print(valx);
    Serial.print("\t Val Y: ");
    Serial.println(valy);*/

    // mandar mensaje de que está parado
    // apagar leds
    if (enviar_parar  == 1){
      msg_send(para_todo, 0, 0);
      enviar_parar  = 0;
    }

    if (micros() > time_parar){
      msg_send(para_todo, 0, 0);
      time_parar = time_espera + micros();
    }
  } else {
    // separa carro o camara
    if (car_cam == carro) {
      // activar led y mensaje
      dir_led(valx, valy);
      enviar_parar = 1;
    } else {
      // valores para la camara
      dir_led(valx, valy);
      enviar_parar = 1;
    }
  }
  // verifica si se apreto el boton del joystick
  switch_joy();

  // Actualiza los valores de los potenciometros ejes
  cal_pos(2, ctda);
}

///////////////////////////////////////////////////////////////////////////////
// funcion separacion de aciones del carro y camara dependiendo del joystick
void dir_led(int valx, int valy) {
  if (valy > vys) {
    //enciende led de UP para motor o camera
    nf_led(led_up); // 2 up

    if (car_cam == carro) {
      Serial.println("Motor up");
      // concatena mensaje para enviar al slave mover motor
      int pwm_car = map(valy, vys, lim_ADC, 0, max_PWM);
      msg_send(1, led_up, pwm_car);
    } else {
      Serial.println("Camara frente");
      if (val_sb > 0) { // limite de movimiento de la camara
        val_sb -= paso_cam; // resta pasos para moviemiento de 0 y 180
      } else {
        val_sb = 0;
      }
      // concatena mensaje para enviar al slave mover camara
      msg_send(camara, servo_base, val_sb);
    }
  } else if (valy < vyi) {
    //enciende led de DOWN para motor o camera
    nf_led(led_dw); // down

    if (car_cam == carro) {
      Serial.println("Motor down");
      // concatena mensaje para enviar al slave mover motor
      int pwm_car = map(valy, vyi, 0, 0, max_PWM);
      msg_send(carro, led_dw, pwm_car); // 3 Down
    } else {
      Serial.println("Camara atras");
      if (val_sb < 179) { // limite de movimiento de la camara
        val_sb += paso_cam; // suma pasos para moviemiento de 0 y 180
      } else {
        val_sb = 179;
      }
      // concatena mensaje para enviar al slave mover camara
      msg_send(camara, servo_base, val_sb);
    }
  } else if (valx < vxi) {
    //enciende led de RIGHT para motor o camera
    nf_led(led_rh); // right

    if (car_cam == carro) {
      Serial.println("Motor rigth");
      // concatena mensaje para enviar al slave mover motor
      int pwm_car = map(valx, vxi, 0, 0, max_PWM / 2);
      msg_send(carro, led_rh, pwm_car);
    } else {
      Serial.println("Camara esq");
      if (val_src > 0) { // limite de movimiento de la camara
        val_src -= paso_cam; // if de 0 y 180
      } else {
        val_src = 0;
      }
      // concatena mensaje para enviar al slave mover camara
      msg_send(camara, servo_camara, val_src);
    }
  } else if (valx > vxs) {
    //enciende led de LEFT para motor o camera
    nf_led(led_lf); // left

    if (car_cam == carro) {
      Serial.println("Motor left");
      // concatena mensaje para enviar al slave mover motor
      int pwm_car = map(valx, vxs, lim_ADC, 0, max_PWM / 2);
      msg_send(carro, led_lf, pwm_car);
    } else {
      Serial.println("Camara abaixo");
      if (val_src < 179) { // limite de movimiento de la camara
        val_src += paso_cam; // suma pasos para moviemiento de 0 y 180
      } else {
        val_src = 179;
      }
      // concatena mensaje para enviar al slave mover camara
      msg_send(camara, servo_camara, val_src);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
//concatena mensaje
void msg_send(int sel_mensaje, int dir, int pwm_ang) {
  String msg = "";
  // Seleciona el mensaje a ser enviado
  switch (sel_mensaje) {
    case carro://Mover carro = 1
      // Concatena mensaje
      msg = "1<" + String(dir) + cs + String(pwm_ang) + "<0<"; // ultimo valor en 0
      Serial.print("serial_slave: ");
      Serial.println(msg);
      // Envia mensaje para el slave/carro
      serial_slave.println(msg);
      break;
    case camara:// Mover camara = 2
      // dir = sel motor
      msg = "2<" + String(dir) + cs + String(pwm_ang) + cs + String(val_cam) + cs;
      Serial.print("serial_slave: ");
      Serial.println(msg);
      serial_slave.println(msg);
      break;
    case 3:
      // alguma caracteristica que hará el slave
      break;
    case 4:
      // alguma caracteristica que hará el slave
      break;
    case para_todo: // = 5
      // PAra todo quando el joystick está em posicion central
      msg = "6<0<0<0<";

      for (int i = 0; i < 3; ++i) {
        Serial.print("serial_slave: ");
        Serial.println(msg);
        serial_slave.println(msg);
      }
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Controla acionamiento de leds de la direcion y los apaga
void nf_led(int pino) {
  if (pino > 1) {
    for (int i = 2; i < 6; i++) { // leds de direcion
      digitalWrite(pinos[i], 0);
    }
    digitalWrite(pinos[pino], 1); // enciende el pino de direcion escogido
  } else {
    for (int i = 2; i < 6; i++) {
      digitalWrite(pinos[i], 0);
    }
  }
}


////////////////////////////////////////////////////////////////////////
// funcion de calibracion y lectura de la pacicion del joystick
void cal_pos(int leitura, int t_amos) {
  valx = 0;
  valy = 0;

  switch (leitura) {
    case 1:
      // captura amostras y guarda valores temporales
      for (int i = 0; i < t_amos; i++) {
        valx = valx + analogRead(inx);
        valy = valy + analogRead(iny);
        delay(t_amos);
      }
      valx = valx / t_amos;
      valy = valy / t_amos;
      // Valores de limites para la calibracion
      vxi = valx - vlc;
      vxs = valx + vlc;
      vyi = valy - vlc;
      vys = valy + vlc;

      // valores de compar modo debug
      /*Serial.print("Val X: ");
        Serial.print(valx);
        Serial.print("\t Val Y: ");
        Serial.println(valy);

        Serial.print("\nVal X Sup: ");
        Serial.print(vxs);
        Serial.print("\t Val Y Inf: ");
        Serial.println(vxi);

        Serial.print("\nVal Y Sup: ");
        Serial.print(vys);
        Serial.print("\t Val Y Inf: ");
        Serial.println(vyi);
        Serial.println();
        delay(1000);*/

      break;
    case 2:
      // captura amostras y retorna en valores de la media
      for (int i = 0; i < t_amos; i++) {
        valx = valx + analogRead(inx);
        valy = valy + analogRead(iny);
        delay(t_amos);
      }
      valx = valx / t_amos;
      valy = valy / t_amos;
      break;
    case 3:
      //Valor sin filtro
      valx = analogRead(inx);
      valy = analogRead(iny);
      break;
  }
}

/////////////////////////////////////////////////////////////////////
// blinca y liga leds
void blink_leds(int qtd_blink, int time_blk) {
  for (int j = 0; j < qtd_blink; j++) {
    for (int i = 2; i < 6; i++) {
      digitalWrite(pinos[i], 1);
    }
    delay(time_blk * 2);
    for (int i = 2; i < 6; i++) {
      digitalWrite(pinos[i], 0);
    }
    delay(time_blk);
  }
}

/////////////////////////////////////////////////////////////////////
// Seleciona que tipo de accion tomar Carro o Camara
void switch_joy() {
  // interactua con los motres y la camara
  if (digitalRead(sw_jt) == 0)  {
    car_cam = !car_cam;
    delay(150);
  }
  if (car_cam == 1) {
    digitalWrite(pinos[0], 1);
  } else {
    digitalWrite(pinos[0], 0);
  }
}
