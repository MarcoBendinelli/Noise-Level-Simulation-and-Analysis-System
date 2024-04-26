import csv
import random
from datetime import datetime


def generate_simulated_noises():
    rows = 30
    columns = 20

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

    with open('Simulated_noises.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        row = ["longX", "latY", "noiseValue", "dateTime"]

        writer.writerow(row)

        for i in range(rows):
            for j in range(columns):
                row = []
                #long_x = lower_long_x + (i * w)
                #lat_y = upper_lat_y - (j * h)
                n = random.randint(20, 60)
                time = datetime.today().strftime('%Y-%m-%d %H:%M:%S')

                row.append(i)
                row.append(j)
                row.append(n)
                row.append(time)

                writer.writerow(row)


def generate_csv_file_with_points_of_interest_and_noises():
    points = []
    number_of_elements = 500

    with open('points.txt', 'r') as f:
        for line in f:
            points.append(line[:-1])

    with open('results.csv', 'w', newline='') as f:
        writer = csv.writer(f)

        for i in range(number_of_elements):
            row = []
            long_x = random.SystemRandom().uniform(6.6, 18.5)
            lat_y = random.SystemRandom().uniform(35.5, 41.3)
            n = random.randint(20, 60)
            city = points[random.randint(0, len(points) - 1)]
            # 13-04-2022  16:27:21
            date = str(random.randint(1, 31)) + '-' + str(random.randint(1, 12)) + '-' + str(random.randint(1999, 2022))
            time = str(random.randint(0, 23)) + ':' + str(random.randint(0, 59)) + ':' + str(random.randint(0, 59))
            formatted_date = date + " " + time

            row.append(long_x)
            row.append(lat_y)
            row.append(n)
            row.append(city)
            row.append(formatted_date)

            writer.writerow(row)
