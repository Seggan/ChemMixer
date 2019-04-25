from game_funcs import *
from time import sleep
from json import load
from sys import exit
add_dict=dict()
remove_dict=dict()
creative_beaker=CreativeBeaker()
discovery_beaker=DiscoveryBeaker()
with open("chemicals.json",'r') as c:
    chemdict=load(c)
temperature=19
print('''ChemMixer 2.2.0

What's new:
fixed some bugs,
added discovery mode,
added aluminum derivatives,
added more reactions
That's all.\n''')
print('''Welcome to ChemMixer, a chemistry lab simulator.
Let's get started.''')
mode=codify(input('''\nWhich mode?
1)Creative Mode
2)Discovery Mode
'''))
if mode=="1":
    x=1
    for n in chemdict:
        add_dict[x]=n
        x+=1
    while True:
        creative_beaker.cont=False
        cmd=codify(input('''\n1)Add a chemical
2)Remove a chemical
3)Change the temperature
4)Exit
5)Empty the beaker
\nType in the number of the command you want to execute: '''))
        if cmd=="1":
            msg="\nWhat chemical should I add?"
            for n in add_dict:
                msg+="\n"+str(n)+")"+add_dict[n]
            cmd2=codify(input(msg+"\n"))
            chem=add_dict[int(cmd2)]
            creative_beaker.add(chem,chemdict)
        elif cmd=="2":
            msg="\nWhat chemical should I remove?"
            x=1
            remove_dict=dict()
            for n in creative_beaker.chemicals:
                remove_dict[x]=n
                x+=1
            for n in remove_dict:
                msg+="\n"+str(n)+")"+remove_dict[n]
            cmd2=codify(input(msg+"\n"))
            chem=remove_dict[int(cmd2)]
            creative_beaker.extract(chem)
        elif cmd=="3":
            cmd2=codify(input("\nWhat temperature should I set the beaker to? "))
            try:
                if int(cmd2)<-273:
                    print("\nPlease enter a number larger than -274.")
                    continue
                if int(cmd2)>3500:
                    print("\nThe beaker melted! Starting with a new one.")
                    creative_beaker.reset()
                    continue
                temperature=int(cmd2.rstrip())
                change_states(temperature,chemdict)
            except TypeError:
                print("\nPlease enter a number.")
        elif cmd=="4":
            print("\nCredits:")
            sleep(1)
            print("Idea based on: BEAKER, by THIX; CHEMIST, by THIX")
            sleep(1)
            print("Programmer: Daniel K.")
            sleep(1)
            print("Testers: Lianna K., Irina K., Daniel K., Drew Drew us")
            sleep(1)
            exit()
        elif cmd=="5":
            creative_beaker.reset()
            print("The beaker is now empty.")
        else:
            print("Sorry, but",cmd,"is not a valid command. Please try again.")
        if not creative_beaker.cont:
            for n in range(0,100):
                creative_beaker.react(temperature,chemdict)
            creative_beaker.contents(temperature,chemdict)
else:
    while True:
        x=1
        for n in discovery_beaker.discovered_chemicals:
            add_dict[x]=n
            x+=1
        discovery_beaker.cont=False
        cmd=codify(input('''\n1)Add a chemical
2)Remove a chemical
3)Change the temperature
4)Exit
5)Empty the beaker
\nType in the number of the command you want to execute: '''))
        if cmd=="1":
            msg="\nWhat chemical should I add?"
            for n in add_dict:
                msg+="\n"+str(n)+")"+add_dict[n]
            cmd2=codify(input(msg+"\n"))
            chem=add_dict[int(cmd2)]
            discovery_beaker.add(chem,chemdict)
        elif cmd=="2":
            msg="\nWhat chemical should I remove?"
            x=1
            remove_dict=dict()
            for n in discovery_beaker.chemicals:
                remove_dict[x]=n
                x+=1
            for n in remove_dict:
                msg+="\n"+str(n)+")"+remove_dict[n]
            cmd2=codify(input(msg+"\n"))
            chem=remove_dict[int(cmd2)]
            discovery_beaker.extract(chem)
        elif cmd=="3":
            cmd2=codify(input("\nWhat temperature should I set the beaker to? "))
            try:
                if int(cmd2)<-273:
                    print("\nPlease enter a number larger than -274.")
                    continue
                if int(cmd2)>3500:
                    print("\nThe beaker melted! Starting with a new one.")
                    discovery_beaker.reset()
                    continue
                temperature=int(cmd2.rstrip())
                change_states(temperature,chemdict)
            except TypeError:
                print("\nPlease enter a number.")
        elif cmd=="4":
            print("\nCredits:")
            sleep(1)
            print("Idea based on: BEAKER, by THIX; CHEMIST, by THIX")
            sleep(1)
            print("Programmer: Daniel K.")
            sleep(1)
            print("Testers: Lianna K., Irina K., Daniel K., Drew Drew us")
            sleep(1)
            exit()
        elif cmd=="5":
            discovery_beaker.reset()
            print("The beaker is now empty.")
        else:
            print("Sorry, but",cmd,"is not a valid command. Please try again.")
        if not discovery_beaker.cont:
            for n in range(0,100):
                discovery_beaker.react(temperature,chemdict)
            for n in discovery_beaker.chemicals:
                if not n in discovery_beaker.discovered_chemicals:
                    print("\nNew chemical discovered!",n)
                    discovery_beaker.discovered_chemicals.append(n)
            discovery_beaker.contents(temperature,chemdict)
    
        
