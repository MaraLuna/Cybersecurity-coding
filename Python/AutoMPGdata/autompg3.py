
# AutoMPG2.py

import logging, fileinput, requests, sys, argparse,os,csv, math
from collections import defaultdict, namedtuple
import matplotlib.pyplot as plt
import numpy as np

#Creates a logger and sets level to Debug
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

#Defines File Handler and adds to logger
file_h = logging.FileHandler('autompg_logfile.log','w')
file_h.setLevel(logging.DEBUG)
logger.addHandler(file_h)

# Defines Stream Handler to log to the console
sh = logging.StreamHandler()
sh.setLevel(logging.INFO)
logger.addHandler(sh)


class AutoMPG():
    def __init__(self,make,model,year, mpg):
        self.make = str(make)
        self.model = str(model)
        self.year = int (year)
        self.mpg = float(mpg)

    def __str__ (self):
        return (repr(self))

    def __repr__ (self):
        return (f"AutoMPG ({self.make}, {self.model}, {self.year}, {self.mpg})")

    def __eq__(self, other):
        if type(self) == type(other):
            return self.make == other.make and self.model == other.model and self.year == other.year and self.mpg == other.mpg
        else:
            return NotImplemented

    def __lt__(self, other):
        if self.make != other.make:
            return self.make < other.make
        elif self.model != other.model:
            return self.model < other.model
        elif self.year != other.year:
            return self.year < other.year
        else:
            return self.mpg < other.mpg

    def __hash__(self):
        return hash ((self.make, self.model, self.year, self.mpg))

class AutoMPGData():

    def __init__(self):
        self.data = []
        self._load_data()

    def _clean_data (self):   # Stripping input file of whitespaces, misspellings and writing to a clean file
        logger.info('Running Clean Data...')
        input_filename = 'auto-mpg.data'
        output_filename = 'auto-mpg.clean.txt'
       
        with open(input_filename, 'r', newline='') as f:
                with open (output_filename, 'w', newline='') as of:
                    for row in f:
                        x = row.expandtabs(2)
                        x = f.read().replace("vw", "volkswagen").replace("vokswagen", "volkswagen").replace("chevroelt", "chevrolet").replace("chevy","chevrolet").replace("maxda", "mazda").replace("mercedes-benz", "mercedes").replace("toyouta", "toyota")
                        of.write(x)

    def _load_data(self):     # Loading cleaned file and creating AutoMPG objects with 4 attributes
        logger.info('Running Load Data')
        input_filename = 'auto-mpg.data'
        output_filename = 'auto-mpg.clean.txt'
        if not os.path.exists(input_filename):
            logger.debug('File auto-mpg.data.txt does not exist, going to get_data.')
            self._get_data() 
        if not os.path.exists(output_filename):
            logger.debug('File auto-mpg.data.clean.txt does not exist, calling clean_data to create.')
            self._clean_data()
        car_list = []
        Record= namedtuple("Record",['mpg','cylinders','displacement','horsepower','weight','acceleration','year', 'origin','name'])
        with open (output_filename, 'r', newline='') as file:
            for row in file:
                c = row.strip("")
                c = c.split()
                c[8] = " ".join(c[8:])
                v=c[8]
                z = v.strip('"')
                car_list.append(Record(c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],z))
                self.make = (z.split(' ')[0])
                model1 = list(z.split(' ')[1:])
                self.model = " ".join(model1)
                self.data.append(AutoMPG(self.make, self.model, c[6], c[0]))

    def mpg_by_year(self,plot):
        year_dict = defaultdict(lambda: [])
        for cars in self.data:
            year_dict[cars.year].append(cars.mpg)
        for keys in year_dict:
            avg = round(sum(year_dict[keys])/len(year_dict[keys]),2)
            print(f"Year {keys} is average mpg of {avg}")
        if plot == True:
            plt.bar(sorted(year_dict.keys()), [year_dict[key][1] for key in sorted(year_dict.keys())])
            plt.xlabel("Years")
            plt.ylabel("Average mpg")
            plt.title('Plot Years average mpg')
            plt.show()
        #logging.info(year_dict.items())

    def mpg_by_make(self,plot):
        make_dict = defaultdict(lambda: [])
        for cars in self.data:
            make_dict[cars.make].append(cars.mpg)
        for keys in make_dict:
            avg = round(sum(make_dict[keys])/len(make_dict[keys]),2)
            print(f"Make {keys} is average mpg of {avg}")
        if plot == True:
            plt.bar(sorted(make_dict.keys()), [make_dict[key][0] for key in sorted(make_dict.keys())])
            plt.xlabel("Make")
            plt.ylabel("Average mpg")
            plt.title('Plot Make average mpg')
            plt.show()
        #logging.info(make_dict.items())

    def __iter__(self):
        return iter(list(self.data))
    
    def __next__(self):
        return next(self.data)

    def _get_data(self):     #  Downloading data file from web
        logger.info('Downloading auto-mpg data from https://archive.ics.uci.edu/ml/machine-learning-databases/auto-mpg/auto-mpg.data..')
        try:
            data = requests.get('https://archive.ics.uci.edu/ml/machine-learning-databases/auto-mpg/auto-mpg.data')
            open('auto-mpg.data', 'wb').write(data.content)
            logger.info('Data downloaded to file auto-mpg.data.')
        except:
            print("Unable to reach web address and downnload data file.")
            logger.debug("File Not Found auto-mpg.data @ https://archive.ics.uci.edu/ml/machine-learning-databases/auto-mpg/auto-mpg.data")

    def sort_by_default(self):   # default sort by make/model
        self.data.sort()

    def sort_by_year(self):     #  Sort by year
        self.data.sort(key=lambda obj: (obj.year, obj.make, obj.model, obj.mpg))

    def sort_by_mpg(self):      # Sort by mpg
        self.data.sort(key=lambda obj: (obj.mpg, obj.make, obj.model, obj.year))

def main():
    parser = argparse.ArgumentParser(description= 'analyze Auto MPG data set')
    parser.add_argument('-l', '--print',
                        dest ='sort',action = 'store', metavar='<sort print order>',choices=['year','mpg','default'],default= 'default',
                        help ="To print entire list with a sort value specified, 'default' is by make/model, or choose 'year' or 'mpg'")
    parser.add_argument('-y','--year',dest = 'avg_year',default = False, action = "store_true",
                            help= 'Averages all cars by year')
    parser.add_argument('-m','--make', dest = 'avg_make', default = False, action = "store_true",
                            help= 'Averages all cars by make')
    parser.add_argument('-o', '--ofile',
                            default= 'sys.stdout',dest='csv_file', metavar="<outfile>",
                            action='store', help='output to csv file or default is stdout')
    parser.add_argument('-p', '--plot',
                            dest='do_plot', default = False, action='store_true',
                            help='plot the data')
    args=parser.parse_args()

    b = AutoMPGData()

    if args.sort !=None:
        logger.info(f"Sorting data by {args.sort}.")
        if args.sort == 'default':
            b.sort_by_default()
        elif args.sort == 'year':
            b.sort_by_year()
        elif args.sort =='mpg':
            b.sort_by_mpg()
        logger.info ("Done sorting data.")
        logger.info ("Printing sorted data...")

        # for a in b:
        #     print(a)
        if args.csv_file is not None:
            with open(args.csv_file, 'w') as f:
                writer = csv.writer(f)
                writer.writerow(b)

        if args.avg_year == True:
            x=b.mpg_by_year(args.do_plot)

        if args.avg_make == True:
            x=b.mpg_by_make(args.do_plot)


if __name__ == '__main__':
    main()