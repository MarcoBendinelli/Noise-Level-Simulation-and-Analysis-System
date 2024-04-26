# System for Simulating and Analyzing Noise Levels :musical_note:

This project involves designing and implementing a system to analyze the level of noise in a country. The country is divided into regions, some of which have open access to noise data from sensors, while others do not. The system utilizes sensor data in regions where it's available and generates data using computer simulations in regions without sensors.

For further details, refer to the full report available [here](Report.pdf).

## More Information
### Sensor Data
Mobile IoT devices, such as smartwatches, are equipped with sensors that measure noise levels every 10 seconds. A sliding window calculates the average of the last six readings, which are then reported to the backend through static IoT devices acting as IPv6 border routers. If the average noise level exceeds a certain threshold, raw readings are reported instead of the average.

The IoT devices are implemented using **Contiki-NG** due to its lightweight and efficient operating system tailored for low-power wireless devices. Moreover, **MQTT** was chosen as the network protocol for the IoT devices because of its simplicity and suitability for devices with resource constraints. MQTT works efficiently on IoT devices, allowing for reliable communication over networks with limited bandwidth and intermittent connectivity.

### Noise Levels
In regions without sensor data, the system relies on computer simulations or virtual software-emulated sensors. Simulations are based on population dynamics, considering noise from people and vehicles. The simulation takes parameters such as the number of people and vehicles, the dimensions of the region, noise levels produced by individuals and vehicles, and the distances affected by their presence.

**MPI** was used to parallelize the computation of a compute-intensive task, namely the simulation of noise levels. MPI, or Message Passing Interface, is a standardized and portable message-passing system designed for parallel computing. It allows for efficient communication and coordination between multiple processes running on different nodes of a distributed system. This parallelization technique is essential for speeding up the simulation process, enabling it to handle large datasets and complex calculations effectively.

### Data Cleaning and Enrichment
The backend receives noise level data from real IoT sensors, simulations, and virtual sensors, annotated with geographic areas. Before storage, the backend performs preprocessing:
1. Discarding invalid/incomplete measurements.
2. Associating each reading with the name of the nearest point of interest.

**Node-RED** was chosen due to its browser-based flow editor, which simplifies the process of wiring together flows using a wide range of nodes. Each node in Node-RED implements specific functionalities, eliminating the need to implement them manually. This approach significantly reduces development time and complexity, as well as facilitating integration with other systems and services. Additionally, Node-RED's visual interface allows for easy monitoring and debugging of flows, enhancing overall efficiency and productivity.

### Data Analysis
The backend periodically computes metrics including:
1. Hourly, daily, and weekly moving averages of noise levels for each point of interest.
2. Top 10 points of interest with the highest noise levels in the last hour.
3. Point of interest with the longest streak of good noise levels.

**Spark** was chosen for its ability to process a large volume of data efficiently. Specifically, Spark SQL was preferred over Spark Streaming for several reasons. Firstly, it allows for the calculation of past metrics and enables the integration of new metrics seamlessly. The Spark SQL module facilitates easy querying of the dataset, making it simpler to manage. Additionally, it's worth noting that Spark SQL is more cost-effective than Spark Streaming. Unlike Spark Streaming, Spark SQL does not need to run constantly; it can be executed only when required, reducing overall expenses.

## Team members
| Name and Surname | Github |
:---: | :---: 
| Matteo Beltrante | [@Beltrante](https://github.com/Beltrante) |
| Marco Bendinelli | [@MarcoBendinelli](https://github.com/MarcoBendinelli) |
| Simone Berasi | [@SimoneBerasi](https://github.com/SimoneBerasi) |

## Guide

### Node-RED
#### First Node-RED setup
Follow the guide to install the Node-RED on your Windows Subsystem (which is supposed to be a Debian-based Subsystem): https://nodered.org/docs/getting-started/raspberrypi
#### How to open Node-RED
Start Node-RED:

```shell
node-red
```
If by typing http://127.0.0.1:1880/ the webpage does not appear to you, you should find the local ip address of your Windows Subsystem.

In fact WSL is a virtual environment, so it has its own virtual network interface.

You can find it just by opening a Windows PowerShell window and by typing:

```shell
wsl hostname -I
```
    
So copy the first ip addres that you find and type in your browser: http://copied_ip_address:1880/

If you want to speed up this operation, follow this guide: https://discourse.nodered.org/t/how-to-run-node-red-on-windows-10-wsl-linux-subsystem/45032
#### Palettes to install
node-red-contrib-watchdirectory

### MQTT
#### First Mosquitto setup
In your Client machine (VM - Ubuntu 16.04 32-bit Contiki NG Middleware Class AY 2021) modify the Mosquitto configuration file:
```shell
cd /etc/mosquitto
```
```shell
sudo nano mosquitto.conf
```
And add the following lines in the bottom of the file:
```shell
  connection bridge-01
  address mqtt.neslab.it:3200
  topic # out 0
  topic # in 0
```
Save, Exit and Restart your machine.

### How to run the IoT simulation
Move the folder Iot_implementation inside the contiki-ng-mw-2122

Start Cooja:
```shell
cd /tools/cooja
```
```shell
ant run
```
Open the simulation:

File -> Open Simulation -> Browse -> IoT_Cooja_Simulation.csc

Browse the correct source files

Start the tunslip6:
```shell
cd /rpl-border-router
```
```shell
make TARGET=cooja connect-router-cooja
```
Start the simulation

### MPI
#### Download 
- Download an MPI toolchain to compile and run MPI C programs. For instance, you can use the [Open MPI implementation](https://www.open-mpi.org/).
- On Linux, Open MPI is typically available in package managers
- On mac OS, you can install the open-mpi package through [Homebrew](https://brew.sh/)
- On Windows, you can use WSL
#### Compile and Run 
You can compile and run the simulation with the following commands:
- mpicc ./simulation.c -o simulation -lm
- mpirun -np 2 ./simulation

### Apache Spark
#### Download 
- Download Apache Spark pre-built with hadoop from [this page](https://spark.apache.org/downloads.html)
#### Run
- compile data_analysis jar with maven
- start spark master from inside hadoop folder with following line
```shell
    ./sbin/start-master.sh -h 127.0.0.1
```
- start spark worker from inside hadoop folder with following line
```shell
    ./sbin/start-worker.sh spark://127.0.0.1:7077
```
- submit work to spark master 
```shell
    ./bin/spark-submit --class it.polimi.middleware.spark.DataAnalysis pathToJar masterAddress pathToFolderContainingResults.csv
```
- (submit example)
```shell
     ./bin/spark-submit --class it.middleware.spark.DataAnalysis /mnt/c/Users/m4rc0/Middleware/Project1/Data_Analysis/target/project_1_spark-1.0.jar spark://127.0.0.1:7077 /mnt/c/Users/m4rc0/Middleware/Project1/Back-end/in_out_files/
```



