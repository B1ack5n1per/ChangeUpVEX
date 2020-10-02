#include "MotorData.cpp"

// Motor Sub-Class for accesessing formated data
class MotorController: public motor {
  public:
    MotorController(): motor(PORT1) {
      
    }
    MotorController(string title, int32_t port, gearSetting gears, bool reversed): motor(port, gears, reversed) {
      name = title;
    }


    MotorData getData() {
      return MotorData(name, temperature(temperatureUnits::celsius), rotation(rotationUnits::deg), velocity(velocityUnits::rpm));
    }

    double speed;

    void update() {
      motor::setVelocity(speed, velocityUnits::pct);
      speed = 0;
    }

  private:
    string name;
};
