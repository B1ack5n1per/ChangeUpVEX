#include "vex.h"
using namespace vex;
using namespace std;

// Motor Data Format
class MotorData {
  public:
    string name;
    double temp, rotations, rpm;
    MotorData(string title, double heat, double rots, double speed) {
      name = title;
      temp = heat;
      rotations = rots;
      rpm = speed;
    }
};