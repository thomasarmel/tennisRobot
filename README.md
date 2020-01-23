# tennisRobot
A robot which fetch tennis balls, computed with C++ in a Raspberry Pi


g++ -o progRobot main.cpp detecttennis.cpp dcmotor.cpp US-I2C_servo.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lwiringPi

modules:

I2c-dev

bcm2835-v4l2
