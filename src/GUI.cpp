#include "vex.h"
#include "vector"
#include "sstream"
#include "iostream"
#include "string"
#include "Autons.cpp"

enum TabType {
  AUTON, MOTOR, SENSOR, ALL
};

enum Auton {
  BLUELEFT, BLUERIGHT, REDLEFT, REDRIGHT
};

template <typename T> string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

class AutonButton {
  public:
    string text;
    color clr;
    bool pressed;
    int x, y, w, h;
    Auton auton;

    AutonButton(int X, int Y, int W, int H, color CLR, Auton AUTON, string TEXT) {
      x = X;
      y = Y;
      w = W;
      h = H;
      clr = CLR;
      text = TEXT;
      auton = AUTON;
    }

    // Draw Element
    void draw(brain::lcd screen) {
      screen.drawRectangle(x, y, w, h, clr);
      if (!text.empty()) {
        screen.printAt(x + (w - screen.getStringWidth(text.c_str())) / 2, y + (h - screen.getStringWidth(text.c_str())) / 2, text.c_str());
      }
    }

    // Test if Pressed
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
    TabType tab;
    bool pressed;
    int x, y, w, h;

    TabButton(int X, int Y, int W, int H, TabType TAB, string TEXT) {
      x = X;
      y = Y;
      w = W;
      h = H;
      text = TEXT;
      tab = TAB;
    }

    // Draw Element
    void draw(brain::lcd screen, TabType currentTab) {
      screen.drawRectangle(x, y, w, h, color(238, 238, 238));
      if (!text.empty()) {
        screen.printAt(x + (w - screen.getStringWidth(text.c_str())) / 2, y + (h - screen.getStringWidth(text.c_str())) / 2, text.c_str());
      }

    }

    // Test if Pressed
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

    // Add Elements to GUI
    GUI(brain::lcd SCREEN, vector<MotorController> mtrs) {
      screen = SCREEN;
      motors = mtrs;
      tabs.push_back(TabButton(0, 0, 155, 50, TabType::AUTON, "Auton"));
      tabs.push_back(TabButton(165, 0, 155, 50, TabType::MOTOR, "Motors"));
      tabs.push_back(TabButton(330, 0, 155, 50, TabType::SENSOR, "Sensors"));
      autons.push_back(AutonButton(5, 55, 230, 85, color(75, 75, 219), Auton::BLUELEFT, "Blue Left"));
      autons.push_back(AutonButton(245, 55, 230, 85, color(75, 75, 219), Auton::BLUERIGHT, "Blue Right"));
      autons.push_back(AutonButton(5, 150, 230, 85, color(224, 61, 61), Auton::REDLEFT, "Red Left"));
      autons.push_back(AutonButton(245, 150, 230, 85, color(224, 61, 61), Auton::REDRIGHT, "Red Right"));
      screen.setFont(fontType::mono12);
    }

    // Draw Elements
    void draw() {
      screen.clearScreen(color(51, 51, 51));
      for (int i = 0; i < tabs.size(); i++) {
        tabs.at(i).draw(screen, currentTab);
      }
      if (currentTab == TabType::MOTOR) {
        for (int i = 0; i < motors.size(); i++) {
          MotorData data = motors.at(i).getData();
          int x = 120 * (i % 4);
          int y = 50 + 95 * floor(i / 4);
          screen.drawRectangle(x, y, 120, 95, color(210, 210, 210));
          screen.printAt(x + 5, y + 5, data.name.c_str());
          screen.printAt(x + 5, y + 19, to_string(data.temp).append(" C").c_str());
          screen.printAt(x + 5, y + 31, to_string(data.rotations).append(" deg").c_str());
          screen.printAt(x + 5, y + 48, to_string(data.rpm).append(" rpm").c_str());
        }
      }
      if (currentTab == TabType::AUTON) {
        for (AutonButton btn : autons) {
          btn.draw(screen);
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
        if (currentTab == TabType::AUTON) {
          for (AutonButton button: autons) {
            button.update(screen.xPosition(), screen.yPosition());
            if (button.pressed) {
              autonChoice = button.auton;
            }
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
