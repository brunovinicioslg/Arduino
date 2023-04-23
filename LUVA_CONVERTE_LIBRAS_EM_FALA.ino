//Carrega a biblioteca Wire
#include<Wire.h>
#define pin1 6
#define pin2 7 //DEFINIÇÃO DOS PINOS MULTIPLEXAÇÃO
#define pin3 8
#define pin4 9
#define pin5 10
#define bt A2 //DIFNE PINO ENTRADA BOTÃO GATILHO
//Endereco I2C do MPU6050
const int MPU = 0x69; //DEFINE O ENDEREÇO PRA LEITURA COMO AD0 NIVEL BAIXO
//Variaveis para armazenar valores dos sensores
int AcX, AcY, AcZ, GyX, GyY, GyZ;
int vetor1[6] = {0, 0, 0, 0, 0, 0};
int vetor2[6] = {0, 0, 0, 0, 0, 0};
int vetor3[6] = {0, 0, 0, 0, 0, 0};
int vetor4[6] = {0, 0, 0, 0, 0, 0};
int vetor5[6] = {0, 0, 0, 0, 0, 0};
int vetor6[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // DEFINE OS VETORES USADOS PRA ARMAZENAR OS DADOS
int vetor7[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetor8[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetor9[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetor10[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetor11[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetor12[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetor13[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int vetorbase[6] = {5000,5000,5000,0,0,0};
int vetorbase2[6] = {-5000,-5000,-5000,0,0,0};
int aceleracao, botao, estado = 0, contador = 0;
int var1 = 0, var2 = 0, var3 = 0, var4 = 0, var5 = 0;
String palavra = "Hello World";


void setup()
{
  Serial.begin(9600);// INICIA O SERIAL
  Wire.begin();
  Wire.beginTransmission(MPU); // INICIA OS SENSORES
  Wire.write(0x6B);
  //Inicializa o MPU-6050
  Wire.write(0);
  Wire.endTransmission(true);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT); // DEFINE OS TIPOS DE PINOS, ENTRADAS OU SAIDAS
  pinMode(pin5, OUTPUT);
  pinMode(bt, INPUT);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
}
void loop()
{
  botao = analogRead(bt);
  //Serial.println(botao);
  multiplex(); // CHAMA A MULTIPLEXAÇÃO LÁ DE BAIXO
  if (botao >= 300 && estado == 0)
  {
    estado = 1;
    contador = 1; // VERIFICA SE O GATILHO FOI ACIONADO
  }
  if (estado == 1 && contador == 1)
  {
    Serial.println("Salvando movimento");
    delay(200);
    multiplex(); //SE FOI ACIONADO FAZ O ARMAZENAMENTO DOS DADOS DENTRO DOS VETORES
    mem1();
    estado = 1;
    contador=2;
  }
  if (estado == 1 && contador == 2)
  {
    multiplex();
    mem2();
    estado = 1;
    contador = 3;
  }
  if (estado == 1 && contador == 3)
  {
    multiplex();
    mem3();
    estado = 1;
    contador = 4;
  }
  if (estado == 1 && contador == 4)
  {
    multiplex();
    mem4();
    estado = 1;
    contador = 5;
  }  if (estado == 1 && contador == 5)
  {
    multiplex();
    mem5();
    estado = 1;
    contador = 6;
  }  if (estado == 1 && contador == 6)
  {
    multiplex();
    mem6();
    estado = 1;
    contador = 7;
  }  if (estado == 1 && contador == 7)
  {
    multiplex();
    mem7();
    estado = 1;
    contador = 8;
  }
  if (estado == 1 && contador == 8)
  {
    multiplex();
    mem8();
    Serial.println("Movimento Salvo");
    estado = 0;
    contador = 0;
  }
  // COMPARA OS VALORES SALVOS COM OS VALORES EM EXECUÇÃO
  if (vetor1[3] == vetor6[0] || vetor1[3] == vetor7[0] || vetor1[3] == vetor8[0] || vetor1[3] == vetor9[0] || vetor1[3] == vetor10[0] || vetor1[3] == vetor11[0] || vetor1[3] == vetor12[0] || vetor1[3] == vetor13[0])
  {
    if (vetor1[4] == vetor6[1] || vetor1[4] == vetor7[1] || vetor1[4] == vetor8[1] || vetor1[4] == vetor9[1] || vetor1[4] == vetor10[1] || vetor1[4] == vetor11[1] || vetor1[4] == vetor12[1] || vetor1[4] == vetor13[1])
    {
      if (vetor1[5] == vetor6[2] || vetor1[5] == vetor7[2] || vetor1[5] == vetor8[2] || vetor1[5] == vetor9[2] || vetor1[5] == vetor10[2] || vetor1[5] == vetor11[2] || vetor1[5] == vetor12[2] || vetor1[5] == vetor13[2])
      {
        var1 = 1;
      }
    }
  }
  if (vetor2[3] == vetor6[3] || vetor2[3] == vetor7[3] || vetor2[3] == vetor8[3] || vetor2[3] == vetor9[3] || vetor2[3] == vetor10[3] || vetor2[3] == vetor11[3] || vetor2[3] == vetor12[3] || vetor2[3] == vetor13[3])
  {
    if (vetor2[4] == vetor6[4] || vetor2[4] == vetor7[4] || vetor2[4] == vetor8[4] || vetor2[4] == vetor9[4] || vetor2[4] == vetor10[4] || vetor2[4] == vetor11[4] || vetor2[4] == vetor12[4] || vetor2[4] == vetor13[4])
    {
      if (vetor2[5] == vetor6[5] || vetor2[5] == vetor7[5] || vetor2[5] == vetor8[5] || vetor2[5] == vetor9[5] || vetor2[5] == vetor10[5] || vetor2[5] == vetor11[5] || vetor2[5] == vetor12[5] || vetor2[5] == vetor13[5])
      {
        var2 = 1;
      }
    }
  }
  if (vetor3[3] == vetor6[6] || vetor3[3] == vetor7[6] || vetor3[3] == vetor8[6] || vetor3[3] == vetor9[6] || vetor3[3] == vetor10[6] || vetor3[3] == vetor11[6] || vetor3[3] == vetor12[6] || vetor3[3] == vetor13[6])
  {
    if (vetor3[4] == vetor6[7] || vetor3[4] == vetor7[7] || vetor3[4] == vetor8[7] || vetor3[4] == vetor9[7] || vetor3[4] == vetor10[7] || vetor3[4] == vetor11[7] || vetor3[4] == vetor12[7] || vetor3[4] == vetor13[7])
    {
      if (vetor3[5] == vetor6[8] || vetor3[5] == vetor7[8] || vetor3[5] == vetor8[8] || vetor3[5] == vetor9[8] || vetor3[5] == vetor10[8] || vetor3[5] == vetor11[8] || vetor3[5] == vetor12[8] || vetor3[5] == vetor13[8])
      {
        var3 = 1;
      }
    }
  }

  if (vetor4[3] == vetor6[9] || vetor4[3] == vetor7[9] || vetor4[3] == vetor8[9] || vetor4[3] == vetor9[9] || vetor4[3] == vetor10[9] || vetor4[3] == vetor11[9] || vetor4[3] == vetor12[9] || vetor4[3] == vetor13[9])
  {
    if (vetor4[4] == vetor6[10] || vetor4[4] == vetor7[10] || vetor4[4] == vetor8[10] || vetor4[4] == vetor9[10] || vetor4[4] == vetor10[10] || vetor4[4] == vetor11[10] || vetor4[4] == vetor12[10] || vetor4[4] == vetor13[10])
    {
      if (vetor4[5] == vetor6[11] || vetor4[5] == vetor7[11] || vetor4[5] == vetor8[11] || vetor4[5] == vetor9[11] || vetor4[5] == vetor10[11] || vetor4[5] == vetor11[11] || vetor4[5] == vetor12[11] || vetor4[5] == vetor13[11])
      {
        var4 = 1;
      }
    }
  }
  if (vetor5[3] == vetor6[12] || vetor5[3] == vetor7[12] || vetor5[3] == vetor8[12] || vetor5[3] == vetor9[12] || vetor5[3] == vetor10[12] || vetor5[3] == vetor11[12] || vetor5[3] == vetor12[12] || vetor5[3] == vetor13[12])
  {
    if (vetor5[4] == vetor6[13] || vetor5[4] == vetor7[13] || vetor5[4] == vetor8[13] || vetor5[4] == vetor9[13] || vetor5[4] == vetor10[13] || vetor5[4] == vetor11[13] || vetor5[4] == vetor12[13] || vetor5[4] == vetor13[13])
    {
      if (vetor5[5] == vetor6[14] || vetor5[5] == vetor7[14] || vetor5[5] == vetor8[14] || vetor5[5] == vetor9[14] || vetor5[5] == vetor10[14] || vetor5[5] == vetor11[14] || vetor5[5] == vetor12[14] || vetor5[5] == vetor13[14])
      {
        var5 = 1;
      }
    }
  }

  if (var1 == 1 && var2 == 1 && var3 == 1 && var4 == 1 && var5 == 1)
  {
    Serial.println(palavra); // CASO OS VALORES LIDOS BATEREM COM  OS SALVOS, ESCREVE ESSA FRASE NO MONITOR SERIAL
    var1 = 0;
    var2 = 0;
    var3 = 0;
    var4 = 0;
    var5 = 0;
  }
}

void leitura() //USEI O VOID PRA FACILITAR A CHAMADA DA LEITURA, DIMINUIR LINHAS DE CODIGO
{
  Wire.beginTransmission(MPU); // INICIO DA LEITURA
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU, 14, true);
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  GyX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}
void multiplex()
{
  //INICIA A PARTE DE MULTIPLEXAÇÃO
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
  leitura();
  vetor1[0] = AcX;
  vetor1[1] = AcY;
  vetor1[2] = AcZ;
  vetor1[3] = GyX;
  vetor1[4] = GyY;
  vetor1[5] = GyZ;
  //Serial.println("GyX1");
  //Serial.println(GyY);
  //delay(500);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
  leitura();
  vetor2[0] = AcX;
  vetor2[1] = AcY;
  vetor2[2] = AcZ;
  vetor2[3] = GyX;
  vetor2[4] = GyY;
  vetor2[5] = GyZ;
  //Serial.println(GyY);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
  leitura();
  vetor3[0] = AcX;
  vetor3[1] = AcY;
  vetor3[2] = AcZ;
  vetor3[3] = GyX;
  vetor3[4] = GyY;
  vetor3[5] = GyZ;
  //Serial.println(AcY);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  digitalWrite(pin5, LOW);
  leitura();
  vetor4[0] = AcX;
  vetor4[1] = AcY;
  vetor4[2] = AcZ;
  vetor4[3] = GyX;
  vetor4[4] = GyY;
  vetor4[5] = GyZ;
  //Serial.println(GyY);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, HIGH);
  leitura();
  vetor5[0] = AcX;
  vetor5[1] = AcY;
  vetor5[2] = AcZ;
  vetor5[3] = GyX;
  vetor5[4] = GyY;
  vetor5[5] = GyZ;
  //Serial.println(GyY);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
}
void mem1()
{
  //USEI NOVAMENTE PARA DIMINUIR CODIGO, ESSA PARTE É ONDE FICAM ARMAZENADOS OS DADOS SALVOS PRA COMPARAR COM OS DADOS EM TEMPO REAL
  vetor6[0] = vetor1[3];
  vetor6[1] = vetor1[4];
  vetor6[2] = vetor1[5];
  vetor6[3] = vetor2[3];
  vetor6[4] = vetor2[4];
  vetor6[5] = vetor2[5];
  vetor6[6] = vetor3[3];
  vetor6[7] = vetor3[4];
  vetor6[8] = vetor3[5];
  vetor6[9] = vetor4[3];
  vetor6[10] = vetor4[4];
  vetor6[11] = vetor4[5];
  vetor6[12] = vetor5[3];
  vetor6[13] = vetor5[4];
  vetor6[14] = vetor5[5];
}
void mem2()
{
  vetor7[0] = vetor1[3];
  vetor7[1] = vetor1[4];
  vetor7[2] = vetor1[5];
  vetor7[3] = vetor2[3];
  vetor7[4] = vetor2[4];
  vetor7[5] = vetor2[5];
  vetor7[6] = vetor3[3];
  vetor7[7] = vetor3[4];
  vetor7[8] = vetor3[5];
  vetor7[9] = vetor4[3];
  vetor7[10] = vetor4[4];
  vetor7[11] = vetor4[5];
  vetor7[12] = vetor5[3];
  vetor7[13] = vetor5[4];
  vetor7[14] = vetor5[5];
}
void mem3()
{
  vetor8[0] = vetor1[3];
  vetor8[1] = vetor1[4];
  vetor8[2] = vetor1[5];
  vetor8[3] = vetor2[3];
  vetor8[4] = vetor2[4];
  vetor8[5] = vetor2[5];
  vetor8[6] = vetor3[3];
  vetor8[7] = vetor3[4];
  vetor8[8] = vetor3[5];
  vetor8[9] = vetor4[3];
  vetor8[10] = vetor4[4];
  vetor8[11] = vetor4[5];
  vetor8[12] = vetor5[3];
  vetor8[13] = vetor5[4];
  vetor8[14] = vetor5[5];
}
void mem4()
{
  vetor9[0] = vetor1[3];
  vetor9[1] = vetor1[4];
  vetor9[2] = vetor1[5];
  vetor9[3] = vetor2[3];
  vetor9[4] = vetor2[4];
  vetor9[5] = vetor2[5];
  vetor9[6] = vetor3[3];
  vetor9[7] = vetor3[4];
  vetor9[8] = vetor3[5];
  vetor9[9] = vetor4[3];
  vetor9[10] = vetor4[4];
  vetor9[11] = vetor4[5];
  vetor9[12] = vetor5[3];
  vetor9[13] = vetor5[4];
  vetor9[14] = vetor5[5];
}
void mem5()
{
  vetor10[0] = vetor1[3];
  vetor10[1] = vetor1[4];
  vetor10[2] = vetor1[5];
  vetor10[3] = vetor2[3];
  vetor10[4] = vetor2[4];
  vetor10[5] = vetor2[5];
  vetor10[6] = vetor3[3];
  vetor10[7] = vetor3[4];
  vetor10[8] = vetor3[5];
  vetor10[9] = vetor4[3];
  vetor10[10] = vetor4[4];
  vetor10[11] = vetor4[5];
  vetor10[12] = vetor5[3];
  vetor10[13] = vetor5[4];
  vetor10[14] = vetor5[5];
}
void mem6()
{
  vetor11[0] = vetor1[3];
  vetor11[1] = vetor1[4];
  vetor11[2] = vetor1[5];
  vetor11[3] = vetor2[3];
  vetor11[4] = vetor2[4];
  vetor11[5] = vetor2[5];
  vetor11[6] = vetor3[3];
  vetor11[7] = vetor3[4];
  vetor11[8] = vetor3[5];
  vetor11[9] = vetor4[3];
  vetor11[10] = vetor4[4];
  vetor11[11] = vetor4[5];
  vetor11[12] = vetor5[3];
  vetor11[13] = vetor5[4];
  vetor11[14] = vetor5[5];
}
void mem7()
{
  vetor12[0] = vetor1[3];
  vetor12[1] = vetor1[4];
  vetor12[2] = vetor1[5];
  vetor12[3] = vetor2[3];
  vetor12[4] = vetor2[4];
  vetor12[5] = vetor2[5];
  vetor12[6] = vetor3[3];
  vetor12[7] = vetor3[4];
  vetor12[8] = vetor3[5];
  vetor12[9] = vetor4[3];
  vetor12[10] = vetor4[4];
  vetor12[11] = vetor4[5];
  vetor12[12] = vetor5[3];
  vetor12[13] = vetor5[4];
  vetor12[14] = vetor5[5];
}
void mem8()
{
  vetor13[0] = vetor1[3];
  vetor13[1] = vetor1[4];
  vetor13[2] = vetor1[5];
  vetor13[3] = vetor2[3];
  vetor13[4] = vetor2[4];
  vetor13[5] = vetor2[5];
  vetor13[6] = vetor3[3];
  vetor13[7] = vetor3[4];
  vetor13[8] = vetor3[5];
  vetor13[9] = vetor4[3];
  vetor13[10] = vetor4[4];
  vetor13[11] = vetor4[5];
  vetor13[12] = vetor5[3];
  vetor13[13] = vetor5[4];
  vetor13[14] = vetor5[5];
}
