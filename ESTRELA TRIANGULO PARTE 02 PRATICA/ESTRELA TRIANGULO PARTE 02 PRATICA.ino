#include <LiquidCrystal.h>  //Inclui a biblioteca
#include "dht.h"            //INCLUSÃO DE BIBLIOTECA
#include <TimerOne.h>
#define sensor01 A7
#define interlock A12
const int pinoDHT11 = A6;  //PINO ANALÓGICO UTILIZADO PELO DHT11
const int buzzer = 11;
const int ledligado = 34, leddesligado = 32, ledfalha = 36, ledinterlock = 38;
const int k1 = 22, k2 = 30, k3 = 28;
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;  //Pinos para ligar o display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                 //Define os pinos que serão usados para ligar o display
dht DHT;                                                   //VARIÁVEL DO TIPO DHT
int estadoliga = 0;
int estagiopartida = 0;
int estadodesliga = 0;
unsigned int segundos = 0;
int tempoantigo = 0;
int temponovo = 0;
int temperaturamotor = 0;
int temperaturamancal = 0;
int tempmotor = 0;
float x=0;

void setup() {
  Serial.begin(9600);
 //INICIALIZA O CONTADOR INTERNO, LCD, E DEFINE OS TIPOS DOS PINOS
  Timer1.initialize(1000000);
  lcd.begin(16, 2);
  pinMode(k1, OUTPUT);
  pinMode(k2, OUTPUT);
  pinMode(k3, OUTPUT);
  pinMode(ledinterlock, OUTPUT);
  pinMode(ledligado, OUTPUT);
  pinMode(leddesligado, OUTPUT);
  pinMode(ledfalha, OUTPUT);
  digitalWrite(ledligado, LOW);
  digitalWrite(leddesligado,HIGH);
  digitalWrite(k1, HIGH);
  digitalWrite(k2, HIGH);
  digitalWrite(k3, HIGH);
  //02 BIPES
  tone(11, 880, 200);
  delay(800);
  tone(11, 880, 200);
  iniciolcd();
}


void loop() {
  Timer1.attachInterrupt(contadorTempo);
  tempmotor = analogRead(sensor01);
  if(tempmotor>880)
  {
    x=map(tempmotor,883,1000,27,100);
  }
  Serial.println(tempmotor);
  Serial.println(x);
  temperaturamotor = x;
  DHT.read11(pinoDHT11);
  temperaturamancal = DHT.temperature;

  //SE TIVER TUDO OK, PEDE PRA APERTAR O BOTAO
  if (estadoliga == 0 && analogRead(interlock) == 0 && temperaturamotor < 50 && temperaturamancal < 50) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("PRESSIONE O");
    lcd.setCursor(0, 1);
    lcd.print("BOTAO P/ INICIAR");
    delay(300);
  }

  //SE TIVER TUDO OK FALA QUE VAI INICIAR A PARTIDA
  if ((analogRead(0)) < 200 && estadoliga == 0 && analogRead(interlock) == 0 && temperaturamotor < 50 && temperaturamancal < 50) {
    estadoliga = 1;
    digitalWrite(ledligado, HIGH);
    digitalWrite(leddesligado, LOW);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("INICIANDO");
    lcd.setCursor(5, 1);
    lcd.print("PARTIDA");
    delay(1000);
  }

 //SE TIVER TUDO OK, INICIA A PARTIDA
  if (estadoliga == 1 && estagiopartida == 0 && analogRead(interlock) == 0 && temperaturamotor < 50 && temperaturamancal < 50)

  {
    //inciando partida estrela
    digitalWrite(k1, LOW);
    digitalWrite(k3, LOW);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("PARTIDA");
    lcd.setCursor(4, 1);
    lcd.print("ESTRELA");
    delay(300);
    //SOMA RA 21
    //UTILIZA O TIMER INTERNO PRA CALCULAR 21 SEGUNDOS
    if (tempoantigo == 0) {
      tempoantigo = segundos;
      temponovo = tempoantigo + 21;
    }
    //PASSOU 21 SEGUNDOS PASSA PRA LIGAÇÃO TRIANGULO
    if (segundos >= temponovo) {
      tempoantigo = 0;
      estagiopartida = 1;
      digitalWrite(k3, HIGH);
      delay(500);
      digitalWrite(k2, LOW);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("PARTIDA");
      lcd.setCursor(4, 1);
      lcd.print("TRIANGULO");
      delay(3000);

      lcd.clear();
    }
  }

//DEPOIS QUE A PARTIDA FOI FINALIZADA, EXIBE AS TEMPERATURAS
  if (estadoliga == 1 && estagiopartida == 1 && analogRead(interlock) ==0 && (analogRead(0)) < 400 ==0 ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP MOTOR: ");
    lcd.setCursor(12, 0);
    lcd.print(temperaturamotor);
    lcd.setCursor(15, 0);
    lcd.write(B11011111);
    lcd.setCursor(16, 1);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("TEMP MANCAL: ");
    lcd.setCursor(12, 1);
    lcd.print(temperaturamancal);
    lcd.setCursor(15, 1);
    lcd.write(B11011111);
    lcd.setCursor(16, 1);
    lcd.print("C");
    delay(500);
  }

  //SE CASO O INTERLOCK FOR ACIONADO, PARA TODO SISTEMA

  if (analogRead(interlock) > 0) {
    digitalWrite(ledinterlock, HIGH);
    digitalWrite(ledligado, LOW);
    digitalWrite(leddesligado, HIGH);
    estadoliga = 0;
    estagiopartida = 0;
    digitalWrite(k1, HIGH);
    digitalWrite(k2, HIGH);
    digitalWrite(k3, HIGH);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TRAVA ACIONADA");
    lcd.setCursor(6, 1);
    lcd.print("DESLIGADO");
    delay(3000);

    estadodesliga = 1;
  }

  // SE CASO ALGUMAS DAS TEMPERATURAS FOREM IGUAIS A 50 OU SUPERIOR, DESLIGA TODO SISTEMA
  if (temperaturamotor >= 50 || temperaturamancal >= 50) {
    digitalWrite(ledfalha, HIGH);
    digitalWrite(ledligado, LOW);
    digitalWrite(leddesligado, HIGH);
    estadoliga = 0;
    estagiopartida = 0;
    digitalWrite(k1, HIGH);
    digitalWrite(k2, HIGH);
    digitalWrite(k3, HIGH);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TEMPERATURA");
    lcd.setCursor(6, 1);
    lcd.print("ALTA");
    delay(1000);
    estadodesliga = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP MOTOR: ");
    lcd.setCursor(12, 0);
    lcd.print(temperaturamotor);
    lcd.setCursor(15, 0);
    lcd.write(B11011111);
    lcd.setCursor(16, 1);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("TEMP MANCAL: ");
    lcd.setCursor(12, 1);
    lcd.print(temperaturamancal);
    lcd.setCursor(15, 1);
    lcd.write(B11011111);
    lcd.setCursor(16, 1);
    lcd.print("C");
    delay(500);
  }

  if (temperaturamotor < 50 || temperaturamancal < 50) {
    digitalWrite(ledfalha, LOW);
  }

  if (analogRead(interlock) == 0) {
    digitalWrite(ledinterlock, LOW);
  }

 //SE APERTAR O BOTÃO DE DESLIGA, DESLIGA O SISTEMA
  if ((analogRead(0)) < 400 > 0) {
    estadoliga = 0;
    estagiopartida = 0;
    digitalWrite(ledligado, LOW);
    digitalWrite(leddesligado, HIGH);
    digitalWrite(k1, HIGH);
    digitalWrite(k2, HIGH);
    digitalWrite(k3, HIGH);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("SISTEMA");
    lcd.setCursor(3, 1);
    lcd.print("DESLIGADO");
    delay(3000);
    estadodesliga = 1;
  }
}

//Função para botão up

// VOID PRA CONTAR OS SEGUNDOS
void contadorTempo() {
  segundos++;
}

void iniciolcd() {


  lcd.clear();
  lcd.setCursor(14, 0);
  lcd.print("UNIUBE");
  lcd.setCursor(0, 1);
  lcd.print("PROJETO INTEGRADO EM ENG. ELETRICA 1 ");
  delay(400);

  for (int posicao = 0; posicao < 38; posicao++) {
    lcd.scrollDisplayLeft();
    delay(800);
  }

  // for (int posicao = 0; posicao < 48; posicao++) {
  //   lcd.scrollDisplayRight();
  //   delay(800);
  // }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BRUNO VINICIUS");
  lcd.setCursor(0, 1);
  lcd.print("RA 1163337");
  delay(4000);

  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.print("PARTIDA");
  lcd.setCursor(4, 1);
  lcd.print("ESTRELA TRIANGULO");
  delay(400);

  for (int posicao = 0; posicao < 17; posicao++) {
    lcd.scrollDisplayLeft();
    delay(600);
  }

  // for (int posicao = 0; posicao < 17; posicao++) {
  //   lcd.scrollDisplayRight();
  //   delay(600);
  // }

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("SEJA");
  lcd.setCursor(4, 1);
  lcd.print("BEM VINDO");
}