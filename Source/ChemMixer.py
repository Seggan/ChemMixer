import os  # check os name; sound only works in windows
import time  # slight update delay
from multiprocessing import freeze_support  # support for exe-ing
from tkinter import *  # graphics module

# self-created file imports
import music  # music
from beaker import *  # beaker class
from chemical import *  # chemical class
from mods import *  # mod support (currently not implemented)


class MessageWin:
    """Class for showing popup messages"""

    def __init__(self, msg, title=''):
        self.win = Tk()
        self.win.title(title)

        # text
        self.text = Label(self.win, text=msg)
        self.text.pack(side=TOP)

        # "ok" button
        self.okbutton = Button(self.win, text="OK", command=lambda: self.win.destroy())
        self.okbutton.pack(side=TOP)


class Tutorial:
    """Tutorial window"""

    def __init__(self, master):
        """init stuff"""
        self.master = master
        self.messages = messages
        self.text_num = 0  # message index
        self.init_widgets()

    def init_widgets(self):
        """init widgets"""
        tutwin.title("Tutorial")

        # message
        self.text = Label(self.master, text=self.messages[self.text_num])
        self.text.pack(side=TOP)

        # "skip tutorial" button
        self.skip = Button(self.master, text="Skip Tutorial", command=lambda: tutwin.destroy())
        self.skip.pack(side=LEFT)

        # "next" button
        self.next = Button(self.master, text="Next >", command=lambda: self.next_win())
        self.next.pack(side=RIGHT)

    def next_win(self):
        self.text_num += 1

        if self.text_num == self.messages.index(self.messages[-1]):  # last message
            # don't need "skip tutorial" and "next" buttons
            self.skip.destroy()
            self.next.destroy()

            # add the "finish" button
            self.finish = Button(self.master, text="Finish", command=lambda: tutwin.destroy())
            self.finish.pack(side=TOP)

        self.text.config(text=self.messages[self.text_num])  # display next message


class Game:
    """The window for the game"""
    master: Tk  # so pycharm will recognise "master" class

    def __init__(self, master):
        """Initializes the main parts; GUI().init_widgets() initialises the widgets"""
        self.master = master

        self.mods_in_mods = list()
        self.mod_menu = list()

        beaker.temperature = 20
        self.desired_temp = 20
        self.reacts = beaker.reactions
        self.current_beaker = 1

        # start music if on windows
        if os.name != "posix":
            self.music_thread = music.MusicThread(2)
            self.music_thread.start()

        self.init_widgets()
        # self.bind_keys()

    def init_widgets(self):
        """Creates all the widgets for the window"""
        # put logo
        self.img = PhotoImage(master=root, file="Resources/Logo.gif")
        self.imglbl = Label(self.master, image=self.img)
        self.imglbl.pack(side=LEFT, expand=YES)
        self.imglbl2 = Label(self.master, image=self.img)
        self.imglbl2.pack(side=RIGHT, expand=YES)

        self.menubar = Menu(self.master)  # create menubar

        # "add" menu
        self.add = Menu(self.menubar)
        # get categories and make "elements" first and "uncategorized" last
        self.catdict = dict()
        self.categories = list(set([chemical.parent.capitalize() for chemical in chemlist]))
        self.categories.remove('Elements')
        self.categories.insert(0, 'Elements')
        self.categories.remove('Uncategorized')
        self.categories.append('Uncategorized')
        # make menus with categories and save in dictionary
        for category in self.categories:
            self.catdict[category] = Menu(self.add)
        # add chemicals in categories
        for category in self.catdict.keys():
            for chemical in chemlist:
                if chemical.parent == category.lower():
                    self.catdict[category].add_command(label=chemical.name,
                                                       command=lambda c=chemical.name: beaker.add(c))
            self.add.add_cascade(label=category, menu=self.catdict[category])

        self.menubar.add_cascade(label="Add", menu=self.add)

        # add the other menus
        self.text = beaker.show_contents(self.current_beaker)
        self.contents = Label(self.master, text=self.text)

        self.remove = Menu(self.menubar)
        self.menubar.add_cascade(label="Remove", menu=self.remove)

        self.beaker_settings = Menu(self.menubar)
        self.beaker_settings.add_command(label="Add Contents of the Other Beaker", command=lambda: self.mix_contents())
        self.beaker_settings.add_command(label="Switch Beakers",
                                         command=lambda: self.switch_beakers())
        self.beaker_settings.add_command(label="Empty the Beaker", command=lambda: beaker.reset())
        self.menubar.add_cascade(label="Beaker Settings", menu=self.beaker_settings)

        self.tools = Menu(self.menubar)
        self.tools.add_command(label="Evaporator", command=lambda: beaker.evaporate())
        self.tools.add_command(label="Remove Stopper", command=lambda: beaker.remove_stopper())
        self.tools.add_command(label="Filter", command=lambda: self.filter())
        self.tools.add_command(label="Electrodes", command=lambda: beaker.spark())
        self.tools.add_command(label="Heater/Cooler", command=lambda: self.create_temp_win())
        self.menubar.add_cascade(label="Tools", menu=self.tools)

        self.others = Menu(self.menubar)
        self.others.add_command(label="Credits", command=lambda: MessageWin('''
Idea based on: BEAKER, by THIX; CHEMIST, by THIX
Programmers: Daniel K., SuperLeaf 1995
Testers: Lianna K., Irina K., Daniel K., Drew Drew us, 
Alir001, cyanidesDuality, Droseraman, SuperLeaf 1995, Artem P.
Music: Petter Saterskog
''', "Credits"))
        self.others.add_command(label="Exit", command=lambda: root.destroy())
        self.menubar.add_cascade(label="Other Commands", menu=self.others)

        '''self.mods = Menu(self.menubar)
        with os.scandir(os.path.join(os.getcwd(), 'Mods')) as it:
            for entry in it:
                if '.json' in entry.name:
                    self.mods_in_mods.append(entry.name)
        self.mods_in_mods = [m.replace('.json', '') for m in self.mods_in_mods]
        for mod in self.mods_in_mods:
            self.mod_menu.append(Menu(self.mods))
        for mod in range(0, len(self.mod_menu)):
            self.mod_menu[mod].add_command(label="load", command=lambda m=self.mods_in_mods[mod]: self.mod_load(m))
            self.mod_menu[mod].add_command(label="unload",
                                           command=lambda m=self.mods_in_mods[mod]: self.mod_unload(m))
            self.mods.add_cascade(label=self.mods_in_mods[mod] + ' mod', menu=self.mod_menu[mod])
        self.menubar.add_cascade(label="Mods", menu=self.mods)'''

        # add "settings" menu if windows os
        if os.name != "posix":
            self.settings = Menu(self.menubar)
            self.settings.add_command(label="Music Settings", command=lambda: self.music_settings())
            self.menubar.add_cascade(label="Settings", menu=self.settings)

        root.config(menu=self.menubar)  # add menubar and menus to window

    """def bind_keys(self):
        def temp_up():
            beaker.temperature += 1

        def temp_down():
            beaker.temperature -= 1

        self.master.bind("<Up>", lambda args: temp_up())
        self.master.bind("<Down>", lambda args: temp_down())
        beaker.react()"""

    def update(self):
        """Updates (almost) everything"""
        time.sleep(0.005)
        if beaker.state_changed:
            beaker.react()

            # refresh "remove" menu
            self.cmds_in_remove = [ch.name for ch in beaker.contents]  # new remove menu contents
            self.remove.delete(0, 'end')  # clear menu
            # add new chemicals
            for chem in self.cmds_in_remove:
                self.remove.add_command(label=chem, command=lambda n=chem: beaker.extract(n))

            # repack images so they won't scroll down
            self.imglbl.pack(side=LEFT, expand=YES)
            self.imglbl2.pack(side=RIGHT, expand=YES)

        # increment or decrement game temperature depending on desired temperature
        if self.desired_temp < beaker.temperature:
            beaker.temperature -= 1
        elif self.desired_temp > beaker.temperature:
            beaker.temperature += 1

        for n in beaker.contents:
            n.change_states(beaker.temperature, bool(n.soluble and in_beaker("water", beaker.contents)))

        # display new contents
        self.text = beaker.show_contents(self.current_beaker)
        self.contents.config(text=self.text)
        self.contents.pack(side=TOP)

        self.master.update()  # refresh window

    def create_temp_win(self):
        """Creates the window for inputting the temperature"""
        self.tempinput = Tk()
        self.tempinput.title("Heater/Cooler")

        self.temptext = Label(self.tempinput, text='''Enter the wanted temperature:''')
        self.temptext.pack(side=TOP)

        self.tempentry = Entry(self.tempinput)
        self.tempentry.pack(side=TOP)

        self.okbutton = Button(self.tempinput, text="OK", command=lambda: self.change_temp())
        self.okbutton.pack(side=TOP)

    def change_temp(self):
        """Checks the user temperature input"""
        try:
            self.desired_temp = int(self.tempentry.get())  # get the inputted temperature as a number
        except ValueError:  # temperature input is not a number
            MessageWin('''Please enter a number.''', "Error")
        self.tempinput.destroy()
        beaker.react()
        # entered number smaller than absolute zero or bigger than max temp
        if self.desired_temp < -273 or self.desired_temp > 3500:
            MessageWin('''Please enter a number higher than
             -274 and lower than 3501.''', "Error")

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
        """Refreshes the application"""
        for ct in self.categories:
            self.add.delete(ct)
        self.catdict = dict()
        self.categories = list(set([chemical.parent.capitalize() for chemical in chemlist]))
        self.categories.remove('Elements')
        self.categories.insert(0, 'Elements')
        self.categories.remove('Uncategorized')
        self.categories.append('Uncategorized')
        for category in self.categories:
            self.catdict[category] = Menu(self.add)
        for category in self.catdict.keys():
            for chemical in chemlist:
                if chemical.parent == category.lower():
                    self.catdict[category].add_command(label=chemical.name,
                                                       command=lambda c=chemical.name: beaker.add(c))

    def music_settings(self):
        def song_1():
            self.music_thread.terminate()
            self.music_thread = music.MusicThread(1)
            self.music_thread.start()

        def song_2():
            self.music_thread.terminate()
            self.music_thread = music.MusicThread(2)
            self.music_thread.start()

        self.win = Tk()
        self.win.title("Music Settings")

        self.one = Button(self.win, text="Cell Lab: Challenge Song", command=lambda: song_1())
        self.one.pack()

        self.two = Button(self.win, text="Cell Lab: Experiment Song", command=lambda: song_2())
        self.two.pack()

        self.end = Button(self.win, text="Stop Music", command=lambda: self.music_thread.terminate())
        self.end.pack()

    def switch_beakers(self):
        switch_beakers(self.current_beaker)
        if self.current_beaker == 1:
            self.current_beaker = 2
        elif self.current_beaker == 2:
            self.current_beaker = 1

        # prevent beaker from setting to the other beaker's temperature
        self.desired_temp = beaker.temperature

    def mix_contents(self):
        if self.current_beaker == 1:
            for n in [n.name for n in beakers[1].contents]:
                beaker.add(n)
                beakers[1].reset()
        elif self.current_beaker == 2:
            for n in [n.name for n in beakers[0].contents]:
                beaker.add(n)
                beakers[0].reset()
        beaker.react()

    def filter(self):
        if self.current_beaker == 1:
            for n in range(0, len(beaker.contents)):
                for m in beaker.contents:
                    if m.state == "liquid":
                        beakers[1].add(m.name)
                        beaker.extract(m.name)
        elif self.current_beaker == 2:
            for n in range(0, len(beaker.contents)):
                for m in beaker.contents:
                    if m.state == "liquid":
                        beakers[0].add(m.name)
                        beaker.extract(m.name)
        beaker.react()


def switch_beakers(current_beaker):
    global beaker
    if current_beaker == 1:
        beakers[0] = beaker
        beaker = beakers[1]
    elif current_beaker == 2:
        beakers[1] = beaker
        beaker = beakers[0]


if __name__ == "__main__":
    freeze_support()

    chemlist = load_chemicals()

    DEBUG = True

    # The beaker
    beakers = [Beaker(chemlist), Beaker(chemlist)]
    beaker = beakers[0]

    beaker.add("oxygen")
    beakers[1].add("oxygen")
    for n in range(0, 3):
        beaker.add("nitrogen")
        beakers[1].add("nitrogen")

    tutwin = Tk()
    tutorial = Tutorial(tutwin)

    root = Tk()
    root.geometry("1000x700")
    root.title("ChemMixer 0.5.3")
    game = Game(root)

    # A try-except to catch an error when exiting
    while True:
        try:
            game.update()
        except Exception as e:
            if str(e) == 'invalid command name ".!label3"':
                sys.exit()
            else:
                if DEBUG:
                    raise
                else:
                    print(e.__name__, str(e))
                    while True:
                        pass
