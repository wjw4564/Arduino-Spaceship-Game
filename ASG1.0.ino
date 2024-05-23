#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 12, 2);
byte character[]{
  0b10000,
  0b01000,
  0b11110,
  0b11111,
  0b11111,
  0b11110,
  0b01000,
  0b10000
};

byte wall[]{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};
void(* resetFunc) (void) = 0;
int UpDown = 1;
int walllist[3][3] = {{-1,-1,0}, {-1,-1,0} ,{-1,-1,0}}; // 내용물 저장
int wawa = 0;
int object = 0;
int clickcount = 0;
bool firstattack = true;
int jotdem = 0;
int score=0;
int nanido = 50;
int nantime = 0;

void setup()
{ 
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, character);
  lcd.createChar(1, wall);
  lcd.setCursor(0, UpDown);
  lcd.write(0);

  pinMode(8, INPUT);
  lcd.clear();
  lcd.print(3);
  delay(1000);
  lcd.clear();
  lcd.print(2);
  delay(1000);
  lcd.clear();
  lcd.print(1);
  delay(1000);
  lcd.clear();
}

void loop()
{
  if (jotdem == 0)
  {
    clickcount = clickcount + 1;
    int wasans = digitalRead(8);
  
    /* wasans == 버튼 입력 감지, clickcount == 클릭 후 혹은 시작 후 0.5초 후 클릭이 먹히게 하는 변수, 이거 두 개 이용해서 플레이어의 위치 조정하는 코드 */
    if (wasans == HIGH and clickcount >= 5)
    {
      clickcount = 0;
      if (UpDown == 1)
      {
        UpDown = 0;
      }
      else if (UpDown == 0)
      { /**/
        UpDown = 1;
      }
    }
  
    /* 처음 혹은 처음생성 후 3초마다 특정 확률로 벽 생성 */
    if (wawa == 10 or firstattack == true) 
    {
      if (wawa == 10 or firstattack == true)
      {
        firstattack = false;
        if (object == 3)
          object = 0;
        walllist[object][2] = 1;
        object = object + 1;
      }
      wawa = 0;
    }
  
    /*movement*/
    for (int x = 0; x <= 5; x++)
    {
      if (walllist[x][2] == 1)
      {
        /*응애교육*/
        if (walllist[x][0] == -1 and walllist[x][1] == -1)
        {
          walllist[x][0] = 15;
          if (random(1, 100) >= 50)
          {
            walllist[x][1] = 0;
          }
          else
          {
            walllist[x][1] = 1;
          }
        }
        /*틀딱교육*/
        else
        {
          /*관짝행*/
          if (walllist[x][0] == 0)
          {
            walllist[x][0] = -1;
            walllist[x][1] = -1;
            walllist[x][2] = 0;
            score = score + 1;
          }
          else
          {
            walllist[x][0] = walllist[x][0] - 1;
          }
        }
      }
    }
    if (nantime == 50)
    {
      nantime = 0;
      nanido = nanido - 1;
    }
    
    lcd.clear();
    for (int i = 0; i <= 3; i++)
    {
      if (walllist[i][0] == 0 and walllist[i][1] == UpDown)
      {
        jotdem = 1;
      }
      else if (walllist[i][2] == 1)
      {
        lcd.setCursor(walllist[i][0], walllist[i][1]);
        lcd.write(1);
      }
    }
    
    lcd.setCursor(0, UpDown);
    lcd.write(0);
    wawa = wawa + 1; 
    nantime = nantime + 1;
    delay(nanido);
  }
  else
  {
    lcd.clear();
    lcd.print("GAME OVER");
    delay(1000);
    lcd.clear();
    while (digitalRead(8) == LOW)
    {
      lcd.print("score:");
      lcd.print(score);
      delay(1000);
      lcd.clear();
    }
    resetFunc();
    jotdem = 0;
  }
}
