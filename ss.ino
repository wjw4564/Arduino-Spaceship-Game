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
int wall_count[5][3] = {{-1,-1,0}, {-1,-1,0} ,{-1,-1,0} ,{-1,-1,0}, {-1,-1,0}}; // 내용물 저장
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
  lcd.setCursor(0, UpDown); //캐릭터 생성 위치?
  lcd.write(0);

  pinMode(8, INPUT);
}

void loop() //여기 아래는 계속 무한반복
{
  clickcount = clickcount + 1; //0.5초 세기
  int push = digitalRead(8);

  if (push == HIGH and clickcount >= 5) //버튼이 눌려지고 0.5초가 지나야 되는 조건
  {
    clickcount = 0; 
    if (UpDown == 1)
    {
      UpDown = 0;
    }
    else if (UpDown == 0)
    { 
      UpDown = 1;    //버튼 누르면 캐릭터 위치 변경
    }
  }

  if (wawa == 30 or firstattack == true)  //3초가 지나거나 or 첫 블럭 일때 
  {
    if (random(100) >= 70 or firstattack == true)  //랜덤수가 70이상 or 첫 블럭 일때
    {
      firstattack = false; 
      if (object == 6)
        object = 0;
      wall_count[object][2] = 1;  //object번째의 블럭 생성
      object = object + 1;
    }
  }
  
  for (int x = 0; x <= 5; x++)  //x 5번 반복
  {
    if (wall_count[x][2] == 1)
    {
      /*응애교육*/
      if (wall_count[x][0] == -1 and wall_count[x][1] == -1)
      {
        wall_count[x][0] = 15;
        if (random(1, 100) >= 50)
        {
          wall_count[x][1] = 0;
        }
        else
        {
          wall_count[x][1] = 1;
        }
      }
      /*틀딱교육*/
      else
      {
        /*관짝행*/
        if (wall_count[x][0] == 0)
        {
          wall_count[x][0] = -1;
          wall_count[x][1] = -1;
          wall_count[x][2] = 0;
        }
        else
        {
          wall_count[x][0] = wall_count[x][0] - 1;
        }
      }
    }
  }
  
  lcd.clear();
  for (int i = 0; i <= 5; i++)
  {
    if (wall_count[i][2] == 1)
    {
      lcd.setCursor(wall_count[i][0], wall_count[i][1]);
      lcd.write(1);
    }
  }
  lcd.setCursor(0, UpDown);
  lcd.write(0);
  wawa = wawa + 1;
  delay(100);
}
