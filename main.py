# -*- coding: utf-8 -*-

from kivy.app import App
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.togglebutton import ToggleButton
from kivy.uix.textinput import TextInput
from kivy.uix.boxlayout import BoxLayout
from kivy.clock import Clock
from kivy.graphics import Canvas, Line, Color, Rectangle
from kivy.animation import Animation
from kivy.properties import NumericProperty
import urllib2
import socket
import threading


class wlabel(Button):
    deg1=NumericProperty(0.0)
    deg_mn=NumericProperty(0.0)
    deg=NumericProperty(0.0)
    deg_mx=NumericProperty(0.0)
    def __init__(self,font_name):
        super(wlabel, self).__init__()

        self.list1 = []

        self.online = True;
        self.buffer = '';

        self.passinput = TextInput(multiline = False)
        self.passinput.bind(on_text_validate = self.on_enter)

        self.box = BoxLayout(orientation = 'vertical')
        self.box_pass = BoxLayout()
        self.box_pass.add_widget(self.passinput)
        self.box.add_widget(Label(text= "Data not fetched\nWait or connect to node-mcu and try again!!!"))
        
        self.pp   =    Popup(title='Connections Available',content=self.box,size_hint=(None, None),size=(400, 400))
        self.pp_pass = Popup(content=self.box_pass,size_hint=(None, None),size=(400,95))
        
        self.b1 = Button(size=(50,50),pos=(50,50))
        with self.b1.canvas:            
            self.b1.bg_rect = Rectangle(source="cover.png", pos=self.b1.pos, size=self.b1.size)
        self.b1.bind(on_press=self.pp.open,pos = self.redrawb1,size =  self.redrawb1)
        self.b1.background_normal = ''
        self.b1.background_color = (0.12941176,0.12941176,0.12941176,1)    
        self.add_widget(self.b1)
        self.lt = threading.Thread()

        self.toggle = ToggleButton(text = 'Online',state = 'down',pos = (150,50),size=(100,40))
        self.toggle.bind(state=self.togg_act)
        self.add_widget(self.toggle)
        
        
        self.ln = Line(circle = (0,0,0))   #arc
        
        self.ln1 = Line(circle = (0,0,0))
        self.ln_mn1 = Line(circle = (0,0,0))
        self.ln_mx1 = Line(circle = (0,0,0))
        
        self.ln2 = Line(circle = (0,0,0))    #refresh
        self.ln_mn2 = Line(circle = (0,0,0))
        self.ln_mx2 = Line(circle = (0,0,0))
        
        self.background_normal = ''
        self.background_color = (0.12941176,0.12941176,0.12941176,1)    
        self.halign = 'center'
        self.markup = True
        self.font_name = font_name        
        self.bind(deg1=self.callback,pos=self.redraw,size=self.redraw,deg_mn=self.redraw,deg=self.redraw,deg_mx=self.redraw)
    def togg_act(self,key,value):
        if value == 'down':            
            self.toggle.text = 'Online'
            self.online = True;
            self.canvas.add(self.ln2)
            self.anim(0.0)
        else:           
            self.toggle.text = 'Offline'
            self.online = False;
            self.anm.cancel(self)
            self.deg1 = 0.0
            self.canvas.remove(self.ln2)

    def passw(self,inst):        
        self.pp_pass.title = inst.text
        self.pp_pass.open()
    
    def on_enter(self,value):
        self.buffer = self.pp_pass.title + '$' + value.text        
        self.pp_pass.dismiss()
        self.pp.dismiss()
        
    def scan_make(self,lst): 
        self.box.clear_widgets()
        for bscan in lst:
            bscan.bind(on_press = self.passw)
            self.box.add_widget(bscan)            
       
        
    def scan_thread(self,dt):
        if not (self.online) and not (self.lt.isAlive()):
            self.lt = threading.Thread(target = self.scan)
            self.lt.start()

        
    
    def scan(self):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(8)
            sock.connect(('192.168.4.1', 6000))
            if(self.buffer!=''):
                sock.send(self.buffer)
                self.buffer = ''
                sock.close()
                return
                
            data = sock.recv(100)        
            k = data.split('$')        
            i = int(k[0])    
            j=1
            while(i!=0):
                while(k[j]!=''):
                    self.list1.append(Button(text = k[j]))
                    j=j+1
                    i = i-1                
                data = sock.recv(100)            
                k = data.split('$')
                j=0
            self.scan_make(self.list1)
            del self.list1[:]
            sock.close()
        except:
            self.box.clear_widgets()
            self.box.add_widget(Label(text = "You have disconnected from node-mcu\nConnect and wait for scan results"))
            return
        
        

    def draw(self,dt):
        self.canvas.add(Color(0.51,0.5294,0.5333))
        self.ln1=Line(width=2.5,circle = (self.center_x,self.center_y,150))
        self.ln_mn1=Line(width=2.5,circle = (self.center_x-250,self.center_y,75))
        self.ln_mx1=Line(width=2.5,circle = (self.center_x+250,self.center_y,75))
        self.canvas.add(self.ln1);
        self.canvas.add(self.ln);
        self.canvas.add(self.ln2);
        self.canvas.add(self.ln_mn1);
        self.canvas.add(self.ln_mn2);
        self.canvas.add(self.ln_mx1);
        self.canvas.add(self.ln_mx2);
        self.text = '[color=828788][size=20]Min                                          Real                                          Max\n\n[/size][/color]'
    def redraw(self,key,value):
        self.canvas.remove(self.ln1);  
        self.canvas.remove(self.ln);
        self.canvas.remove(self.ln_mn1);
        self.canvas.remove(self.ln_mn2);
        self.canvas.remove(self.ln_mx1);
        self.canvas.remove(self.ln_mx2);
            
        self.canvas.add(Color(0.51,0.5294,0.5333))
            
        self.ln1=Line(width=2.5,circle=(self.center_x,self.center_y,150))
        self.ln_mn1=Line(width=2.5,circle = (self.center_x-250,self.center_y,75))
        self.ln_mx1=Line(width=2.5,circle = (self.center_x+250,self.center_y,75))
            
        self.canvas.add(self.ln1);
        self.canvas.add(self.ln_mn1);
        self.canvas.add(self.ln_mx1);
            
        self.canvas.add(Color(1.,1.,0))
            
        self.ln = Line(width=2.5,circle=(self.center_x,self.center_y,150,0,self.deg))
        self.ln_mn2 = Line(width=2.5,circle=(self.center_x-250,self.center_y,75,0,self.deg_mn))
        self.ln_mx2 = Line(width=2.5,circle=(self.center_x+250,self.center_y,75,0,self.deg_mx))
            
        self.canvas.add(self.ln);
        self.canvas.add(self.ln_mn2);
        self.canvas.add(self.ln_mx2);    
    def redrawb1(self,key,value):
        self.bg_rect.size = self.size
        self.bg_rect.pos = self.pos    
    def anim(self,dt):        
        self.anm = Animation(deg1=-365.0,d = 4., t='in_quad')
        self.canvas.add(Color(0,1.,0))  
        self.anm.start(self)
        self.anm.bind(on_complete=self.update)
    def callback(self,key,value):        
        self.canvas.remove(self.ln2);
        self.ln2 = Line(width=1.1,circle = (self.center_x,self.center_y,125,0,value))
        self.canvas.add(self.ln2);        
    def update(self,animation,widget):        
        if self.online:
            try:         
                self.data = urllib2.urlopen("http://Satyamban.pythonanywhere.com/10200314038_XYZ_ESP-TEMP",timeout = 5).read()
                self.datarray = self.data.split('$$')
                self.mint =  float(self.datarray[0]);
                self.tem = float(self.datarray[1]);
                self.maxt = float(self.datarray[2]);
            
                self.deg_mn = (self.mint/100)*(-360)
                self.deg = (self.tem/100)*(-360)
                self.deg_mx = (self.maxt/100)*(-360)            
            
            except:                
                self.online = False
                self.toggle.state = 'normal'
                return
            if(self.data!=''):
                self.text = "[color=828788][size=20]%s[/size][/color]\n[size=40]%s[/size]" % ("Min                                          Real                                          Max",self.datarray[0]+"°C             "+self.datarray[1]+"°C             "+self.datarray[2]+"°C")
            self.deg1=0.0
            self.anim(0.0)
        

class TestApp(App):        
    def build(self):
        wl = wlabel(font_name = 'Roboto-BoldItalic')
        wl.draw(0.0)        
        Clock.schedule_once(wl.anim, 1)
        Clock.schedule_once(wl.scan_thread, 0)
        Clock.schedule_interval(wl.scan_thread, 10)        
        return wl

if __name__ == '__main__':    
    TestApp().run()

