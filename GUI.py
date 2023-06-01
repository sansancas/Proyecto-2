import builtins 
import tkinter as tk
from tkinter import *
import serial, time 
from time import sleep
from Adafruit_IO import Client, RequestError, Feed 

ser = 0
root = Tk()
root.counter = 0

'''-----------------------Configuración de la comunicación serial-----------------------'''
#puerto_comunicación = serial.Serial('COM12', baudrate=9600, timeout= 10, parity= 'N', stopbits= serial.STOPBITS_ONE, bytesize= serial.EIGHTBITS)

'''-----------------------------Configuracion de adafruit------------------------------------'''

ADAFRUIT_IO_USERNAME = "sansancas"
ADAFRUIT_IO_KEY = "aio_hrZB39GkTVX8mAzPcfHZKc4tMNGZ"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)


'''-------------Comunicacion de GUI a ADAFRUIT----------------------------------'''

digital_feed = aio.feeds ('texto')

aio.send_data(digital_feed.key, root.counter)
digital_data = aio.receive(digital_feed.key)



adafruit_suma = aio.receive('texto').value



def estirar():
    root.counter = 1
    L['text'] = root.counter
    digital_feed = aio.feeds('texto')
    aio.send_data(digital_feed.key, root.counter)


def flexionar():
    root.counter = 2
    L['text'] = root.counter
    digital_feed = aio.feeds('texto')           #Proceso para actualizar los valores que obtenemos de la interfaz 
    aio.send_data(digital_feed.key, root.counter)

def guardar():
    root.counter = 3
    L['text'] = root.counter
    digital_feed = aio.feeds('texto')
    aio.send_data(digital_feed.key, root.counter)   
     
def mostrar():
    root.counter = 4
    L['text'] = root.counter
    digital_feed = aio.feeds('texto')
    aio.send_data(digital_feed.key, root.counter)
    
def activar():
    root.counter = 5
    L['text'] = root.counter
    digital_feed = aio.feeds('texto')
    aio.send_data(digital_feed.key, root.counter)    
    
def actualizar():
    adafruit_suma = aio.receive('texto').value
    L['text'] = adafruit_suma
    digital_feed = aio.feeds ('texto')
    ser.write(int(adafruit_suma))

def open():
	global ser
	ser = serial.Serial('COM12', baudrate=9600, timeout= 10, parity= 'N', stopbits= serial.STOPBITS_ONE, bytesize= serial.EIGHTBITS)
	print(ser.name)        
	
def close():
	global ser
	print(ser.name)         #
	ser.close()
 
    

b1 = Button(root, text = "Estirar", command = estirar)
b1.place(x=150, y=80)

b2 = Button(root, text = "Flexionar", command = flexionar)
b2.place(x=150, y=245)

b3 = Button(root, text = 'Actualizar', command = actualizar)
b3.place(x=350, y=245)

b4 = Button(root, text = "Guardar", command = guardar)
b4.place(x=150, y=150)

b5 = Button(root, text = "Mostrar", command = mostrar)
b5.place(x=150, y=10)

b6 = Button(root, text = 'Activar/Desactivar', command = activar)
b6.place(x=150, y=300)

b7 = Button(root, text = "Open", command = open)
b7.place(x=150, y=350)

b8 = Button(root, text = 'Close', command = close)
b8.place(x=300, y=375)

txt = adafruit_suma
L = Label(root, text = txt)
L.pack()

while True:

    root.update_idletasks()
    root.update()
    
