#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

bool loadinginclang = true;
bool loadinginraylib = false;

void exec() {
    std::system("clang++ projects/test.cpp -o my_program -I$HOME/homebrew/include -L$HOME/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo && ./my_program");
}

void install() {
    std::this_thread::sleep_for(std::chrono::seconds(4));
    #if defined(__APPLE__) || defined(__MACH__)
    std::system("osascript -e 'do shell script \"xcode-select --install\" with administrator privileges'");
    #elif defined(_WIN32) || defined(_WIN64)
    std::system("winget install llvm.llvm --scope user --silent --accept-source-agreements --accept-package-agreements");
    #endif
    loadinginclang = false;
    loadinginraylib = true;
    std::this_thread::sleep_for(std::chrono::seconds(7));
    #if defined(__APPLE__) || defined(__MACH__)
    std::system("mkdir -p ~/homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C ~/homebrew && eval \"$($HOME/homebrew/bin/brew shellenv)\" && echo 'eval \"$($HOME/homebrew/bin/brew shellenv)\"' >> ~/.zprofile && brew install raylib");
    #elif defined(_WIN32) || defined(_WIN64)
    std::system("mkdir %USERPROFILE%\\raylib && curl -L https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_msvc16.zip -o %USERPROFILE%\\raylib.zip && tar -xf %USERPROFILE%\\raylib.zip -C %USERPROFILE%\\raylib --strip-components 1 && del %USERPROFILE%\\raylib.zip");
    #endif
    loadinginraylib = false;
};

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);

    InitWindow(1200, 800, "Code N C++");

    SetTargetFPS(60);

    Rectangle codeView = { 40, 40, 760, 400 };

    Font FiraMono = LoadFont("FiraMono-Regular.ttf");

    std::ifstream myFile("projects/test.cpp");

    std::string CodeText = "";
    int time = 0;
    bool editing = false;
    float initialposxrun = 150;
    float initialposyrun = 600;
    float runbtnx = initialposxrun;
    float runbtny = initialposyrun;
    float runbtntxtsize = 17;
    float easespeed = 10.00000;
    Rectangle runbtnrect = (Rectangle){ runbtnx, runbtny, 150, 20 };
    Rectangle helpbtnrect = { 200, 600, 150, 20 };

    Font Retrofont = LoadFont("FiraMono-Regular.ttf");
    Font Comfortaa = LoadFont("Comfortaa.ttf");

    Color runbtncolor = { 0, 200, 255, 255 };
    std::thread installer_thread(install);
    Color helpbtncolor = { 0, 0, 255 };
    while (!WindowShouldClose()) {
        time += 1 / 60;
        int key = GetKeyPressed();

        if (key != KEY_BACKSPACE && key != KEY_ENTER) {
            if (key != 0) {         
                std::ofstream file("projects/test.cpp", std::ios::app);

                file << (char)key;
                std::cout << CodeText << "\n";

                file.close();

                std::ifstream s("projects/test.cpp");
                std::stringstream d;
                d << s.rdbuf();
                s.close();
        
                CodeText = d.str();
            }
        } else if (key == KEY_BACKSPACE || key == KEY_DELETE) {
            if (key != 0) {            
                std::ifstream file("projects/test.cpp");
                std::stringstream buffer;
                buffer << file.rdbuf();
                std::string content = buffer.str();
                if (!content.empty()) {
                    content.pop_back();
                };

                file.close();

                std::ofstream myFile("projects/test.cpp", std::ios::trunc);
                myFile << content;
                myFile.close();

                std::ifstream s("projects/test.cpp");
                std::stringstream d;
                d << s.rdbuf();
        
                CodeText = d.str();
                s.close();
            }
        } else if (key == KEY_ENTER) {
            if (key != 0) {
                std::ifstream file("projects/test.cpp");
                std::stringstream buffer;
                buffer << file.rdbuf();
                std::string content = buffer.str();
                content = content + "\n";

                file.close();

                std::ofstream myFile("projects/test.cpp", std::ios::trunc);
                myFile << content;
                myFile.close();

                std::ifstream s("projects/test.cpp");
                std::stringstream d;
                d << s.rdbuf();
        
                CodeText = d.str();
                s.close();
            }
        };

        BeginDrawing();

        ClearBackground((Color){ 30, 30, 30, 255 });

        DrawRectangleRoundedLinesEx(codeView, 0.06f, 32, 2.0f, GRAY);

        DrawRectangleRounded(helpbtnrect, 0.1, 32, helpbtncolor);

        DrawTextEx(FiraMono, CodeText.c_str(), (Vector2){ 60, 60 }, 10, 1, WHITE);

        DrawTextEx(Comfortaa, "Hey! I bet you ope-\n-ned this to get help\n, and first of all\n, double click on\n ./CodeNC to open\n the app. \nGo into the \n projects/ \nto see the file \nnamed test.cpp\n, if you don't\n want to use\n the editor \nin the app, click it \nand code there.\nFor more, visit \nhttps://code-and-c.artificial-intelligence.workers.dev", (Vector2){ 810, 40 }, 20, 1, WHITE);

        // LOADING TEXT:

        if (loadinginclang == true) {
            DrawTextEx(Retrofont, "LOADING IN CLANGD: C++ COMPILER.", (Vector2){0,0}, 35, 1, WHITE);
        } else if (loadinginraylib == true) {
            DrawTextEx(Retrofont, "LOADING IN RAYLIB AND HOMEBREW. (FINISHED LOADING CLANGD COMPILER), PLEASE WAIT!", (Vector2){0,0}, 15, 1, WHITE);
        };

        // END OF LOADING TEXT.

        // RUN BUTTON

        Vector2 runbtntxtpos = {runbtnrect.x + ((runbtnrect.width / 2) - 11), runbtnrect.y + ((runbtnrect.height / 2 - 8))};

        DrawRectangleRounded(runbtnrect, 0.5, 32,  runbtncolor);
        DrawTextEx(FiraMono, "Run", runbtntxtpos, runbtntxtsize, 1, (Color){ 230, 230, 230, 255 });

        // END OF RUN BUTTON

        // RUN BUTTON FUNCTIONS:

        if (CheckCollisionPointRec(GetMousePosition(), runbtnrect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            std::thread t1(exec);
            t1.join();
        }
        
        // Check if the user is hovering over the 'run' button.
        
        if (CheckCollisionPointRec(GetMousePosition(), runbtnrect)) {
            if (easespeed > 0) {
                easespeed -= 2.5;
                runbtnrect = { runbtnx - (easespeed / 2), runbtny - (easespeed / 2), runbtnrect.width + easespeed, runbtnrect.height + easespeed};
                runbtnx = runbtnx - (easespeed / 2);
                runbtny = runbtny - (easespeed / 2);
                runbtntxtsize += easespeed / 4;
                runbtntxtpos.x -= 40;
                runbtntxtpos.y -= 40;
                runbtncolor = { 0, 150, 255, 255 };
            }
        } else {
            if (easespeed < 10) {
                easespeed += 2.5;
                runbtnrect = {initialposxrun, initialposyrun, 150, 20};
                runbtnx = initialposxrun;
                runbtny = initialposyrun;
                runbtntxtpos = {runbtnrect.x + ((runbtnrect.width / 2) - 11), runbtnrect.y + ((runbtnrect.height / 2 - 8))};
                runbtntxtsize = 17;
                easespeed = 10;
                runbtncolor = { 0, 200, 255, 255 };
            }
        }

        EndDrawing();
    };
    UnloadFont(FiraMono);
    CloseWindow();
}
