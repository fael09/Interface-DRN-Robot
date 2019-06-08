// Cobnexiones del joystick
#define led 13
#define inx A5
#define iny A4
#define sw_jt 3

int vlc = 40;
int valx = 0, valy = 0, valxt = 0, valyt = 0; // variables de posicion
int ctda = 5; // cantidad de amostras para filtrar
// valores de limites de posicion del joystick
int vxi = 0, vxs = 0, vyi = 0, vys = 0;

bool car_cam = 1;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(inx, INPUT_PULLUP);
  pinMode(iny, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(inx, 0);
  digitalWrite(iny, 0);

  pinMode(sw_jt, INPUT_PULLUP);
}

void loop() {

  /*cal_pos(2, 10); // captura y actualiza valores de la pocicion del joystick valx y valy

    if (valx < vxs && valx > vxi && valy < vys && valy > vyi){
        //salir2 = 1;
    }*/

  cal_pos(2, 5);
  switch_joy();
  Serial.print("Valor SW: ");
  Serial.print(car_cam);
  Serial.print("\t Val X: ");
  Serial.print(valx);
  Serial.print("\t Val Y: ");
  Serial.println(valy);
  delay(100);


}

////////////////////////////////////////////////////////////////////////
// funcion de calibracion y lectura de la pacicion del joystick
void cal_pos(int leitura, int t_amos) {
  valx = 0;
  valy = 0;

  switch (leitura) {
    case 1:
      // captura amostras y guarda valores temporales
      for (int i = 0; i < ctda; i++) {
        valx = valx + analogRead(inx);
        valy = valy + analogRead(iny);
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
      for (int i = 0; i < ctda; i++) {
        valx = valx + analogRead(inx);
        valy = valy + analogRead(iny);
        delay(t_amos);
      }
      valx = valx / ctda;
      valy = valy / ctda;
      break;
    case 3:
      valx = analogRead(inx);
      valy = analogRead(iny);
      break;


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
    digitalWrite(led, 1);
  } else {
    digitalWrite(led, 0);
  }
}