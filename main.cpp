// protected - members cannot be accessed from outside the class, however, they can be accessed in inherited classes.

//public - members are accessible from outside the class

//private - members cannot be accessed (or viewed) from outside the class



#include "raylib.h"
using namespace std;

Color Green = Color{38,185,154,255};
Color Dark_Green = Color{20,160,133,255};
Color Light_Green = Color{129,204,184,255};
Color Yellow = Color{243,213,91,255};

int player_score = 0;
int cpu_score = 0;

class Ball{
public:
float x,y; // position
int speed_x,speed_y;
int radius;

void Draw(){
    // DrawCircle(screen_width/2,screen_height/2,radius in float,color);
    DrawCircle(x,y,radius,Yellow);
}
    
void Update(){
    x += speed_x;
    y += speed_y;
    
    
    if(y+radius >= GetScreenHeight() || y-radius <= 0){
        speed_y *= -1;
    }
    if(x+radius >= GetScreenWidth()){ //Cpu wins
        cpu_score += 1;
        ResetBall();
    }
    if(x-radius <= 0){ //Player wins
        player_score += 1;
        ResetBall();
    }    
    /*if(x+radius >= GetScreenWidth() || x-radius <= 0){
        speed_x *= -1;
    }*/
}

void ResetBall(){
    x = GetScreenHeight()/2;
    y = GetScreenWidth()/2;
        
    int speed_choices[2] = {-1,1};
    speed_x *= speed_choices[GetRandomValue(0,1)];
    speed_y *= speed_choices[GetRandomValue(0,1)];
}


};


class Paddle {

protected:
void LimitMovement(){
    //paddle not moving outside the screen
    if(y <= 0){
        y = 0;
    }
    if(y + height >= GetScreenHeight()){
        y = GetScreenHeight() - height;
    }
}

public:
    float x,y; // position
    float width,height;
    int speed;
    
 void Draw(){
    DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE); 
}

void Update(){
    //bool IsKeyDown(int key); // Check if a key is being pressed
    if(IsKeyDown(KEY_UP)){
        y = y - speed;
    }
    if(IsKeyDown(KEY_DOWN)){
        y = y + speed;
    }
    
    LimitMovement();
}
};


class CpuPaddle: public Paddle{
    //cpupaddle now has all the attribute and function of Paddle class
    
    public:
    void Update(int ball_y){
        // if the y center pos of ball > y center pos of paddle .. move the paddle up else down
        
        if(y + height/2 > ball_y){
            y = y - speed;
        }
        if(y + height/2 < ball_y){
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() 
{
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width,screen_height,"My Pong Game!");
    SetTargetFPS(60);

    //ball object
    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;


    //player object
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;
    
    //cpu object
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - player.height/2;
    cpu.speed = 6;
    

    //Game Loop
    while(WindowShouldClose() == false){

        BeginDrawing();
        
        //updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);
        
        //Checking for collisons(circle collides with rec)
        //player collision
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
            ball.speed_x *= -1;
        }
        
        //cpu collision
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})){
            ball.speed_x *= -1;
        }
        
        //drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2,0,screen_width/2,screen_height,Green);        
        DrawCircle(screen_width/2,screen_height/2,150,Light_Green);
        DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);
        ball.Draw();
        
        DrawText(TextFormat("%i",cpu_score),screen_width/4-20,20,80,WHITE); //(text,x,y,fontsize,color)
        DrawText(TextFormat("%i",player_score),3*screen_width/4-20,20,80,WHITE);
         
         
        // DrawRectangle(int posX,int posY,int width,int height,color);
        //DrawRectangle(10,screen_height/2-120/2,25,120,WHITE); // paddle 1
        
        cpu.Draw(); //paddle 1/cpu player 1
        player.Draw(); //paddle 2/player 2
        
        EndDrawing();
    }

    CloseWindow();
}