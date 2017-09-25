'''
    L295N
        PWM controlled: duty cycle depents on remote
        Frequency fixed at 400Hz - 800Hz
        
        
        
        https://sourceforge.net/p/raspberry-gpio-python/wiki/PWM/
    To create a PWM instance:
        p = GPIO.PWM(channel, frequency)
    To start PWM:
        p.start(dc)   # where dc is the duty cycle (0.0 <= dc <= 100.0)
    To change the frequency:
        p.ChangeFrequency(freq)   # where freq is the new frequency in Hz
    To change the duty cycle:
        p.ChangeDutyCycle(dc)  # where 0.0 <= dc <= 100.0
    To stop PWM:
        p.stop()



'''