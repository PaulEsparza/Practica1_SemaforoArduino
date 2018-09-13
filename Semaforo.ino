/*
 * Definimos las variables y los pin utilizados en la placa arduino
 * s - Refiendose al semaforo de autos y el respectivo color de led
 * p - Refiriendose al semaforo de peatones y el respectivo color del led
 * El potencimetro que establecera el tiempo de espera antes del inicio de cambio de estados en el semaforo de coches
 * El buzzer que hara su funcion alertando en el semaforo de peatones cuando el estado en verde este a punto de cambiar a rojo
 */

#define sRojo 12
#define sAmarillo 11
#define sVerde 10
#define pRojo 9
#define pVerde 8
#define boton 7
#define buzzer 6
#define potenciometro A0
boolean estadoBoton = false;  // Variable que guarda el estado del pushBotton cuando es o no oprimido
int valorPot = 0; // Variable que almacena el valor en el que se encuentra el potenciometro
int tiempoEsperaPot = 10; // Variable que se multiplica con el valor del potenciometro para asignar su tiempo de espera antes del cambio de estado del semaforo
int crossTime = 0; // Variable que se utiliza como tiempo de espera antes de los cambios de estado del semaforo

/*
 * Definimos los tipos (entrada - salida) de cada componente utilizado
 */

void setup() {
  Serial.begin(9600);
  pinMode(sRojo, OUTPUT);
  pinMode(sAmarillo, OUTPUT);
  pinMode(sVerde, OUTPUT);
  pinMode(pRojo, OUTPUT);
  pinMode(pVerde, OUTPUT);
  pinMode(boton, INPUT);
}

void loop() {
  Serial.println(valorPot); // Imprime el valor actual en el que se encuentra el potenciometro
  valorPot = analogRead(potenciometro); // Guardamos el valor del potenciometro en una variable
  digitalWrite (pRojo, HIGH); // Encendemos led rojo de peatones
  digitalWrite (sVerde, HIGH); // Encendemos led verde del semaforo

  /*
   * Evaluamos el estado del pushBotton, si ha sido oprimido o aun no 
   */
   
  estadoBoton = digitalRead(boton);
  if (estadoBoton == true) {
    esperaPotenciometro();
    secuenciaSemaforo();
  }
}

/*
 * Establecemos la secuencia de espera y los cambios estados que seguira el semaforo de autos 
 */

void secuenciaSemaforo() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(sVerde, LOW);
    delay(1000);
    digitalWrite(sVerde, HIGH);
    delay(1000);
    digitalWrite(sVerde, LOW);
  }
  digitalWrite(sAmarillo, HIGH);
  delay(2500);
  digitalWrite(sAmarillo, LOW);
  digitalWrite(sRojo, HIGH);
  delay(2000);
  digitalWrite(pRojo, LOW);
  digitalWrite(pVerde, HIGH);
  delay(crossTime); // Tiempo de espera antes de que se inicie la secuencia de estados en los semaforos
  secuenciaPeaton(); // Mandamos a llamar el metodo de la secuencia que utiliza el semaforo de peatones
  digitalWrite(pRojo, HIGH);
  delay(2000);
  digitalWrite(sRojo, LOW);
  digitalWrite(pVerde, LOW);
}

/*
 * Establecemos la secuencia de espera y los cambios estados que seguira el semaforo de peatones 
 */
 
void secuenciaPeaton() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pVerde, LOW);
    delay(1000);
    digitalWrite(pVerde, HIGH);
    analogWrite(buzzer, 190); // Mandamos la señal para hacer sonar el buzzer durante la secuencia del semaforo
    delay(1000);
    digitalWrite(pVerde, LOW);
    analogWrite(buzzer, 0); // Apagamos la señal para que suene acorde al ciclo del semaforo y su parpadeo
  }
}

void esperaPotenciometro() {
  // Multiplicamos el valor actual del potenciometro y lo asignamos en una variable para ser usada en el tiempo de espera del semaforo antes del cambio de estado
  valorPot = valorPot * tiempoEsperaPot; 
  Serial.println(valorPot); // Imprime el resultado de la multiplicacion para el tiempo de espera del semaforo
  digitalWrite(sVerde, HIGH); // Enciende led verde del semaforo de autos
  crossTime = map(valorPot, 0, 1023, 10, 25)*100; // Funcion map para trabajar con el rango de tiempo de espera
  Serial.println(crossTime); // Imprime el resultado para el tiempo de espera del semaforo
  delay(crossTime); // Tiempo de espera antes de que se inicie la secuencia de estados en los semaforos
}
