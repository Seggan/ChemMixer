from game_funcs import *
from json import load
from tkinter import *
class GUI(Frame):
    def __init__(self, master, names):
        super().__init__(master)
        self.master=master
        self.init_widgets(names)
    def init_widgets(self, names):
        self.menubar=Menu(self.master)
        self.add=Menu(self.menubar)
        for name in names:
            self.add.add_checkbutton(label=name, command=lambda n=name:b.add(n))
        self.menubar.add_cascade(label="add", menu=self.add)
        root.config(menu=self.menubar)
with open("chemicals.json",'r') as r:
    chemdict=load(r)
b=CreativeBeaker()
root=Tk()
root.geometry("1000x700")
root.title("ChemMixer 4.0.0")
gui=GUI(root,[name for name in chemdict.keys()])
root.mainloop()
