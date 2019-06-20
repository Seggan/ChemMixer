from game_funcs import *
from tkinter import *
from json import load, dump
import os
import sys


def cap_first_letter(word):
    word2 = str()
    try:
        word2 = word[0].upper()
    except:
        pass
    for char in word[1:]:
        word2 = word2 + char
    return word2


class Error:
    def __init__(self, txt):
        self.error = Tk()
        self.error.title('')

        self.errortext = Label(self.error, text=txt)
        self.errortext.pack(side=TOP)

        self.okbutton = Button(self.error, text="OK", command=lambda: self.error.destroy())
        self.okbutton.pack(side=TOP)


class GUI:
    """The window for the game"""

    def __init__(self, master, temp, chem):
        """Initializes the main parts; GUI().init_widgets() initialises the widgets"""
        self.master = master
        self.mods_in_mods = list()
        self.mod_menu = list()
        self.temp = 20
        self.chems = chem
        self.reacts = beaker.reactions
        self.init_widgets(temp, chem)

    def init_widgets(self, temp, chem):
        """Creates all the widgets for the window"""
        self.img = PhotoImage(file="Logo.png")
        self.imglbl = Label(self.master, image=self.img)
        self.imglbl.pack(side=LEFT)

        self.imglbl2 = Label(self.master, image=self.img)
        self.imglbl2.pack(side=RIGHT)

        self.menubar = Menu(self.master)

        self.add = Menu(self.menubar)
        self.catdict = dict()
        self.categories = list(set([cap_first_letter(chem[chemical]["parent"]) for chemical in chem.keys()]))
        self.categories.remove('Elements')
        self.categories.insert(0, 'Elements')
        self.categories.remove('Uncategorized')
        self.categories.append('Uncategorized')
        for category in self.categories:
            self.catdict[category] = Menu(self.add)
        for category in self.catdict.keys():
            for chemical in chem.keys():
                if chem[chemical]["parent"] == category.lower():
                    self.catdict[category].add_command(label=chemical, command=lambda c=chemical: beaker.add(c))
            self.add.add_cascade(label=category, menu=self.catdict[category])

        self.menubar.add_cascade(label="Add", menu=self.add)

        self.text = beaker.show_contents(temp, chem)
        self.contents = Label(self.master, text=self.text)
        self.contents.pack(side=TOP)

        self.remove = Menu(self.menubar)
        self.menubar.add_cascade(label="Remove", menu=self.remove)

        self.others = Menu(self.menubar)
        self.others.add_command(label="Change Temperature", command=lambda: self.create_temp_win())
        self.others.add_command(label="Empty the Beaker", command=lambda: beaker.reset())
        # self.others.add_command(label="Credits", command=lambda:self.credits())
        self.others.add_command(label="Exit", command=lambda: root.destroy())
        self.menubar.add_cascade(label="Other Commands", menu=self.others)

        self.mods = Menu(self.menubar)
        with os.scandir(os.path.join(os.getcwd(), 'Mods')) as it:
            for entry in it:
                if '.json' in entry.name:
                    self.mods_in_mods.append(entry.name)
        self.mods_in_mods = [m.replace('.json', '') for m in self.mods_in_mods]
        for mod in self.mods_in_mods:
            self.mod_menu.append(Menu(self.mods))
        for mod in range(0, len(self.mod_menu)):
            self.mod_menu[mod].add_command(label="load", command=lambda m=self.mods_in_mods[mod]: self.mod_load(m))
            self.mod_menu[mod].add_command(label="unload", command=lambda m=self.mods_in_mods[mod]: self.mod_unload(m))
            self.mods.add_cascade(label=self.mods_in_mods[mod] + ' mod', menu=self.mod_menu[mod])
        self.menubar.add_cascade(label="Mods", menu=self.mods)

        root.config(menu=self.menubar)

    def update(self, temp, chem):
        """Updates (almost) everything"""
        beaker.react(temperature, chemdict)
        change_states(temp, chem, beaker.contents)
        self.text = beaker.show_contents(temp, chem)
        self.contents.config(text=self.text)
        self.contents.pack(side=TOP)
        self.remove.delete(0, 'end')
        self.cmds_in_remove = beaker.contents
        for chem in self.cmds_in_remove:
            self.remove.add_command(label=chem, command=lambda n=chem: beaker.extract(n))
        """self.imglbl.pack(side=LEFT)
        self.imglbl2.pack(side=RIGHT)"""

    def credits(self):
        """Creates the credits window"""
        self.creds = Tk()
        self.creds.title("Credits")
        self.credit_text = Label(self.creds, text='''
Idea based on: BEAKER, by THIX; CHEMIST, by THIX
Programmers: Daniel K., Super Leaf 1995
Testers: Lianna K., Irina K., Daniel K., Drew Drew us, Alir001, cyanidesDuality, Droseraman, Super Leaf 1995
''')
        self.credit_text.pack()

    def create_temp_win(self):
        """Creates the window for inputting the temperature"""
        self.tempinput = Tk()
        self.tempinput.title("Change the Temperature")

        self.temptext = Label(self.tempinput, text="Enter the wanted temperature:")
        self.temptext.pack(side=TOP)

        self.tempentry = Entry(self.tempinput)
        self.tempentry.pack(side=TOP)

        self.okbutton = Button(self.tempinput, text="OK", command=lambda: self.change_temp())
        self.okbutton.pack(side=TOP)

    def change_temp(self):
        """Checks the user temperature input"""
        try:
            self.temp = int(self.tempentry.get())
        except ValueError:
            Error('''Please enter a number.''')
        self.tempinput.destroy()
        beaker.react(temperature, chemdict)
        if self.temp < -273 or self.temp > 3500:
            self.temp = temperature
            Error('''Please enter a number higher than
             -274 and lower than 3501.''')

    def mod_load(self, mod):
        """Really just a combo of functions that loads a mod; does not load them"""
        mod_load(mod)
        self.refresh()
        beaker.refresh()

    def mod_unload(self, mod):
        """Really just a combo of functions that unloads a mod; does not unload them"""
        mod_unload(mod)
        self.refresh()
        beaker.refresh()

    def refresh(self):
        """Refreshes the "add" menu"""
        self.add = Menu(self.menubar)
        self.catdict = dict()
        self.categories = list(set([cap_first_letter(chemdict[chemical]["parent"]) for chemical in chemdict.keys()]))
        self.categories.remove('Elements')
        self.categories.insert(0, 'Elements')
        self.categories.remove('Uncategorized')
        self.categories.append('Uncategorized')
        for category in self.categories:
            self.catdict[category] = Menu(self.add)
        for category in self.catdict.keys():
            for chemical in chemdict.keys():
                if chemdict[chemical]["parent"] == category.lower():
                    self.catdict[category].add_command(label=chemical, command=lambda c=chemical: beaker.add(c))
            self.add.add_cascade(label=category, menu=self.catdict[category])

        self.menubar.add_cascade(label="Add", menu=self.add)


with open("chemicals.json", 'r') as r:
    chemdict = load(r)
temperature = 20
# The beaker
beaker = Beaker(chemdict)
root = Tk()
root.geometry("1000x700")
root.title("ChemMixer 0.4.2")
gui = GUI(root, temperature, chemdict)
# A try-except to catch an error when exiting
while True:
    try:
        gui.update(temperature, chemdict)
        temperature = gui.temp
        root.update()
    except Exception as e:
        if str(e) == 'invalid command name ".!label3"':
            sys.exit()
        else:
            root.destroy()
            print("Error:", e)
            while True:
                pass
