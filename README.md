# Heterosix
openGL simulation Hexapod equipped with 5 dof robotic arm
A mechanical engineering senior project at NCKU

## 1. How to use Hexapod
### declare hexapods object
```
hexapods hexapod;
```
### get motor angle by motorID.
```
hexapod.motorAngle(int motorID);
```
parameter:
>> picture uploading

### get motor angle by legID and motor part ID
```
hexapod.motorAngle(int legID, int motor);
```
parameter:
>> picture uploading

## 2. Use five-axis robotic arm
>> pending

## 3. Manipulator command
ReadPS2(), ReadBluetooth() and ReadWifi() should return ``` _command ``` object.
```
_command{int commandID, double val1, double val2}
```
val1 for 1 dimension movement like adjust body height; or 2 dimension movement like x-value for setting move direction. 
val2 for 2nd demension mevement, like y-value for setting move direction.
Some command don't need further value, in this case just give it 0. 
commandID
>> see "commandList.h"
>> update frequently......
