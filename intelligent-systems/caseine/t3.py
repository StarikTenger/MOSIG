from pyDatalog import pyDatalog 

class Journey(pyDatalog.Mixin):

    def __init__(self, departure, arrival, time): 
        super(Journey, self).__init__() # I have no idea what does this line mean
        self.departure = departure
        self.arrival = arrival
        self.time = time

    
    def __repr__(self): # specifies how to display out Character
        return "🚂 " + self.departure + " - " + self.arrival + " [" + self.time + "]"


journeys = []

import csv

with open('trains.csv', newline='') as csvfile:
    rdr = csv.reader(csvfile, delimiter=';', quotechar='|')
    for row in rdr:
        new_journey = Journey(row[0], row[1], row[2])
        journeys.append(new_journey)
        
journeys.pop(0)
