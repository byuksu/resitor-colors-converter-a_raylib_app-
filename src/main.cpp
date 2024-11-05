#include "raylib.h"
#include "button.hpp"
#include <iostream>
#include <cmath>

using namespace std;
static int inputValue=0;
static Color Rcolor[]={WHITE,WHITE,WHITE,WHITE};
static int count=0;
static int Cvalues[]={0,0,0};
static string ResValueText="resistor value";
void gitColour(float x);
void gitRes(Color *rcolor);
void ColortoValue(int *val);
void gitVal(bool &mouseOnText, Rectangle &textBox, bool &textBoxEditMode, int &letterCount, char *inputText, bool &validInput);
void drawVal(Rectangle &textBox, bool &textBoxEditMode, char *inputText, bool &validInput);
void ValuetoColor(int &value);
Color getcolor(int &x);
Rectangle drawExitButton(float yposition);
Rectangle drawAgainButton(float yposition);
const char* colorNames[] = {"Black", "Brown", "Red", "Orange", "Yellow", "Purple", "Blue", "Gray", "White", "Green"};
static int colornum[3]={0,0,0};
int main(void)
{
    InitWindow(800, 600, "colour converter");
    SetTargetFPS(60);
    //giriş sayfası
    button button1("resources/1.png", {300, 150});
    button button2("resources/2.png", {300, 300});
    bool Button1flag = true;
    bool Button2flag = true;
    
    // Kullanıcı girdisi için karakter dizisi
    char inputText[15] = "\0";  // Giriş kutusu başlangıçta boş
    int letterCount = 0;        // Girilen karakter sayısı
    bool mouseOnText = false;   // Fare ile metin kutusunda mıyız?

    Rectangle textBox = {300, 200, 225, 50};  // Metin kutusunun boyutu ve konumu
    bool textBoxEditMode = false;  // Metin kutusunun düzenleme modu açık mı?

    bool validInput = false; // Girilen sayı geçerli mi?

    while (WindowShouldClose() == false)
    {
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        BeginDrawing();
        ClearBackground(WHITE);

        if ((button1.isPressed(mousePosition, mousePressed)) || !Button1flag)
        {

            Button1flag = false;
            gitColour(60);
            if(count==4){
                Button1flag = true;
                count=0;
                Rcolor[0]=WHITE;
                Rcolor[1]=WHITE;
                Rcolor[2]=WHITE;
                ResValueText="resistor value";
            }
            
        }
        else if ((button2.isPressed(mousePosition, mousePressed)) || !Button2flag)
        {
            Button2flag = false;
            gitVal(mouseOnText,textBox,textBoxEditMode,letterCount,inputText,validInput);
            drawVal(textBox, textBoxEditMode, inputText, validInput);
            Rectangle exbut = drawExitButton(500);
            if(CheckCollisionPointRec(GetMousePosition(),exbut) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Button2flag = true;
            }

        }
        else if (Button1flag && Button2flag)
        {
            button1.draw();
            button2.draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


void gitColour(float x)
{
    gitRes(Rcolor);
    const char* charPtr = ResValueText.c_str();
    DrawRectangleLines(50,400,400,100,BLACK);
    DrawText(charPtr,55,420,50,BLACK);
    // Renk dairelerinin merkezleri ve yarıçapı
    Vector2 positions[] = {{30, x}, {90, x}, {150, x}, {210, x}, {270, x}, {330, x}, {390, x}, {450, x}, {510, x}};
    Color colors[] = {BLACK, BROWN, RED, ORANGE, YELLOW, PURPLE, BLUE, GRAY, WHITE};
    
    
    for (int i = 0; i < 9; i++)
    {
        DrawCircle(positions[i].x, positions[i].y, 20, BLACK);  // Dış çember
        DrawCircle(positions[i].x, positions[i].y, 14, colors[i]);  // İç çember
        Rectangle exitRec=drawExitButton(200);
        Rectangle newRec=drawAgainButton(300);
        // Eğer fare bu dairenin üzerindeyse ve tıklanmışsa renk yazdır
        if (CheckCollisionPointCircle(GetMousePosition(), positions[i], 20) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Cvalues[count]=i;
            Rcolor[count++]=colors[i];
            if(count==3){
                ColortoValue(Cvalues);
            }
        }//exit button
        else if(CheckCollisionPointRec(GetMousePosition(),exitRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            count=4;
        }//again button
        else if(CheckCollisionPointRec(GetMousePosition(),newRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            count=0;
            Rcolor[0]=WHITE;
            Rcolor[1]=WHITE;
            Rcolor[2]=WHITE;
            ResValueText="resistor value";
        }
    }
}

void gitRes(Color *rcolor)
{
    Rectangle rec = {600, 50, 100, 400};
    DrawRectangle(645, 10, 10, 40, BLACK);
    DrawRectangleRoundedLines(rec, 10, 1, 10, BLACK);
    DrawRectangleLines(600, 150, 100, 10, BLACK); //first color
    DrawRectangle(601, 151, 99, 8, rcolor[0]);
    DrawRectangleLines(600, 200, 100, 10, BLACK); //second color
    DrawRectangle(601, 201, 99, 8, rcolor[1]);
    DrawRectangleLines(600, 250, 100, 10, BLACK); //third color
    DrawRectangle(601, 251, 99, 8, rcolor[2]);
    DrawRectangleLines(600, 350, 100, 10, BLACK);
    DrawRectangle(645, 460, 10, 40, BLACK);
}

void ColortoValue(int *val)
{
    int value;
    value = (val[0] * 10) + val[1];
    int res = (int)pow(10.0, val[2]);
    value *= res;
    if(value >= 1000000) {
        ResValueText = to_string(value / 1000000.0);
        ResValueText.erase(ResValueText.length() - 4, 4);
        ResValueText += " M ohm";
    } else if(value >= 1000) {
        ResValueText = to_string(value / 1000.0);
        ResValueText.erase(ResValueText.length() - 4, 4);
        ResValueText += " K ohm";
    } else {
        ResValueText = to_string(value);
        //ResValueText.erase(ResValueText.length() - 4, 4);
        ResValueText += " ohm";
    }
}

void gitVal(bool &mouseOnText,Rectangle &textBox, bool &textBoxEditMode, int &letterCount, char *inputText, bool &validInput)
{
    // Fare metin kutusunun üzerinde mi?
    mouseOnText = CheckCollisionPointRec(GetMousePosition(), textBox);

    // Fareye tıklanırsa düzenleme moduna geç
    if (mouseOnText && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) textBoxEditMode = true;
    else if (!mouseOnText && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) textBoxEditMode = false;

    // Düzenleme modundaysa klavyeden giriş al
    if (textBoxEditMode) {
        int key = GetCharPressed();
        // Harf ya da sayı tuşlarına basılırsa input'a ekle
        if ((key >= 48) && (key <= 57) && (letterCount < 14)) {
            inputText[letterCount] = (char)key;
            letterCount++;
        }

        // Backspace'e basılırsa son karakteri sil
        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            inputText[letterCount] = '\0';
        }
    }

    // Girilen sayıyı kontrol edip karesini hesapla
    if (letterCount > 0) {
        validInput = true;
        int number = atoi(inputText); // Girilen sayıyı tam sayıya çevir
        inputValue=number;
    } else {
        validInput = false;
    }

}

void drawVal(Rectangle &textBox, bool &textBoxEditMode, char *inputText, bool &validInput)
{
       // "Bir sayı giriniz" yazısı
    DrawText("Bir deger giriniz:", 50, 160, 20, DARKGRAY);

    // Metin kutusunu çiz (fare ile tıklanırsa yeşil çerçeve, aksi halde gri)
    DrawRectangleRec(textBox, LIGHTGRAY);
    if (textBoxEditMode) DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, GREEN);
    else DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);

    // Girilen metni kutucuğa yazdır
    DrawText(inputText, textBox.x + 5, textBox.y + 10, 30, MAROON);

    
    DrawText("RENKLER", 50, 300, 20, DARKGRAY);

    if (validInput) {
        
        ValuetoColor(inputValue);
        DrawText(colorNames[colornum[0]], 50, 350, 40, Rcolor[0]); // ilk değer
        DrawText(colorNames[colornum[1]], 50, 400, 40, Rcolor[1]); // ilk değer
        DrawText(colorNames[colornum[2]], 50, 450, 40, Rcolor[2]); // ilk değer
        gitRes(Rcolor);
        

    } else {
        DrawText("Geçerli bir deger giriniz.", 50, 350, 40, RED); // Geçerli sayı girilmediyse uyarı
    }
}
void ValuetoColor(int &value) {
    int multiplier = 0;
    int renk1, renk2;

    while(value > 100) {
        multiplier++;
        value /= 10;
    }

    renk2 = value % 10;
    renk1 = value / 10;
    colornum[0]=renk1;
    colornum[1]=renk2;
    colornum[2]=multiplier;
    Rcolor[0]=getcolor(renk1);
    Rcolor[1]=getcolor(renk2);
    Rcolor[2]=getcolor(multiplier);

}
Color getcolor(int &x) {
    switch(x) {
        case 0: return BLACK;
        case 1: return BROWN;
        case 2: return RED;
        case 3: return ORANGE;
        case 4: return YELLOW;
        case 5: return PURPLE;
        case 6: return BLUE;
        case 7: return GRAY;
        case 8: return WHITE;
        default: return BLACK;
    }
}

Rectangle drawExitButton(float yposition)
{
    Rectangle exitRec = {200, yposition, 200, 80};
    DrawRectangleRounded(exitRec,1,0,RED);
    DrawText("EXIT",230,yposition + 15,50,WHITE);
    return exitRec;
}

Rectangle drawAgainButton(float yposition)
{
    Rectangle newRec = {200, yposition, 200, 80};
    DrawRectangleRounded(newRec,1,0,GREEN);
    DrawText("AGAIN",225,yposition + 15,50,WHITE);
    return newRec;
}
