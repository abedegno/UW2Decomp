/* target: ovr109 */
/* opts: -mm -1 -G -O -Y -d */

int far _input_addkey(int key, int arg, int mask, void (far *func)(int));
void far JoyStickCalibration_seg011_1B8(int arg);
void far Interupt4_COM1_ovr132_0(int arg);

void far ovr109_0(void)
{
    _input_addkey(0x16A, 0, 0xFF, JoyStickCalibration_seg011_1B8);
    _input_addkey(0x283, 0, 0xFF, Interupt4_COM1_ovr132_0);
}

void far ovr109_31(void)
{
}
