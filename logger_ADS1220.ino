
#include <OLED_I2C.h>

extern uint8_t SmallFont[];
OLED myOLED(SDA, SCL, 8);
/*
  Код для обработки инкрементального энкодера, 
  ручка которого перемещается дискретно,
  и переход из одного положения в другое сопровождается сменой
  всех четырёх состояний выходов. 
*/

// используемые пины
const int encPinA = 4; // первый выход энкодера
const int encPinB = 3; // второй выход энкодера
const int PinSW = 5;   // кнопка

// переменные для хранения состояний пинов
// (чтобы не производить ненужных считываний)
bool encA;
bool encB;
bool sw;
bool mem = false;
bool menu_on = false;
byte menu_list_count = 0;

// переменные для информации о предыдущих состояниях
// и движениях энкодера
bool prev11;             // предыдущее состояние было 11
bool clockwise1 = true;  // переход по часовой стрелке 1 (от 11)
bool clockwise2 = false; // переход по часовой стрелке 2 (от 00)

// изменяемое значение
int old_value = 0;
int value = 0;
byte pos_pointer = 16;

void setup()
{
  Serial.begin(115200);
  pinMode(encPinA, INPUT);
  digitalWrite(encPinA, HIGH); // включаем подтяжку к плюсу
  pinMode(encPinB, INPUT);
  digitalWrite(encPinB, HIGH);
  pinMode(PinSW, OUTPUT);
  digitalWrite(PinSW, HIGH); // включаем подтяжку к плюсу

  InitDisplay();
}

void loop()
{

  //считывание кнопки энкодера
  SwitchClick(digitalRead(PinSW));

  // считывание состояния выходов энкодера
  encA = digitalRead(encPinA);
  encB = digitalRead(encPinB);
  ConditionEncoder();

  if (menu_on == true && old_value != value)
  {
    PosPointer();
    switch (menu_list_count)
    {
    case 0:
      DrawMenuList0(pos_pointer);
      break;
    case 1:
      DrawMenuList1(pos_pointer);
      break;
    case 2:
      DrawMenuList2(pos_pointer);
      break;
    case 3:
      DrawMenuList3(pos_pointer);
      break;
    }

    //Serial.println("--------------" + String(pos_pointer));
  }

  old_value = value;
}
