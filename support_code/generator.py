import csv
import random
import sys
from tqdm import tqdm
from datetime import datetime
from datetime import timedelta


def generate_simulated_noises():
    rows = 40
    columns = 50

    upper_long_x = 18.51944
    lower_long_x = 6.626111
    upper_lat_y = 41.3
    lower_lat_y = 35.49

    #   6,41.3        h       18,41.3
    #      w                     w
    #    6,35         h        18,35

    w = upper_long_x - lower_long_x
    h = upper_lat_y - lower_lat_y
    w = w / (rows - 1)
    h = h / (columns - 1)

    with open('Simulated_noises.txt', 'w') as f:
        for i in range(rows):
            for j in range(columns):
                long_x = lower_long_x + (i * w)
                lat_y = upper_lat_y - (j * h)
                n = random.randint(20, 60)
                time = datetime.today().strftime('%Y-%m-%d %H:%M:%S')
                line = str(long_x) + ',' + str(lat_y) + " " + str(n) + " " + time
                f.write(line)
                f.write('\n')


def generate_csv_file_with_points_of_interest_and_noises():
    points = []
    number_of_elements = int(sys.argv[1])

    with open('points.txt', 'r') as f:
        for line in f:
            points.append(line[:-1])

    with open('results.csv', 'w', newline='') as f:
        writer = csv.writer(f)

        for i in tqdm(range(number_of_elements)):
            row = []
            long_x = random.SystemRandom().uniform(6.6, 18.5)
            lat_y = random.SystemRandom().uniform(35.5, 41.3)
            n = random.randint(20, 60)
            city = points[random.randint(0, len(points) - 1)]
            # 13/04/2022  16:27:21
            thisDay = datetime.today().strftime('%d') 
            thisMonth = datetime.today().strftime('%m')
            lastWeek = int(thisDay)-7
            thisYear = datetime.today().strftime('%Y')
            date = str(thisYear) + '-' + str(thisMonth) + '-' + str(random.randint(lastWeek,int(thisDay)))
            time = str(random.randint(0, 23)) + ':' + str(random.randint(0, 59)) + ':' + str(random.randint(0, 59))
            formatted_date = date + " " + time

            row.append(long_x)
            row.append(lat_y)
            row.append(n)
            row.append(city)
            row.append(formatted_date)

            writer.writerow(row)


generate_csv_file_with_points_of_interest_and_noises()