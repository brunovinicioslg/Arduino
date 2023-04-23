//BIBLIOTECAS UTILIZADAS
#include <TimerOne.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//DEFININDO PINOS E VARIAVEIS
const int buzzer = 9;
const int k1 = 8;
const int k2 = 10;
const int k3 = 13;
const int ledinterlock = 1;
const int ledligado = 6;
const int leddesligado = 7;
const int ledfalha = 0;
#define liga A3
#define desliga A4
#define interlock A2
#define sensormotor A0
#define sensormancal A1
int estadoliga = 0;
int estagiopartida = 0;
int estadodesliga = 0;
unsigned int segundos = 0;
int tempoantigo = 0;
int temponovo = 0;
float temperaturamotor = 0;
float temperaturamancal = 0;

void setup() {
  //INICIALIZA O CONTADOR INTERNO, LCD, E DEFINE OS TIPOS DOS PINOS
  Timer1.initialize(1000000);
  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(k1, OUTPUT);
  pinMode(k2, OUTPUT);
  pinMode(k3, OUTPUT);
  pinMode(ledinterlock, OUTPUT);
  pinMode(ledligado, OUTPUT);
  pinMode(leddesligado, OUTPUT);
  pinMode(ledfalha, OUTPUT);
  digitalWrite(ledligado, LOW);
  digitalWrite(leddesligado, HIGH);
  //02 BIPES
  tone(buzzer, 1500);
  delay(300);
  noTone(buzzer);
  delay(300);
  tone(buzzer, 1500);
  delay(300);
  noTone(buzzer);
  //INICIA AS MENSAGENS DE BOAS VINDAS
  //iniciolcd();
}

void loop() {
  //PARTE QUE CONTA O TEMPO E MEDE A TEMPERATURA
  Timer1.attachInterrupt(contadorTempo);
  temperaturamotor = (analogRead(sensormotor) * 0.0048828125 * 100);
  temperaturamancal = (analogRead(sensormancal) * 0.0048828125 * 100);

//SE TIVER TUDO OK, PEDE PRA APERTAR O BOTAO
  if (analogRead(liga) == 0 && estadoliga == 0 && analogRead(interlock) == 0 && temperaturamotor < 50 && temperaturamancal < 50) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("PRESSIONE O");
    lcd.setCursor(0, 1);
    lcd.print("BOTAO P/ INICIAR");
    delay(300);
  }

  //SE TIVER TUDO OK FALA QUE VAI INICIAR A PARTIDA
  if (analogRead(liga) > 0 && estadoliga == 0 && analogRead(interlock) == 0 && temperaturamotor < 50 && temperaturamancal < 50) {
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
    digitalWrite(k1, HIGH);
    digitalWrite(k3, HIGH);
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
      digitalWrite(k3, LOW);
      delay(500);
      digitalWrite(k2, HIGH);
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
  if (estadoliga == 1 && estagiopartida == 1 && analogRead(interlock) ==0 && analogRead(desliga) ==0 ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP MOTOR:");
    lcd.setCursor(0, 1);
    lcd.print(temperaturamotor);
    lcd.setCursor(4, 1);
    lcd.write(B11011111);
    lcd.setCursor(5, 1);
    lcd.print("C");
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP MANCAL:");
    lcd.setCursor(0, 1);
    lcd.print(temperaturamancal);
    lcd.setCursor(4, 1);
    lcd.write(B11011111);
    lcd.setCursor(5, 1);
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
    digitalWrite(k1, LOW);
    digitalWrite(k2, LOW);
    digitalWrite(k3, LOW);
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
    digitalWrite(k1, LOW);
    digitalWrite(k2, LOW);
    digitalWrite(k3, LOW);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TEMPERATURA");
    lcd.setCursor(6, 1);
    lcd.print("ALTA");
    delay(1000);
    estadodesliga = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP MOTOR:");
    lcd.setCursor(0, 1);
    lcd.print(temperaturamotor);
    lcd.setCursor(4, 1);
    lcd.write(B11011111);
    lcd.setCursor(5, 1);
    lcd.print("C");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP MANCAL:");
    lcd.setCursor(0, 1);
    lcd.print(temperaturamancal);
    lcd.setCursor(4, 1);
    lcd.write(B11011111);
    lcd.setCursor(5, 1);
    lcd.print("C");
    delay(1000);
  }

  if (temperaturamotor < 50 || temperaturamancal < 50) {
    digitalWrite(ledfalha, LOW);
  }

  if (analogRead(interlock) == 0) {
    digitalWrite(ledinterlock, LOW);
  }

 //SE APERTAR O BOTÃO DE DESLIGA, DESLIGA O SISTEMA
  if (analogRead(desliga) > 0) {
    estadoliga = 0;
    estagiopartida = 0;
    digitalWrite(ledligado, LOW);
    digitalWrite(leddesligado, HIGH);
    digitalWrite(k1, LOW);
    digitalWrite(k2, LOW);
    digitalWrite(k3, LOW);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("SISTEMA");
    lcd.setCursor(3, 1);
    lcd.print("DESLIGADO");
    delay(3000);
    estadodesliga = 1;
  }
}

// VOID PRA CONTAR OS SEGUNDOS
void contadorTempo() {
  segundos++;
}

//VOID PRA APARECER AS BOAS VINDAS
void iniciolcd() {


  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.print("UNIUBE");
  lcd.setCursor(0, 1);
  lcd.print("PROJETO INTEGRADO EM ENGENHARIA ELETRICA 1");
  delay(200);

  for (int posicao = 0; posicao < 46; posicao++) {
    lcd.scrollDisplayLeft();
    delay(200);
  }

  for (int posicao = 0; posicao < 46; posicao++) {
    lcd.scrollDisplayRight();
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BRUNO VINICIUS");
  lcd.setCursor(0, 1);
  lcd.print("RA 1163337");
  delay(3000);

  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.print("PARTIDA");
  lcd.setCursor(4, 1);
  lcd.print("ESTRELA TRIANGULO");
  delay(200);

  for (int posicao = 0; posicao < 17; posicao++) {
    lcd.scrollDisplayLeft();
    delay(200);
  }

  for (int posicao = 0; posicao < 17; posicao++) {
    lcd.scrollDisplayRight();
    delay(200);
  }

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("SEJA");
  lcd.setCursor(4, 1);
  lcd.print("BEM VINDO");
  delay(3000);
  tone(buzzer, 1500);
  delay(300);
  noTone(buzzer);
}