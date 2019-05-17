from game_funcs import *
from json import load
import sys
from tkinter import *
class GUI(Frame):
    def __init__(self, master, names, temp, chem):
        super().__init__(master)
        self.master=master
        self.init_widgets(names, temp, chem)
    def init_widgets(self, names, temp, chem):
        self.menubar=Menu(self.master)
        self.add=Menu(self.menubar)
        for name in names:
            self.add.add_command(label=name, command=lambda n=name:beaker.add(n))
        self.menubar.add_cascade(label="Add", menu=self.add)
        root.config(menu=self.menubar)
        self.text=beaker.show_contents(temp, chem)
        self.contents=Label(self.master, text=self.text)
        self.contents.pack(side=TOP)
    def update_contents(self, temp, chem):
        self.text=beaker.show_contents(temp, chem)
        self.contents.config(text=self.text)
        self.contents.pack(side=TOP)
with open("chemicals.json",'r') as r:
    chemdict=load(r)
temperature=20
beaker=CreativeBeaker()
root=Tk()
root.geometry("1000x700")
root.title("ChemMixer 4.0.0")
gui=GUI(root,[name for name in chemdict.keys()], temperature, chemdict)
# A try-except for stopping the
while True:
    try:
        gui.update_contents(temperature, chemdict)
        root.update()
    except:
        sys.exit()
