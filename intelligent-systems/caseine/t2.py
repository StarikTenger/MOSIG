from pyDatalog import pyDatalog 

class Character(pyDatalog.Mixin): 

    def __init__(self, name, mother, father, house): 
        super(Character, self).__init__() # I have no idea what does this line mean
        self.name = name
        self.mother = mother
        self.father = father
        self.house = house

    
    def __repr__(self): # specifies how to display out Character
        return self.name# + ' - ' + (self.mother.name if self.mother != None else 'No mother') + ' - ' + (self.father.name if self.father != None else 'No father') + ' - ' + (self.house if self.house != '' else 'No house')


characters = []
name_to_character = {}

import csv

with open('got.csv', newline='') as csvfile:
    rdr = csv.reader(csvfile, delimiter=';', quotechar='|')
    for row in rdr:
        new_character = Character(row[0], row[1], row[2], row[3])
        characters.append(new_character)
        name_to_character[new_character.name] = new_character

characters.pop(0)
#print(characters[0])

# Create new characters from ghost parents
for c in characters:
    if c.father != '' and not (c.father in name_to_character):
        new_character = Character(c.father, '', '', '')
        characters.append(new_character)
        name_to_character[new_character.name] = new_character
    if c.mother != '' and not (c.mother in name_to_character):
        new_character = Character(c.mother, '', '', '')
        characters.append(new_character)
        name_to_character[new_character.name] = new_character

santa = Character('SANTA :)', None, None, 'YOUR HOUSE))))')
santa.mother = santa
santa.father = santa

# Replace mother and father from strings to real people objects
for c in characters:
    c.mother = name_to_character[c.mother] if c.mother in name_to_character else santa
    c.father = name_to_character[c.father] if c.father in name_to_character else santa
    
for c in characters:
    if type(c.mother) == str or type(c.father) == str:
        print("foo")
    
#print(characters[0])
    
pyDatalog.create_terms('X, Y, Z, W, grandParent, parent, sibling, cousin, ancestor')#mother, father,
# the manager of Mary is John
# +(mother['John'] == 'Mary')
# +(mother['Sam']  == 'Mary')
# +(mother['Tom']  == 'Mary')
# +(father['Sam'] == 'Thomas')


Character.parent(X, Y) <= (Character.mother[X]==Y)
Character.parent(X, Y) <= (Character.father[X]==Y)
Character.grandParent(X, Z) <= (Character.parent(X, Y)) & (Character.parent(Y, Z))
Character.sibling(X, Y) <= (Character.parent(X, Z)) & (Character.parent(Y, Z)) & (X != Y)
#Character.cousin(X, Y) <= (Character.sibling(Z, W)) & (Character.parent(X, Z)) & (Character.parent(Y, W)) & (X != Y)
Character.cousin(X, Y) <= (Character.grandParent(X, Z)) & (Character.grandParent(Y, Z)) & (X != Y)
#Character.ancestor(X, Y) <= (Character.parent(X, Y))
#Character.ancestor(X, Y) <= Character.ancestor(Character.parent(X, Z), Y)

# print(Character.parent(name_to_character['Arya Stark'], Y))
# print(Character.grandParent(name_to_character['Arya Stark'], Z))

# print(Character.sibling(name_to_character['Arya Stark'], Y))

print(Character.cousin(name_to_character['Arya Stark'], Y))

# print(Character.cousin(name_to_character['Jon Snow'], Y))
