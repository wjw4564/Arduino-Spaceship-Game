#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 12, 2);
byte sshibal[]{
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

int UpDown = 1;
int undertale[5][3] = {{-1,-1,0}, {-1,-1,0} ,{-1,-1,0} ,{-1,-1,0}, {-1,-1,0}}; // 내용물 저장
int wawa = 5;
int object = 0;
int clickcount = 0;
bool firstattack = true;

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, sshibal);
  lcd.createChar(1, wall);
  lcd.setCursor(0, UpDown);
  lcd.write(0);

  pinMode(8, INPUT);
}

void loop()
{
  clickcount = clickcount + 1;
  Serial.print(clickcount);
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
  if (wawa == 30 or firstattack == true) 
  {
    if (random(100) >= 70 or firstattack == true)
    {
      firstattack = false;
      if (object == 6)
        object = 0;
      undertale[object][2] = 1;
      object = object + 1;
    }
  }

  /*movement*/
  for (int x = 0; x <= 5; x++)
  {
    if (undertale[x][2] == 1)
    {
      /*응애교육*/
      if (undertale[x][0] == -1 and undertale[x][1] == -1)
      {
        undertale[x][0] = 15;
        if (random(1, 100) >= 50)
        {
          undertale[x][1] = 0;
        }
        else
        {
          undertale[x][1] = 1;
        }
      }
      /*틀딱교육*/
      else
      {
        /*관짝행*/
        if (undertale[x][0] == 0)
        {
          undertale[x][0] = -1;
          undertale[x][1] = -1;
          undertale[x][2] = 0;
        }
        else
        {
          undertale[x][0] = undertale[x][0] - 1;
        }
      }
    }
  }
  
  lcd.clear();
  for (int i = 0; i <= 5; i++)
  {
    if (undertale[i][2] == 1)
    {
      lcd.setCursor(undertale[i][0], undertale[i][1]);
      lcd.write(1);
    }
  }
  lcd.setCursor(0, UpDown);
  lcd.write(0);
  wawa = wawa + 1;
  delay(100);
}
