#include "vex.h"
#include "vector"
#include "XDrive.cpp"

enum Tabs {
  AUTON, MOTOR, SENSOR
};

class Element {
  public:
    virtual void draw(brain::lcd screen, Tabs currentTab);
};

class Rectangle {
  public: 
    int x,y,w,h;
    Rectangle(int X, int Y, int W, int H) {
      x = X;
      y = Y;
      w = W;
      h = H;
    }
    Rectangle() {
      x = 0;
      y = 0;
      w = 0;
      h = 0;
    }
};

class Btn {
  public: 
    Tabs tab;
    Rectangle rect;
    color clr;
    bool pressed;

    Btn(Rectangle rectangle, color CLR, Tabs TAB) {
      rect = rectangle;
      clr = CLR;
      tab = TAB;
    }

    void draw(brain::lcd screen, Tabs currentTab) {
      if (currentTab == tab) {
        screen.drawRectangle(rect.x, rect.y, rect.w, rect.h, clr);
      }
    }

    void update(int x, int y) {
      if (x >= rect.x && y >= rect.y && x <= rect.x + rect.w && y <= rect.y + rect.h) {
        pressed = true;
      } else {
        pressed = false;
      }
    }
};

class GUI {
  public:
    Tabs currentTab = Tabs::MOTOR;
    brain::lcd screen;
    bool pressing;
    std::vector<MotorController> motors;
    Btn buttons[2] = { Btn(Rectangle(0, 0, 200, 50), color(238, 238, 238), Tabs::MOTOR), Btn(Rectangle(250, 0, 200, 50), color(238, 238, 238), Tabs::SENSOR)};

    GUI(brain::lcd SCREEN, vector<MotorController> mtrs) {
      screen = SCREEN;
      motors = mtrs;
    }

    void draw() {
      screen.clearScreen(color(51, 51, 51));
      for (int i = 0; i < 2; i++) {
        buttons[i].draw(screen, currentTab);
      }
      if (currentTab == Tabs::MOTOR) {
        for (int i = 0; i < motors.size(); i++) {
          MotorController cont = motors.at(i);
          screen.drawRectangle(120 * (i % 4), 50 + 95 * floor(i / 4), 120, 95);
        }
      }
    }

    void update() {
      if (screen.pressing()) {
        for (int i = 0; i < 2; i++) {
          buttons[i].update(screen.xPosition(), screen.yPosition());
        }
        if (buttons[0].pressed) {
          currentTab = Tabs::SENSOR;
        }
        if (buttons[1].pressed) {
          currentTab = Tabs::MOTOR;
        }

        draw(); 
        pressing = true;
      }

      if (pressing && !screen.pressing()) {
        for (int i = 0; i < 2; i++) {
          buttons[i].update(10000, 10000);
        }
        draw(); 
        pressing = false;
      }
    }
};
