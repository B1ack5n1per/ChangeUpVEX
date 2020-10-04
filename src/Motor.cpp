#include "MotorData.cpp"

// Motor Sub-Class for accesessing formated data
class MotorController: public motor {
  public:
    MotorController(): motor(PORT1) {
      
    }
    MotorController(string title, int32_t port, gearSetting gears, bool reversed): motor(port, gears, reversed) {
      name = title;
      rpmMultiplier = 1;
    }

    MotorController(string title, int32_t port, gearSetting gears, bool reversed, double rpmStretch): motor(port, gears, reversed) {
      name = title;
      rpmMultiplier = rpmStretch;
    }


    MotorData getData() {
      return MotorData(name, temperature(temperatureUnits::celsius), rotation(rotationUnits::deg), velocity(velocityUnits::rpm) * rpmMultiplier);
    }

    double speed;

    void update() {
      motor::setVelocity(speed, velocityUnits::pct);
      speed = 0;
    }

  private:
    string name;
    double rpmMultiplier;
};
