#include "XDrive.cpp"

class AutonController {
  public:
    XDrive xDrive;
    MotorController index;
    MotorController fly;
    MotorController leftIntake;
    MotorController rightIntake;

    AutonController(XDrive drive, MotorController indexer, MotorController flywheel, MotorController left_intake, MotorController right_intake) {
      xDrive = drive;
      index = indexer;
      fly = flywheel;
      leftIntake = left_intake;
      rightIntake = right_intake;
    }

    void blueLeft() {
      
    }
    
    void blueRight() {
      
    }
    
    void redLeft() {
      
    }
    
    void redRight() {
      
    }
};