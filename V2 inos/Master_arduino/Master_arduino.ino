// conexiones para arduino
// D4 >> DI (TX)
// D5 >> RO (RX)
// Re >> De >> VCC Transmisor
// Usar 5v no MAX485
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
#define inx A4
#define iny A5
// Control final mensaje
#define cs '<'
// carro o camara
bool car_cam = true;
// varibles de control del esclavo
// 0 - seleciona opcio > 1=Mover Carro, 2=Mover Camara, 3=Encender Slave, 4=Encender linterna, 5....
// 1 - tipo de movimiento del carro 2=up, 3=dw, 4=rh, 5=lf
// 2 - tipo de PWM que puede ser cotrolado, por causa del driver solo funciona a 255
// 3 - time=0 no hay control de tiempo, mayor que zero hay control de tiempo
int var_ctrl[] = { 0, 0, 0, 0 };

// Matrix de pinos
int pinos[] = { led, rele_ctrl, up, dw, rh, lf, inx, iny };

String data_in = "", data_send = "";
int ctda = 10; // cantidad de amostras para filtrar
int tam = 20; // tamanho de la histerisis para el control de salida de la pocision 0 del joystick
int valx = 0, valy = 0, valxt = 0, valyt = 0; // variables de posicion
int cam_val = 0; // control del switch para pasar de control de carro/camara
int lim_inf = 400, lim_sup = 700; // limites de los potenciometros

// valores de limites de posicion del joystick
int vxi = 0, vxs = 0, vyi = 0, vys =0;
int vlc = 40; // valor limite de calibração

// Valores iniciales de los angulos de los servos 
int val_sb = 90; // servo base esq<->dir
int val_src = 90; // servo rotacion de la camara ari<->aba
int paso_cam = 3; // paso de angulos rotacion
int val_cam = 5;  // tempo de delay entre paos

int env_zero = 1, salir0 = 1, salir1 = 1, salir2 = 1; // control de parada de los motores del carro y de los whiles de control del joystick

// valor que el joystick tiene poara controlar el robot
int pwm_serial = 250;

// creo un objeto para comunicacion
SoftwareSerial serial_slave = SoftwareSerial(5, 4);

// Setup
void setup(){
  // comunicacion con el slave
  serial_slave.begin(115200);
  serial_slave.setTimeout(10);
  // Comunicacion master computador
  Serial.begin(115200);
  Serial.setTimeout(10);

  // inicializo pinos de entrada y salida
  for (int i = 0; i < 8; i++) {
    if (i < 6){
      pinMode(pinos[i], OUTPUT);
      digitalWrite(pinos[i], 0);
    }else{
      pinMode(pinos[i], INPUT_PULLUP);
    }
  }

  // calibrar sensores de posicion joystick
  cal_pos(1, 10); // 1 = 1 calibracion - 2 captura //// 2 = tempo entre amostras 

  // Blinco leds de direcion
  blink_leds();

  // parar tudo
  msg_send(5, 0, 0); // manda un mensaje de para de todo

  // pin del siwtch del joystick como entrada
  pinMode(sw_jt, INPUT_PULLUP); // pino para selecionar camara o mover carro con el joystiock

  // blinca led 13 iniprogram
  nf_led(0, 2, 100);
}


/////////////////////////////////////////////////////////////////////////////
// Loop
void loop(){

  // verifica si se apreto el boton del joystick
  switch_joy();

  if (car_cam == 1){ // entra em el control de carro == 1, camera == 0
    // se quesa aqui si no hay movimiento

    while (salir0 == 1){ // posicion central del joystick
      // para salir de enviar datos
      digitalWrite(pinos[0], 1); // led carro = 1, cam = 0

      cal_pos(2, 10); // captura y actualiza valores de la pocicion del joystick valx y valy

      // 
      if (valx < vxs && valx > vxi && valy < vys && valy > vyi){
        //salir2 = 1;
      }else{
        salir0 = 0; // sale del while principal
      }

      if (env_zero == 1){ // ésto se hace una vez cada vez que entra en el while
        // Valores de calibracion
        /*Serial.print("Val vxi: ");
        Serial.print(vxi);
        Serial.print("\t Val vxs: ");
        Serial.print(vxs);
        Serial.print("\t Val vyi: ");
        Serial.print(vyi);
        Serial.print("\t Val vys: ");
        Serial.println(vys);*/

        Serial.println("\n Está en reposo \n");

        nf_led(1, 0, 0); // apaga los leds de direcion
        msg_send(5, 0, 0); // manda un mensaje de para de todo
        env_zero = 0; // sale de este if
        //print_valxy(valx, valy);// imprime los valores de 
      }

      switch_joy();

      if(env_zero == 1){
        salir0 = 0;
      }
      ler_serial(); // Para leer y controlar por el computador
    } // sale del while si hay movimiento del joystick


    //Serial.println("\n Saliendo del reposo \n");
    //print_valxy(valx, valy);// imprime los valores de 

    while (salir1 == 1){
      if (valx < (valxt - tam) || valx > (valxt + tam) || valy < (valyt - tam) || valy > (valyt + tam)){
        cal_pos(2, 1); // captura y actualiza valores de la pocicion del joystick valx y valy

        //print_valxy(valx, valy);// imprime los valores de 

        // pasa valores para separacxion de aciones y encender led de direcion
        // esta funcion hace funciona solo con el joystick
        dir_led(valx, valy, 1); 

        if (valx < vxs && valx > vxi && valy < vys && valy > vyi){
          valxt = valx;
          valyt = valy;
        }
        salir0 = 1; // while de para todo
      }else{
        salir1 = 0;
        valxt = valx;
        valyt = valy;
      }
    }

    msg_send(5, 0, 0); // manda un mensaje de para de todo

    switch_joy();

  }else{
    // Mover camara ///////////////////////////////////////////////////////////
    digitalWrite(pinos[0], 0); // led carro = 1, cam = 0
    cal_pos(2, 10); // captura y actualiza valores de la pocicion del joystick valx y valy
    msg_send(5, 0, 0); // manda un mensaje de para de todo
    while(salir2 == 1){
      if(valx < (valxt - tam) || valx > (valxt + tam) || valy < (valyt - tam) || valy > (valyt + tam)){

        cal_pos(2, 10); // captura y actualiza valores de la pocicion del joystick valx y valy

        //print_valxy(val_sb, val_src);// imprime los valores de 

        // pasa valores para separacxion de aciones y encender led de direcion
        // esta funcion hace funciona solo con el joystick
        dir_led(valx, valy, 2);

        if (valx < vxs && valx > vxi && valy < vys && valy > vyi){
          valxt = valx;
          valyt = valy;
        }
      }else{
        salir2 = 0;
        valxt = valx;
        valyt = valy;
      }
    }
    nf_led(1, 0, 0); // apaga los leds de direcion
  }// end control motor o cam

  // Reinicio variables
  env_zero = 1;
  salir0 = 1;
  salir1 = 1;
  salir2 = 1;

  valxt = valx;
  valyt = valy;

}// fin void loop


/**************************************************************************/
/*********************** Lectura de datos *******************************/
void ler_serial(){
  data_send = ""; // reinicio la variable principal
  if (Serial.available() != 0){
    for (int i = 0; i < 4; i++){
      data_in = Serial.readStringUntil(cs); // leo cada valor de entrada separado por el caracter >
      data_send = data_send + data_in + cs; // concateno nuevamente el Mensaje
      var_ctrl[i] = data_in.toInt(); // convirtiendo el string a int
    }

    Serial.readString(); // Limpa el buffer

    switch (var_ctrl[0]){ // Separando
      case 0:
        Serial.print("Mensaje a ser enviada por serial_slave: ");
        Serial.println(data_send);
        // para todo
        nf_led(1, 0, 0); // apaga los leds de direcion
        msg_send(5, 0, 0); // manda un mensaje de para de todo
      case 1://Mover carro
        Serial.print("Mensaje a ser enviada por serial_slave: ");
        Serial.println(data_send);
        serial_slave.println(data_send);
        separar_led(var_ctrl[1]);
        // funcion para mover el carro 
        break;
      case 2:// Mover camara
        Serial.print("Mensaje a ser enviada por serial_slave: ");
        Serial.println(data_send);
        serial_slave.println(data_send);
        separar_led(var_ctrl[1]);
        break;
      case 3:
        if (var_ctrl[1] == 1){
          digitalWrite(pinos[1], 1); // Activa el rele, prende el carro
        }else{
          digitalWrite(pinos[1], 0); // Desactiva el rele, apaga el carro
        }
        break;
      case 4:
        // Modo debug
        for (int i = 0; i < 4; i++) {
          Serial.print("Valores recibidos: ");
          Serial.println(var_ctrl[i]);
        }
        break;
    }
    
  }
}

///////////////////////////////////////////////////////////////////////////////
// funcion separacion de aciones del carro y camara dependiendo del joystick
void dir_led(int valx, int valy, int ccs){ // ccs seleciona motor o camera
  if (valx < lim_inf) {
    //enciende led de UP para motor o camera
    nf_led(2, 0, 0); // 2up

    if (ccs == 1){
      Serial.println("Motor up");
      // concatena mensaje para enviar al slave mover motor
      msg_send(1, 2, 0);
    }else{
      Serial.println("Camara frente");
      if (val_sb > 0){ // limite de movimiento de la camara
        val_sb -= paso_cam; // resta pasos para moviemiento de 0 y 180
      }else{
        val_sb = 0;
      }
      // concatena mensaje para enviar al slave mover camara
      msg_send(2, 2, val_sb);
    }
  }else if (valx > lim_sup){
      //enciende led de DOWN para motor o camera
      nf_led(3, 0, 0); // down

      if (ccs == 1){
        Serial.println("Motor down");
        // concatena mensaje para enviar al slave mover motor
        msg_send(1, 3, 0); // 3 Down
      }else{
        Serial.println("Camara atras");
        if (val_sb < 179){// limite de movimiento de la camara
          val_sb += paso_cam; // suma pasos para moviemiento de 0 y 180
        }else{
          val_sb = 179;
        }
        // concatena mensaje para enviar al slave mover camara
        msg_send(2, 2, val_sb);
      }
    }else if (valy < lim_inf){
      //enciende led de RIGHT para motor o camera
      nf_led(4, 0, 0); // right
      if (ccs == 1){
        Serial.println("Motor rigth");
        // concatena mensaje para enviar al slave mover motor
        msg_send(1, 4, 0);
      }else{
        Serial.println("Camara esq");
        if (val_src > 0){ // limite de movimiento de la camara
          val_src -= paso_cam; // if de 0 y 180
        }else{
          val_src = 0;
        }
        // concatena mensaje para enviar al slave mover camara
        msg_send(2, 1, val_src);
      }
    }else if (valy > lim_sup){
      //enciende led de LEFT para motor o camera
      nf_led(5, 0, 0); // left

      if (ccs == 1){
        Serial.println("Motor left");
        // concatena mensaje para enviar al slave mover motor
        msg_send(1, 5, 0);
      }else{
        Serial.println("Camara abaixo");
        if (val_src < 179){ // limite de movimiento de la camara
          val_src += paso_cam; // suma pasos para moviemiento de 0 y 180 
        }else{
          val_src = 179; 
        }
        // concatena mensaje para enviar al slave mover camara
        msg_send(2, 1, val_src);
      }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//concatena mensaje
void msg_send(int cc, int dir, int ang){
  String msg = "";
  // Seleciona el mensaje a ser enviado
  switch (cc) {
    case 1://Mover carro
      msg = "1<" + String(dir) + cs + String(pwm_serial) + "<0<";
      Serial.print("serial_slave: ");
      Serial.println(msg);
      serial_slave.println(msg);
      break;
    case 2:// Mover camara
      msg = "2<" + String(dir) + cs + String(ang) + cs + String(val_cam) + cs;
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
    case 5:
      // PAra todo quando el joystick está em posicion central
      msg = "6<0<0<0<";
      
      for (int i = 0; i < 3; ++i){
        Serial.print("serial_slave: ");
        Serial.println(msg);
        serial_slave.println(msg);
      }
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Blinka el led 13 y controla acionamiento de leds de la direcion
void nf_led(int pino, int count, int t_l){
  if (pino > 1){
    for (int i = 2; i < 6; i++) { // leds de direcion 
      digitalWrite(pinos[i], 0);
    }
    digitalWrite(pinos[pino], 1); // enciende el pino de direcion escogido
  }else{
    switch (pino){
      case 0:
        for (int i = 0; i < count; i++){
          digitalWrite(pinos[pino], 1);
          delay(t_l * 2);
          digitalWrite(pinos[pino], 0);
          delay(t_l);
        }
        break;
      case 1:// desliga todos os leds 
        for (int i = 2; i < 6; i++){
          digitalWrite(pinos[i], 0);
        }
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
// apaga todos los leds
void off_leds(){
  for (int i = 0; i < 6; i++)  {
    digitalWrite(pinos[i], 0);
  }
}

/////////////////////////////////////////////////////////////////////
// blinca y liga leds
void blink_leds(){
  for (int j = 0; j < 10; j++){
    for (int i = 2; i < 6; i++){
      digitalWrite(pinos[i], 1);
    }
    delay(100);
    for (int i = 2; i < 6; i++){
      digitalWrite(pinos[i], 0);
    }
    delay(50);
  }
}

///////////////////////////////////////////////////////////////////////////
// activacion de led segundo a dirçao
void separar_led(int dir){
  switch (dir)  {
    case 2:
      nf_led(2, 0, 0); // up
      break;
    case 3:
      nf_led(3, 0, 0); // dw
      break;
    case 4:
      nf_led(4, 0, 0); // dw
      break;
    case 5:
      nf_led(5, 0, 0); // dw
      break;
  }
}

////////////////////////////////////////////////////////////////////////
// funcion de calibracion y lectura de la pacicion del joystick
void cal_pos(int leitura, int t_amos){
  valx = 0;
  valy = 0;
  
  switch (leitura) {
      case 1:
      // captura amostras y guarda valores temporales
        for (int i = 0; i < ctda; i++){
          valx = valx + analogRead(pinos[6]);
          valy = valy + analogRead(pinos[7]);
          delay(t_amos);
        }
        valx = valx / ctda;
        valy = valy / ctda;
        // Valores de limites para la calibracion
        vxi = valx - vlc; 
        vxs = valx + vlc;
        vyi = valy - vlc; 
        vys = valy + vlc;
        break;
      case 2:
      // captura amostras y retorna en valores de la media
        for (int i = 0; i < ctda; i++){
          valx = valx + analogRead(pinos[6]);
          valy = valy + analogRead(pinos[7]);
          delay(t_amos);
        }
        valx = valx / ctda;
        valy = valy / ctda;
        break;
  }
}

void print_valxy(int vax, int vay){
  if(car_cam == 1){
    Serial.print("Valor de x para carro: ");
    Serial.print(vax);
    Serial.print("\t");
    Serial.print("Valor de y para carro: ");
    Serial.print(vay);
    Serial.print("\t");
    Serial.print("Val cam: ");
    Serial.println(car_cam);
  }else{
    Serial.print("Valor de servo base para camara: ");
    Serial.print(vax);
    Serial.print("\t");
    Serial.print("Valor de servo camara para camara: ");
    Serial.print(vay);
    Serial.print("\t");
    Serial.print("Val cam: ");
    Serial.println(car_cam);
  }

}

void switch_joy(){
  // interactua con los motres y la camara
  if (digitalRead(sw_jt) == 0)  {
    car_cam = !car_cam;
    env_zero = 1;
    delay(150);
  }
}