/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Vision20             vision        20              
// touchSensor          bumper        A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "GUI.cpp"

competition Competition;

controller control = controller();
// Declare and Initialize Motors
MotorController indexer = MotorController("Indexer", PORT6, ratio6_1, false);
MotorController intakeLeft = MotorController("Intake-Left", PORT4, ratio18_1, false);
MotorController intakeRight = MotorController("Intake-Right", PORT5, ratio18_1, false);
MotorController flywheel = MotorController("Flywheel", PORT7, ratio6_1, true, 2);
MotorController leftFront = MotorController("Front-Left", PORT1, ratio18_1, false);
MotorController leftBack = MotorController("Back-Left", PORT2, ratio18_1, false);
MotorController rightBack = MotorController("Back-Right", PORT9, ratio18_1, true);
MotorController rightFront = MotorController("Front-Right", PORT10, ratio18_1, true);
XDrive xDrive = XDrive(leftFront, leftBack, rightBack, rightFront);

GUI gui = GUI(Brain.Screen, vector<MotorController> {leftFront, rightFront, intakeLeft, intakeRight, leftBack, rightBack, indexer, flywheel});
AutonController autonSelect = AutonController(xDrive, indexer, flywheel, intakeLeft, intakeRight);

BallColour loaded;

void pre_auton(void) {
  vexcodeInit();
  gui.draw();
}

void autonomous(void) {
  switch(gui.autonChoice) {
    case Auton::BLUELEFT:
      autonSelect.blueLeft();
      break;
    case Auton::BLUERIGHT:
      autonSelect.blueRight();
      break;
    case Auton::REDLEFT:
      autonSelect.redLeft();
      break;
    case Auton::REDRIGHT:
      autonSelect.redRight();
      break;
  }
}

void usercontrol(void) {
  // Initiate Motors and GUI
  xDrive.spin(directionType::fwd);
  indexer.spin(directionType::fwd);
  intakeLeft.spin(directionType::fwd);
  intakeRight.spin(directionType::fwd);
  flywheel.spin(directionType::fwd);
  flywheel.setVelocity(0, velocityUnits::pct);
  gui.draw();

  bool flywheelSpinning = false, holding = false;

  while (1) {
    
    // XDrive Control 
    xDrive.setVelocity(control.Axis4.position(), control.Axis3.position(), control.Axis1.position());

    // Index Control
    if (control.ButtonR1.pressing()) {
      indexer.setVelocity(-100, velocityUnits::pct);
    } else if (control.ButtonR2.pressing()) {
      indexer.setVelocity(100, velocityUnits::pct);
    } else {
      indexer.setVelocity(0, velocityUnits::pct);
    }
    
    // Intake Control
    if (control.ButtonL1.pressing()) {
      intakeLeft.setVelocity(-100, velocityUnits::pct);
      intakeRight.setVelocity(100, velocityUnits::pct);
    } else if (control.ButtonL2.pressing()) {
      intakeLeft.setVelocity(100, velocityUnits::pct);
      intakeRight.setVelocity(-100, velocityUnits::pct);
    } else {
      intakeLeft.setVelocity(0, velocityUnits::pct);
      intakeRight.setVelocity(0, velocityUnits::pct);
    }

    // Flywheel Control
    if (control.ButtonB.pressing()) {
      if (!holding) {
        flywheelSpinning = !flywheelSpinning;
      }
      if (flywheelSpinning ) {
        flywheel.setVelocity(100, velocityUnits::pct);
      } else {
        flywheel.setVelocity(0, velocityUnits::pct);
      }
      holding = true;
    } else {
      holding = false;
    }

    // Update GUI
    gui.update();

    // Get Ball Colour
    loaded = gui.getColour();

    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    // Update GUI
    gui.update();
    wait(100, msec);
  }
};
