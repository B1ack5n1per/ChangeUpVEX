#include "Motor.cpp"

class XDrive {
  public:
    MotorController leftA, leftB, rightA, rightB;

    XDrive(MotorController a, MotorController b, MotorController c, MotorController d) {
      leftA = a;
      leftB = b;
      rightA = c;
      rightB = d;
    }
    

    void spin(directionType dir) {
      leftA.spin(dir);
      leftB.spin(dir);
      rightA.spin(dir);
      rightB.spin(dir);
    }

    void setVelocity(int32_t x, int32_t y, int32_t r) {
      // Set Translation
      leftA.speed += cube(y) + cube(x);
      rightA.speed += cube(y) + cube(x);
      leftB.speed += cube(y) - cube(x);
      rightB.speed += cube(y) - cube(x);

      // Set Rotation
      leftA.speed += cube(r);
      leftB.speed += cube(r);
      rightA.speed -= cube(r);
      rightB.speed -= cube(r);

      // Apply Transformation
      leftA.update();
      leftB.update();
      rightA.update();
      rightB.update();
    }

    double cube(int32_t val) {
      return val * val * val / 10000;
    }
};