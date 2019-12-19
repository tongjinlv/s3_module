rmmod pwm.ko
insmod pwm.ko
echo 1 4 1 > /sys/class/misc/misc_pwm/pwm/func 2> /dev/null