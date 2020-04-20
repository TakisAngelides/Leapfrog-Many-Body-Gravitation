//The use of the vector2D structure follows from the Java Platform. It allows for quick vector 
//manipulations and it is more elegant than always specifying dimensions. The Planet class acts as
//a state vector carrying for each planet the necessary information for the leapfrog iteration.
//The leapfrog iteration should increment x by 0.5dt*v, increment t by 0.5dt, find f, increment
//v by dt*f/m, increment x y 0.5dt*v and increment t by 0.5dt.


#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

//Definition of the gravitational constant G

#define G 0.001

//Definitions of structures vector2D and Planet

struct vector2D //If you are from planet Python this is the equivalent of a class
{
  double x;
  double y;
};

struct Planet //If you are from planet Python this is the equivalent of a class
{
  double mass;
  vector2D force;
  vector2D velocity;
  vector2D position;
};

//Definitions of arithmetic operations on vector2D objects and other useful functions

double magnitude(vector2D &a) //Takes a vectord2D object and returns its magnitude as a double
{
  return (sqrt(pow(a.x,2)+pow(a.y,2)));
}

vector2D operator+(vector2D const &a, vector2D const &b) //The vector2D in the signature here specifies what type is the return
//A variable that stores the address of another variable is a pointer. By preceding a pointer with a * we fetch the actual value.
{
  vector2D final;
  final.x = a.x + b.x;
  final.y = a.y + b.y;
  return final;
}

vector2D operator-(vector2D const &a, vector2D const &b) //The & symbol means we pass in the address of the variable in memory and not the value
{
  vector2D final;
  final.x = a.x - b.x;
  final.y = a.y - b.y;
  return final;
}

vector2D operator*(vector2D const &a, double const &b) //Multiplication of a vector with a scalar b
{
  vector2D final;
  final.x = a.x * b;
  final.y = a.y * b;
  return final;
}

//Using a for loop the program will iterate all planets in the solarSystem printing out their position

void showState(Planet* arr, int numberOfPlanets) //We give this function a pointer to an array of Planet objects, the numberOfPlanets is just the length of the array
{
	for(int j=0;j<numberOfPlanets;j++)
	{
		cout << arr[j].position.x << "\t" << arr[j].position.y << "\t"; //Print x,y position
	}

	cout << endl;
}

//The function calculates the force of planet b on planet a 

vector2D gForce(Planet a, Planet b) //Takes 2 planet objects
{
	//The constant A can be changed to simulate electrodynamics

	double A = G*a.mass*b.mass;
	vector2D r = (b.position - a.position); //Define the vector r1 - r2
	vector2D gravityForce; //Vector to represent the x and y components of the force

	//To find magnitude of force
		
	gravityForce.x = (A*pow(magnitude(r),-2));
	gravityForce.y = (A*pow(magnitude(r),-2));

	//To set correct direction with r hat

	gravityForce.x = (gravityForce.x)*(-r.x/magnitude(r));
	gravityForce.y = (gravityForce.y)*(-r.y/magnitude(r));

	//Handle divideByZero

	if(fabs(r.x)<=0.1 && fabs(r.y)<=0.1)
	{
		gravityForce.x = 0;
		gravityForce.y = 0;
	}

	return gravityForce;
}

void leapFrog(Planet* arr, int numberOfPlanets, double &time, double timeStep,int numberOfIterations)
{
	vector2D gravForce;
	
	for(int i=0;i<numberOfIterations;i++)
	{
			
		//To update position x = x + v*(0.5*dt)
	
		for(int i=0;i<numberOfPlanets;i++)
		{
			arr[i].position = arr[i].position + (arr[i].velocity*(0.5*timeStep));
			time += 0.5*timeStep; //Increment time by 0.5*dt
		}

		//To update force we call the gForce function for every pair of planets 

		for(int i=0;i<numberOfPlanets;i++)
		{
			for(int j=i+1;j<numberOfPlanets;j++)
			{
				gravForce = gForce(arr[i],arr[j]); //Find f = f(x,t)
				arr[j].force = arr[j].force + gravForce; //Updates state vector for force field
				arr[i].force = arr[i].force - gravForce; //Updates state vector for force field 
			}
		}

		//To update velocity and position v = v + (force/masss) * dt , x = x + v*(0.5*dt)

		for(int i=0;i<numberOfPlanets;i++)
		{
			arr[i].velocity = arr[i].velocity + (arr[i].force*(timeStep/arr[i].mass));
			arr[i].position = arr[i].position + (arr[i].velocity*(0.5*timeStep));
			showState(arr,numberOfPlanets); //Prints out the position vector for all planets in the solarSystem array
			arr[i].force.x = 0;
			arr[i].force.y = 0;
			time += 0.5*timeStep;
		}
	}
}


int main()
{
	//Definitions for leapfrog
	
	int numberOfPlanets = 4;
	int numberOfIterations = 2000;
	double timeStep = 0.2;
	double time = 0.0;

	//Each Planet in the array requires a field setting for mass, velocity and position

	Planet solarSystem[numberOfPlanets];

	solarSystem[0].mass = 100000000;
	solarSystem[0].position.x = 0;
	solarSystem[0].position.y = 0;
	solarSystem[0].velocity.x = 0;
	solarSystem[0].velocity.y = 0;

	solarSystem[1].mass = 1000;
	solarSystem[1].position.x = 70;
	solarSystem[1].position.y = -150;
	solarSystem[1].velocity.x = 12;
	solarSystem[1].velocity.y = -6;

	solarSystem[2].mass = 9000;
	solarSystem[2].position.x = 90;
	solarSystem[2].position.y = 90;
	solarSystem[2].velocity.x = 15;
	solarSystem[2].velocity.y = -15;

	solarSystem[3].mass = 400;
	solarSystem[3].position.x = 50;
	solarSystem[3].position.y = -50;
	solarSystem[3].velocity.x = -20;
	solarSystem[3].velocity.y = -20;

	leapFrog(solarSystem,numberOfPlanets,time,timeStep,numberOfIterations);

	return 0;	 
}
