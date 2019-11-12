from chemical import *

messages = [
    '''Welcome to the ChemMixer tutorial.  
    Press \"Next >\" to get started.''',

    '''Adding Chemicals:
To add chemicals go to the "Add" menu, then go 
to the desired family of chemicals, then finally 
choose the chemical you want.''',

    '''Removing Chemicals:
To remove a chemical go to the "Remove" menu and 
then select the chemical you want to remove.''',

    '''Changing the Temperature:
To change the temperature go to the "Tools" menu and 
select the "Heater" option, then input 
the desired temperature in degrees Celsius.''',

    '''Sparking:
To spark, to to the "Tools" menu and 
select "Electrodes". This is the only way to oxidize 
stuff. Sparking does not work in temperatures > 
10\u00b0 Celsius.''',

    '''Switching Beakers:
To switch beakers, to to the "Beaker Settings" menu and 
select "Switch Beakers".''',

    '''Transferring Beaker Contents:
To transfer beaker contents, to to the "Beaker Settings" 
menu and select "Add Contents of the Other Beaker". This 
will add the contents of the other beaker to the current 
beaker.''',

    '''Other Tools:
Filter: Transfers the liquid contents of the beaker to the 
second beaker.

Evaporator: Evaporates all the water in the current beaker.

Stopper: Opening the stopper releases all the gases from 
the current beaker.''',

    '''Sample Reactions:
Reactant(s): 2 moles of hydrogen, 1 mole of oxygen
Requirements: A spark
Product(s): 2 moles of water

Reactant(s): 1 mole of hydrogen peroxide
Requirements: Temperatures above 150\u00b0 Celsius
Products: 1 mole of water, 1 mole of oxygen'''
]


def amount(obj, array):
    num = 0
    for entry in array:
        if entry == obj:
            num += 1
    return num


def sublist(child, parent):
    parent_dict = {entry: amount(entry, parent) for entry in parent}
    child_dict = {entry: amount(entry, child) for entry in child}
    for entry in child_dict.keys():
        if entry in parent_dict.keys():
            if child_dict[entry] > parent_dict[entry]:
                return False
        else:
            return False
    return True


def in_beaker(chemical_name, contents):
    for chemical in contents:
        if chemical.name == chemical_name:
            return True
    return False


class Beaker:
    """A class to represent a beaker"""

    def __init__(self, chemlist):
        from json import load
        self.temperature = 20
        self.chemlist = chemlist
        self.dc = [chem for chem in chemlist if chem.element]
        self.contents = list()
        self.previous_contents = self.contents
        with open("Resources/reactions.json", 'r') as r:
            reacts = load(r)
            self.reactions = reacts["reactions"]
            self.sparks = reacts["sparks"]
        self.cont = False
        self.state_changed = False

    def add(self, chemical):
        for chem in self.chemlist:
            if chem.name == chemical:
                self.contents.append(chem)
                self.state_changed = True
                break

    def extract(self, chemical):
        if in_beaker(chemical, self.contents):
            for chem in self.chemlist:
                if chem.name == chemical:
                    self.contents.remove(chem)
                    self.state_changed = True
                    break

    def reset(self):
        """Empties the beaker"""
        self.contents = list()

    def react(self):
        """Checks for reactions and if there are any, reacts them"""
        self.state_changed = False
        for a in self.reactions.keys():
            if sublist(self.reactions[a]["reactants"], [n.name for n in self.contents]) and (
                    self.reactions[a]["temp min"] < self.temperature <= self.reactions[a]["temp max"]):
                for c in self.reactions[a]["reactants"]:
                    self.extract(c)
                for d in self.reactions[a]["results"]:
                    self.add(d)
        for chemical in self.contents:
            chemical.change_states(self.temperature, bool(chemical.soluble and in_beaker("water", self.contents)))

    def spark(self):
        self.state_changed = True
        for a in self.sparks.keys():
            if sublist(self.sparks[a]["reactants"], [n.name for n in self.contents]) and self.temperature > 9:
                for c in self.sparks[a]["reactants"]:
                    self.extract(c)
                for d in self.sparks[a]["results"]:
                    self.add(d)

    def show_contents(self, current_beaker):
        """Shows the contents of the beaker"""
        accounted = list()
        solution = False
        txt = f'''Beaker {current_beaker} is at {str(self.temperature)} degrees Celsius and has the
following chemicals inside:'''
        for chemical in self.contents:
            if chemical.state == "aqueous":
                solution = True
        for chemical in self.contents:
            if chemical not in accounted:
                count = amount(chemical, self.contents)
                if count == 1:
                    mole = ' mole of '
                else:
                    mole = ' moles of '
                if chemical.name == "water" and not solution:
                    txt += f"\n {str(count)}{mole}{chemical.state} {chemical.name}"
                if chemical.name != "water":
                    txt += f"\n {str(count)}{mole}{chemical.state} {chemical.name}"
                accounted.append(chemical)
        return txt

    def refresh(self):
        from json import load
        with open("Resources/reactions.json", 'r') as r:
            reacts = load(r)
            self.reactions = reacts["reactions"]
            self.sparks = reacts["sparks"]

    def evaporate(self):
        if in_beaker("water", self.contents):
            for n in range(0, [n.name for n in self.contents].count("water")):
                self.extract("water")

    def remove_stopper(self):
        for n in range(0, len(self.contents)):
            for m in self.contents:
                if m.state == "gaseous":
                    self.extract(m.name)
