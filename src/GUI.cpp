#include "vex.h"
#include "vector"
#include "XDrive.cpp"

enum TabType {
  AUTON, MOTOR, SENSOR, ALL
};

enum Auton {
  BLUELEFT, BLUERIGHT, REDLEFT, REDRIGHT
};

class AutonButton {
  public:
    string text;
    color clr;
    bool pressed;
    int x, y, w, h;
    Auton auton;

    AutonButton(int X, int Y, int W, int H, color CLR, string TEXT, Auton AUTON) {
      x = X;
      y = Y;
      w = W;
      h = H;
      clr = CLR;
      text = TEXT;
      auton = AUTON;
    }

    void draw(brain::lcd screen) {
      screen.drawRectangle(x, y, w, h, clr);
      if (!text.empty()) {
        screen.printAt(x + (w - screen.getStringWidth(text.c_str())) / 2, y + (h - screen.getStringWidth(text.c_str())) / 2, text.c_str());
      }
    }

    void update(int screenX, int screenY) {
      if (screenX >= x && screenY >= y && screenX <= x + w && screenY <= y + h) {
        pressed = true;
      } else {
        pressed = false;
      }
    }
};

class TabButton {
  public: 
    string text;
    color clr;
    TabType tab;
    bool pressed;
    int x, y, w, h;

    TabButton(int X, int Y, int W, int H, color CLR, TabType TAB, string TEXT) {
      x = X;
      y = Y;
      w = W;
      h = H;
      clr = CLR;
      text = TEXT;
      tab = TAB;
    }

    void draw(brain::lcd screen, TabType currentTab) {
      screen.drawRectangle(x, y, w, h, clr);
      if (!text.empty()) {
        screen.printAt(x + (w - screen.getStringWidth(text.c_str())) / 2, y + (h - screen.getStringWidth(text.c_str())) / 2, text.c_str());
      }

    }

    void update(int screenX, int screenY) {
      if (screenX >= x && screenY >= y && screenX <= x + w && screenY <= y + h) {
        pressed = true;
      } else {
        pressed = false;
      }
    }
};

class GUI {
  public:
    brain::lcd screen;
    vector<MotorController> motors;
    vector<TabButton> tabs;
    vector<AutonButton> autons;
    TabType currentTab = TabType::MOTOR;
    Auton autonChoice = Auton::BLUELEFT;
    bool pressing;

    GUI(brain::lcd SCREEN, vector<MotorController> mtrs) {
      screen = SCREEN;
      motors = mtrs;
      tabs.push_back(TabButton(0, 0, 155, 50, color(238, 238, 238), TabType::AUTON, "Auton"));
      tabs.push_back(TabButton(165, 0, 155, 50, color(238, 238, 238), TabType::MOTOR, "Motors"));
      tabs.push_back(TabButton(330, 0, 155, 50, color(238, 238, 238), TabType::SENSOR, "Sensors"));
    }

    // Draw Elements
    void draw() {
      screen.clearScreen(color(51, 51, 51));
      for (int i = 0; i < tabs.size(); i++) {
        tabs.at(i).draw(screen, currentTab);
      }
      if (currentTab == TabType::MOTOR) {
        for (int i = 0; i < motors.size(); i++) {
          MotorController cont = motors.at(i);
          screen.drawRectangle(120 * (i % 4), 50 + 95 * floor(i / 4), 120, 95);
        }
      }
    }

    // Handle Screen Press
    void update() {
      if (screen.pressing()) {
        // Test For Tab Change
        for (TabButton tab: tabs) {
          tab.update(screen.xPosition(), screen.yPosition());
          if (tab.pressed) {
            currentTab = tab.tab;
          }
        }

        // Test For Auton Change
        for (AutonButton button: autons) {
          button.update(screen.xPosition(), screen.yPosition());
          if (button.pressed) {
            autonChoice = button.auton;
          }
        }
        draw(); 
        pressing = true;
      }

      if (pressing && !screen.pressing()) {
        for (int i = 0; i < tabs.size(); i++) {
          tabs.at(i).update(10000, 10000);
        }
        draw(); 
        pressing = false;
      }
    }
};
