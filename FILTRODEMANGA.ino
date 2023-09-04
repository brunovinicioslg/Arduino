#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <EEPROM.h>
//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
//armazena as variaveis
int LED_FALHA = 1;
int LED_ATIVO = 2;
int VALVULA_1 = 3;
int VALVULA_2 = 4;
int VALVULA_3 = 5;
int VALVULA_4 = 6;
int VALVULA_5 = 7;
int VALVULA_6 = 8;
int VALVULA_7 = 9;
int VALVULA_8 = 10;
int pressao = 0;
int botao = 0;
int estado = 0;
const int buzzer = 11;
const int chave = 0;
int modo = 0;
int funcao = 0;
int falha = 0;
int trava = 0;
int verifica_modo = 0;
int estado_manual = 1;
unsigned int segundos = 0;
unsigned int valor_total= 0;
int endereco = 0;
int valor= 0;
int valvula = 0;
int valvula_ativa[] = { VALVULA_1, VALVULA_2, VALVULA_3, VALVULA_4, VALVULA_5, VALVULA_6, VALVULA_7, VALVULA_8 };

void setup() {
  //definições
  Timer1.initialize(1000000);
  pinMode(LED_FALHA, OUTPUT);
  pinMode(LED_ATIVO, OUTPUT);
  pinMode(VALVULA_1, OUTPUT);
  pinMode(VALVULA_2, OUTPUT);
  pinMode(VALVULA_3, OUTPUT);
  pinMode(VALVULA_4, OUTPUT);
  pinMode(VALVULA_5, OUTPUT);
  pinMode(VALVULA_6, OUTPUT);
  pinMode(VALVULA_7, OUTPUT);
  pinMode(VALVULA_8, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(chave, INPUT);

//exibição das mensagens
  lcd.init();
  lcd.setBacklight(HIGH);
  tone(buzzer, 1500);
  delay(2000);
  noTone(buzzer);
  lcd.setCursor(0, 0);
  lcd.print("UNIUBE");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PROJETO INT.");
  lcd.setCursor(0, 1);
  lcd.print("ENG. ELET. 2");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BRUNO VINICIUS");
  lcd.setCursor(0, 1);
  lcd.print("RA 1163337-1");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SEQUENCIADOR");
  lcd.setCursor(0, 1);
  lcd.print("PARA LIMPEZA");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DE FILTRO");
  lcd.setCursor(0, 1);
  lcd.print("DE MANGA");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SEJA BEM");
  lcd.setCursor(0, 1);
  lcd.print("VINDO!");
  delay(3000);
  lcd.clear();
  tone(buzzer, 1500);
  delay(300);
  noTone(buzzer);
  delay(300);
  tone(buzzer, 1500);
  delay(300);
  noTone(buzzer);

  //pega o valor armazenado na memoria e continua onde parou
  valor = valor_total;
}

void loop() {
  Timer1.attachInterrupt(contadorTempo);
//faz as leituras
  pressao = analogRead(A0);
  botao = analogRead(A1);


//verificacao se caso tem falha
  switch (falha) {
    case 0:
      digitalWrite(LED_ATIVO, HIGH);

      switch (modo) {
        case 1:

          ligaValvula();


          break;
        default:
          modoManual();


          break;
      }
      break;
    default:
      digitalWrite(LED_ATIVO, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FALHA");
      lcd.setCursor(0, 1);
      lcd.print("PRESSAO ALTA");
      break;
  }

// exibe as mensagens de modo
  if (estado_manual == 1 && funcao == 0 && trava == 0 && falha == 0) {

    if (modo == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AUTO");
      lcd.setCursor(6, 0);
      lcd.print("CICLOS:");
      lcd.setCursor(13, 0);
      lcd.print(valor);
      lcd.setCursor(0, 1);
      lcd.print("PRESSAO:");
      lcd.setCursor(9, 1);
      lcd.print(pressao);
      delay(300);
    }

    if (modo == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MANUAL");
      lcd.setCursor(0, 1);
      lcd.print("PRESSAO:");
      lcd.setCursor(9, 1);
      lcd.print(pressao);
      delay(300);
    }
  }



  //chama as funções
  pressaoFalha();
  botoesSelecao();
  selecionarModo();
  alvaMemoria();
}




// VOID PRA CONTAR OS SEGUNDOS
void contadorTempo() {
  segundos++;
}

void modoManual() {
  switch (funcao) {
    case 1:
      //reset
      digitalWrite(VALVULA_1, HIGH);
      digitalWrite(VALVULA_2, HIGH);
      digitalWrite(VALVULA_3, HIGH);
      digitalWrite(VALVULA_4, HIGH);
      digitalWrite(VALVULA_5, HIGH);
      digitalWrite(VALVULA_6, HIGH);
      digitalWrite(VALVULA_7, HIGH);
      digitalWrite(VALVULA_8, HIGH);
      break;
    case 2:
      //test
      valvula = valvula_ativa[estado_manual] - 2;
      while (segundos < 6) {
        digitalWrite(valvula, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("VALVULA");
        lcd.setCursor(9, 0);
        lcd.print(estado_manual - 1);
        lcd.setCursor(0, 1);
        lcd.print("ATIVA");
        delay(300);
      }
      estado_manual = 1;
      lcd.clear();

      break;
    case 3:
      //select
      if (trava == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("VALVULA");
        lcd.setCursor(9, 0);
        lcd.print(estado_manual);
        lcd.setCursor(0, 1);
        lcd.print("<SELECIONAR>");
        if (estado_manual <= 8) {
          ++estado_manual;
        }
        if (estado_manual > 8) {
          estado_manual = 1;
        }

        trava = 1;
      }
      if (segundos >= 1) {
        trava = 0;
        segundos = 0;
      }
      break;
    default:
      digitalWrite(VALVULA_1, LOW);
      digitalWrite(VALVULA_2, LOW);
      digitalWrite(VALVULA_3, LOW);
      digitalWrite(VALVULA_4, LOW);
      digitalWrite(VALVULA_5, LOW);
      digitalWrite(VALVULA_6, LOW);
      digitalWrite(VALVULA_7, LOW);
      digitalWrite(VALVULA_8, LOW);
      break;
  }
}

void selecionarModo() {
  if (digitalRead(chave) != LOW) {
    modo = 1;
    if (verifica_modo == 0) {
      estado = 0;
      verifica_modo = 1;
    }

  } else {
    if (verifica_modo == 1) {
      verifica_modo = 0;
    }
    modo = 0;
  }
}

void salvaMemoria(){


  EEPROM.write(0,valor/256); //primeira parte 
  EEPROM.write(1,valor%256); //////resto da divisão

  int parte1 = EEPROM.read(0);
  int parte2 = EEPROM.read(1);
 
  ///////soma os valores
  valor_total = (parte1 * 256) + parte2;
}
void botoesSelecao() {
  if (botao > 100 && botao < 199) {


    funcao = 1;
    delay(300);


  } else if (botao > 200 && botao < 300) {

    funcao = 2;
    delay(300);

  } else if (botao > 400 && botao < 500) {


    funcao = 3;
    delay(300);

  } else {


    funcao = 0;
    delay(300);
  }
}

void pressaoFalha() {
  if (pressao >= 800) {
    digitalWrite(LED_FALHA, HIGH);
    falha = 1;

  } else {
    digitalWrite(LED_FALHA, LOW);
    falha = 0;
  }
}
void ligaValvula() {
  if (estado == 0) {
    digitalWrite(VALVULA_1, HIGH);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 1;
    segundos = 0;
  }
  if (estado == 1 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 2;
    segundos = 0;
  }
  if (estado == 2 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, HIGH);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 3;
    segundos = 0;
  }
  if (estado == 3 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 4;
    segundos = 0;
  }

  if (estado == 4 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, HIGH);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 5;
    segundos = 0;
  }
  if (estado == 5 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 6;
    segundos = 0;
  }
  if (estado == 6 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, HIGH);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 7;
    segundos = 0;
  }
  if (estado == 7 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 8;
    segundos = 0;
  }
  if (estado == 8 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, HIGH);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 9;
    segundos = 0;
  }
  if (estado == 9 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 10;
    segundos = 0;
  }
  if (estado == 10 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, HIGH);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 11;
    segundos = 0;
  }
  if (estado == 11 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 12;
    segundos = 0;
  }
  if (estado == 12 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, HIGH);
    digitalWrite(VALVULA_8, LOW);
    estado = 13;
    segundos = 0;
  }
  if (estado == 13 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 14;
    segundos = 0;
  }
  if (estado == 14 && segundos >= 5) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, HIGH);
    estado = 15;
    segundos = 0;
  }
  if (estado == 15 && segundos >= 2) {
    digitalWrite(VALVULA_1, LOW);
    digitalWrite(VALVULA_2, LOW);
    digitalWrite(VALVULA_3, LOW);
    digitalWrite(VALVULA_4, LOW);
    digitalWrite(VALVULA_5, LOW);
    digitalWrite(VALVULA_6, LOW);
    digitalWrite(VALVULA_7, LOW);
    digitalWrite(VALVULA_8, LOW);
    estado = 16;
    segundos = 0;
  }
  if (estado == 16 && segundos >= 25) {
    estado = 0;
    segundos = 0;
    ++valor;
  }
}