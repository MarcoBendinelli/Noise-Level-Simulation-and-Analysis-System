# MiddlewareProject1

## [Node-RED](https://github.com/Beltrante/Middleware/tree/main/Project1/Back-end)
### First Node-RED setup
Follow the guide to install the Node-RED on your Windows Subsystem (which is supposed to be a Debian-based Subsystem): https://nodered.org/docs/getting-started/raspberrypi
### How to open Node-RED
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
### Palettes to install
node-red-contrib-watchdirectory

## [MQTT](https://github.com/Beltrante/Middleware/tree/main/Project1/Iot_implementation)
### First Mosquitto setup
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

### How to run the simulation
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



## [MPI](https://github.com/Beltrante/Middleware/tree/main/Project1/Simulation)
### Download 
- Download an MPI toolchain to compile and run MPI C programs. For instance, you can use the [Open MPI implementation](https://www.open-mpi.org/).
- On Linux, Open MPI is typically available in package managers
- On mac OS, you can install the open-mpi package through [Homebrew](https://brew.sh/)
- On Windows, you can use WSL
### Compile and Run 
You can compile and run the simulation with the following commands:
- mpicc ./simulation.c -o simulation -lm
- mpirun -np 2 ./simulation

## [Apache Spark](https://github.com/Beltrante/Middleware/tree/main/Project1/Data_Analysis)

### Download 
- Download Apache Spark pre-built with hadoop from [this page](https://spark.apache.org/downloads.html)
### Run
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
     ./bin/spark-submit --class it.polimi.middleware.spark.DataAnalysis /mnt/c/Users/matteo/Middleware/Project1/Data_Analysis/target/project_1_spark-1.0.jar spark://127.0.0.1:7077 /mnt/c/Users/matteo/Middleware/Project1/Back-end/In-Out_files/
```



