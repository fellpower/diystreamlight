## DIY STREAMLIGHT
# PWM controlled DIY Streamlight
![DIY Streamlight](https://github.com/fellpower/diystreamlight/blob/main/licht.jpg)


# Infos

Als ein Creator und Youtubestar braucht man natürlich ein Licht, wenn man sich vor der Kamera zeigen will. Da mir diese ganzen Kaufoptionen viel zu teuer waren, habe ich mir selbst ein Licht gebaut.
Wenn man jedoch stundenlang vor dem PC sitzt, kann einem diese hohe Helligkeit durchaus mal einen Kopfschmerz verpassen, weshalb ich mich entschlossen habe, das Licht dimmbar zu machen.
Genutzt werden hier ein Wemos D1 Mini, ein IRLZ44N MOSFET Transistor und ein bisschen Hühnerfutter, wie zb ein DC to DC Bock Konverter, um die benötigten 3,3V aus der 12V Versorgungsspannung zu erzeugen.
Auf dem Wemos läuft ein HTTP Server, welcher einem ermöglicht, die Helligkeit und die PWM Frequenz einzustellen. Weiterhin ist so eine Einbindung in das Streamdeck möglich.


![DIY Streamlight](https://github.com/fellpower/diystreamlight/blob/main/streamdeck.png)

# Schaltplan

Der Schaltplan ist demnach sehr einfach.

![DIY Streamlight](https://github.com/fellpower/diystreamlight/blob/main/schaltbild_pwm_diy_streamlicht.png)

# Buck Converter

Dieser hat leider einen Baufehler, weshalb ihr, wenn ihr einen Lötpunkt bei 3,3V setzt, nicht die vollen 3,3V bekommt. Um das zu fixen, einfach eine Leiterbahn durchtrennen und den Lötpunkt bei 3,3V setzen.

![DIY Streamlight](https://github.com/fellpower/diystreamlight/blob/main/buck.png)





