#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>


#define PI 3.14
#define SIMULATION_STEPS 1
#define COMMA 44

//Default values, used if no correct values are provided
#define PEOPLE_NUMBER 1000
#define VEHICLE_NUMBER 0
#define REGION_WIDTH 3000
#define REGION_LENGTH 3000
#define VEHICLE_NOISE 70
#define PERSON_NOISE 60
#define PERSON_AREA_AFFECTED 100
#define VEHICLE_AREA_AFFECTED 100
#define PERSON_SPEED 1
#define VEHICLE_SPEED 200
#define TIME_STEP 10

FILE *file;
time_t starting_t;
struct tm starting_tm;

int people_number;
int vehicle_number;
int region_width;
int region_length;
float vehicle_noise;
float person_noise;
int person_area_affected;
int vehicle_area_affected;
float person_speed;
float vehicle_speed;
int time_step;

int person_distance_affected;
int vehicle_distance_affected;

//Assign default values to variables
void generate_default_parameters(){
	people_number = PEOPLE_NUMBER;
    vehicle_number = VEHICLE_NUMBER;
    region_width = REGION_WIDTH;
    region_length = REGION_LENGTH;
    vehicle_noise = VEHICLE_NOISE;
    person_noise = PERSON_NOISE;
    person_area_affected = PERSON_AREA_AFFECTED;
    vehicle_area_affected = VEHICLE_AREA_AFFECTED;
    person_speed = PERSON_SPEED;
    vehicle_speed = VEHICLE_SPEED;
    time_step = TIME_STEP;
}


//Type defining a person
typedef struct person_t	
{
	int id;
	int x_position;     
	int y_position;
	float x_velocity;   //speed in m/s along x axis
	float y_velocity;   //speed in m/s along x axis

} person;

//Type defining a vehicle
typedef struct vehicle_t
{
	int id;
	int x_position;
	int y_position;
	float x_velocity;
	float y_velocity;
  
} vehicle;





//Compute the new position and velocity (which change only in the direction, if a border is hit) after time_step seconds of every person
//The case in which the person goes out of the border is handled as if it hit the border and change direction
void update_people(person *people, int num_people) {
	
	for (int i=0; i < num_people; i++) {
		//printf("person x position: %d\n", people[i].x_position);

		people[i].x_position = people[i].x_position + round(people[i].x_velocity * time_step);
		people[i].y_position = people[i].y_position + round(people[i].y_velocity * time_step);
		
		if (people[i].x_position < 0) {
			people[i].x_position = people[i].x_position * (-1);
			people[i].x_velocity = people[i].x_velocity * (-1);
		}
		
		if (people[i].x_position >= region_length) {

			int j =0;
			while(people[i].x_position >= region_length*2){
				people[i].x_position = people[i].x_position - region_length;
				j++;
			}

			if(j%2 == 0){
				people[i].x_position = region_length - (people[i].x_position - region_length);
				people[i].x_velocity = people[i].x_velocity * (-1);
			}
			else{
				people[i].x_position = people[i].x_position - region_length;


			}		
			
		}


		if (people[i].y_position < 0) {
			people[i].y_position = people[i].y_position * (-1);
			people[i].y_velocity = people[i].y_velocity * (-1);
		}

		
		if (people[i].y_position >= region_width) {

			int j = 0;
			while(people[i].y_position >= region_width*2){
				people[i].y_position = people[i].y_position - region_width;
				j++;
			}

			if(j%2 == 0){
				people[i].y_position = region_width - (people[i].y_position - region_width);
				people[i].y_velocity = people[i].y_velocity * (-1);
			}
			else{
				people[i].y_position = people[i].y_position - region_width;
			}

		}
		

	}


}

//Compute the new position and velocity (which change only in the direction, if a border is hit) after time_step seconds of every vehicle
void update_vehicles(vehicle *vehicles, int num_vehicles) {
	
	
	for (int i=0; i < num_vehicles; i++) {


		vehicles[i].x_position = vehicles[i].x_position + round(vehicles[i].x_velocity * time_step);
		vehicles[i].y_position = vehicles[i].y_position + round(vehicles[i].y_velocity * time_step);
		
		if (vehicles[i].x_position < 0) {
			vehicles[i].x_position = vehicles[i].x_position * (-1);
			vehicles[i].x_velocity = vehicles[i].x_velocity * (-1);
		}
		
		if (vehicles[i].x_position >= region_length) {

			int j =0;
			while(vehicles[i].x_position >= region_length*2){
				vehicles[i].x_position = vehicles[i].x_position - region_length;
				j++;
			}

			if(j%2 == 0){
				vehicles[i].x_position = region_length - (vehicles[i].x_position - region_length);
				vehicles[i].x_velocity = vehicles[i].x_velocity * (-1);
			}
			else{
				vehicles[i].x_position = vehicles[i].x_position - region_length;


			}		
			
		}


		if (vehicles[i].y_position < 0) {
			vehicles[i].y_position = vehicles[i].y_position * (-1);
			vehicles[i].y_velocity = vehicles[i].y_velocity * (-1);
		}

		
		if (vehicles[i].y_position >= region_width) {

			int j = 0;
			while(vehicles[i].y_position >= region_width*2){
				vehicles[i].y_position = vehicles[i].y_position - region_width;
				j++;
			}

			if(j%2 == 0){
				vehicles[i].y_position = region_width - (vehicles[i].y_position - region_width);
				vehicles[i].y_velocity = vehicles[i].y_velocity * (-1);
			}
			else{
				vehicles[i].y_position = vehicles[i].y_position - region_width;
			}

		}
		

	}


}





//Generate a person in random position with random velocity
person generate_person(int id) {

	person temp_person;
	float direction;		//angular direction where the velocity is directed

	temp_person.id = id;
	temp_person.x_position = (rand() % (int)(region_length));
	
	temp_person.y_position = (rand() % (int)(region_width));

	direction = ((float)(rand() % ((360 + 1)*100) ))/100;
	temp_person.x_velocity = person_speed * cos((direction*PI)/180);
	temp_person.y_velocity = person_speed * sin((direction*PI)/180);

	return temp_person;

}



//Generate a vehicle in random position with random velocity
vehicle generate_vehicle(int id) {

	vehicle temp_vehicle;
	float direction;		//angular direction where the velocity is directed

	temp_vehicle.id = id;
	temp_vehicle.x_position = (rand() % (int)(region_length ));
	temp_vehicle.y_position = (rand() % (int)(region_width));

	direction = ((float)(rand() % ((360 + 1)*100) ))/100;
	temp_vehicle.x_velocity = vehicle_speed * cos((direction*PI)/180);
	temp_vehicle.y_velocity = vehicle_speed * sin((direction*PI)/180);

	return temp_vehicle;

}

//Generate vehicles
person* generate_people(int num_people) {

	person *persons = (person*) malloc(num_people * sizeof(person));

	for (int i=0; i<num_people; i++) {
		persons[i] = generate_person(i);
		//printf("x pos: %d\n", persons[i].x_position);

	}

	return persons;

}


//Generate vehicles
vehicle* generate_vehicles(int num_vehicles) {

	vehicle *vehicles = (vehicle*) malloc(num_vehicles * sizeof(vehicle));

	for (int i=0; i<num_vehicles; i++) {
		vehicles[i] = generate_vehicle(i);
	}

	return vehicles;

}


//Compute the sum of two noise overlapping
float compute_noise_sum(float noise0, float noise1) {


	if (noise0==0 && noise1==0){
		return 0;
	}

	return 10* (log10 (pow(10, noise0/10) + pow(10, noise1/10)));


}


//Set at 0 the noise in every square meter of the region
void initialize_noise(float *region_noise) {

	for (int i=0; i<region_length * region_width; i++) {
		region_noise[i] = 0;
	}

}


//Compute the noise caused by people and vehicles in every square meter of the region
void update_noise(float *region_noise, person *people, vehicle *vehicles, int num_people, int num_vehicles) {

//Start code for people contribution

	for (int i=0; i<num_people; i++) {
		int x = people[i].x_position;
		int y = people[i].y_position;

		for (int j = x - person_distance_affected; j <= x + person_distance_affected; j++) {
			for (int k = y - person_distance_affected; k <= y + person_distance_affected; k++) {
				if (j >= 0 && k >= 0 && j < region_length && k < region_width)  {
					region_noise[j + k*region_length] = compute_noise_sum(region_noise[j + k*region_length], person_noise);
				}
			}
		}
	}

//End code for people contribution and start code for vehicles contribution


	for (int i=0; i<num_vehicles; i++) {
		int x = vehicles[i].x_position;
		int y = vehicles[i].y_position;

		for (int j = x - vehicle_distance_affected; j <= x + vehicle_distance_affected; j++) {
			for (int k = y - vehicle_distance_affected; k <= y + vehicle_distance_affected; k++) {
				if (j >= 0 && k >= 0 && j < region_length && k < region_width)  {
					region_noise[j + k*region_length] = compute_noise_sum(region_noise[j + k*region_length], vehicle_noise);
				}
			}
		}
	}



}


//Check if provided parameters are coherent
void check_parameters(){
    if(people_number < 0 || vehicle_number < 0 || region_width <= 0 || region_length <= 0 || person_noise < 0 || vehicle_noise < 0 ||
    person_area_affected <0 || vehicle_area_affected <0 || person_speed <0 || vehicle_speed <0 || time_step<=0){

        printf("Incorrect parameters value, using defaults\n");

        generate_default_parameters();
    } 
}

void set_time(){
	starting_t = time(NULL);
	starting_tm = *localtime(&starting_t);
}

void print_result(float *region_noise, int current_step) {

	struct tm tm = starting_tm;

	tm.tm_sec = tm.tm_sec + current_step * time_step;

	mktime(&tm);

	if(current_step==0){
		file = fopen("result.csv", "a+");
	}


	for(int i=0; i < region_length*region_width; i++){
		
		int x = i % region_length;
		int y = (int)floor(i/region_width);

		fprintf(file, "%d,%d,%f,%d-%02d-%02d %02d:%02d:%02d\n", x,  y, region_noise[i], tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	}

	if(current_step == SIMULATION_STEPS - 1){
		fclose(file);
	}

}



int main(int argc, char  *argv[]) {



	MPI_Init(&argc, &argv);

	int my_rank, world_size;

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	

	srand(time(NULL) + my_rank);

    if (argc <12){
        if(my_rank == 0){
            printf("Incorrect number of parameters, using defaults\n");
        }
	    generate_default_parameters();
	}
    else {
        
        people_number = atoi(argv[1]);
        vehicle_number = atoi(argv[2]);
        region_width = atoi(argv[3]);
        region_length = atoi(argv[4]);
        person_noise = atof(argv[5]);
        vehicle_noise = atof(argv[6]);
        person_area_affected = atoi(argv[7]);
        vehicle_area_affected = atoi(argv[8]);
        person_speed = atof(argv[9]);
        time_step = atoi(argv[11]);
        vehicle_speed = atof(argv[10]);
        check_parameters();

    }

    MPI_Barrier(MPI_COMM_WORLD);

    double starting_time = MPI_Wtime(); 
    
	//compute the number of people and vehicles to generate for each process
	int people_per_process = people_number / world_size;
	int vehicles_per_process = vehicle_number / world_size;
	int people_per_process0 = people_per_process + (people_number - people_per_process*(world_size));
	int vehicles_per_process0 = vehicles_per_process + (vehicle_number - vehicles_per_process*(world_size));
	
	person_distance_affected = (int)person_area_affected/2;
	vehicle_distance_affected = (int)vehicle_area_affected/2;

	float *region_noise = (float*) malloc(region_length * region_width * sizeof(float));		//linearized square matrix to have simpler MPI messages
	person *local_people;
	vehicle *local_vehicles;

	if (my_rank == 0) {
		people_per_process = people_per_process0;
		vehicles_per_process = vehicles_per_process0;
		set_time();
	}

	//Generate people
	
	local_people = generate_people(people_per_process);
	local_vehicles = generate_vehicles(vehicles_per_process);



	//Generate vehicles


	MPI_Barrier(MPI_COMM_WORLD);


	float *results;
    float *chunk_noise;

	
	results = (float*) malloc(world_size * region_length * region_width * sizeof(float));		//linearized square matrix to have simpler MPI messages

	


	//starting the simulation
	for (int s=0; s<SIMULATION_STEPS; s++){

		initialize_noise(region_noise);
		update_noise(region_noise, local_people, local_vehicles, people_per_process, vehicles_per_process);
		update_people(local_people, people_per_process);
		update_vehicles(local_vehicles, vehicles_per_process);

		MPI_Barrier(MPI_COMM_WORLD);



		MPI_Allgather(
			region_noise,
			region_length*region_width,
			MPI_FLOAT,
			results,
			region_width*region_length,
			MPI_FLOAT,
			MPI_COMM_WORLD
		);

		MPI_Barrier(MPI_COMM_WORLD);
		


        int chunk = (int) region_length*region_width / world_size;


        MPI_Barrier(MPI_COMM_WORLD);

        for (int i=0; i<region_length*region_width; i++){
            region_noise[i]=0;
        }

        for (int i=0; i<world_size; i++){
            for(int j=0; j<chunk; j++){
                region_noise[j + chunk*i] = compute_noise_sum(region_noise[j + chunk*i], results[j + i*region_length*region_width + my_rank*chunk]);
            }
        }


        chunk_noise = (float*) malloc(chunk * sizeof(float));

        for (int i=0; i<chunk; i++){
            chunk_noise[i]=0;
            for (int j=0; j<world_size; j++){
                chunk_noise[i] = compute_noise_sum(chunk_noise[i], region_noise[i + j*chunk]);
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

		MPI_Gather(
			chunk_noise,
			chunk,
			MPI_FLOAT,
			region_noise,
			chunk,
			MPI_FLOAT,
			0,
			MPI_COMM_WORLD
		);


        if (my_rank==0){

            //print_result(region_noise, s);
        }


		MPI_Barrier(MPI_COMM_WORLD);
		

	}



	free(results);

    free(chunk_noise);
	free(local_people);
	free(local_vehicles);
	free(region_noise);

	double ending_time = MPI_Wtime(); 
	printf("The whole process took seconds: %f\n", ending_time - starting_time);
  	MPI_Finalize();

}