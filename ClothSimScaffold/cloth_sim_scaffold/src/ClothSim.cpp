#include <iostream>

#include <glm/geometric.hpp>// glm::cross, glm::normalize

#include "ClothSim.h"


ClothSim::ClothSim(std::shared_ptr<PlaneMesh> pMesh)
{
	mesh = pMesh;
	res_x = mesh->resWid;
	res_z = mesh->resLen;
	width = mesh->width;
	length = mesh->length;

	restLengthX = mesh->leapWid;
	restLengthZ = mesh->leapLen;
	restLengthXZ = sqrt(pow(mesh->leapWid, 2) + pow(mesh->leapLen, 2));

	init();

}

ClothSim::~ClothSim()
{

}

void ClothSim::init()
{
	forces.clear();
	velocities.clear();
	prev_positions.clear();

	for (int i = 0; i < mesh->vertices.size(); i++) {
		init_positions.push_back(mesh->vertices[i].pos);

		forces.push_back(glm::vec3(0));
		velocities.push_back(glm::vec3(0));
		prev_positions.push_back(mesh->vertices[i].pos);
	}
}

void ClothSim::reInit()
{
	for (int v = 0; v < mesh->vertices.size(); v++) {
		mesh->vertices[v].pos = init_positions[v];
		forces[v] = glm::vec3(0);
		velocities[v] = glm::vec3(0);
		prev_positions[v] = mesh->vertices[v].pos;
	}
}

// get the neighbour id on different directions
int ClothSim::getId(int direction, int id) {
	if (DIRS.WEST == direction) { return id - 1; }
	else if (DIRS.NORTHWEST == direction) { return id + res_x - 1;}
	else if (DIRS.NORTH == direction) {return id + res_x;}
	else if (DIRS.NORTHEAST == direction) {return id + res_x + 1;}
	else if (DIRS.EAST == direction) {return id + 1;}
	else if (DIRS.SOUTHEAST == direction) {return id - res_x + 1;}
	else if (DIRS.SOUTH == direction) {return id - res_x;}
	else {return id - res_x - 1;}
}

// TODO: calculate spring force
glm::vec3 ClothSim::getSpringForce(int direction, int id) {

	// the rest spring length
	float lenRest = 0;
	if (direction == DIRS.NORTH || direction == DIRS.SOUTH) lenRest = restLengthZ;
	else if(direction == DIRS.WEST || direction == DIRS.EAST) lenRest = restLengthX;
	else lenRest = restLengthXZ;

	// the spring vector
	glm::vec3 vSpring = mesh->vertices[id].pos - mesh->vertices[getId(direction, id)].pos;
	// the spring length
	float lenSpring = glm::length(vSpring); // distance
	
	// the unit vector of spring direction
	glm::vec3 vSpringUnit = vSpring /lenSpring;

	// [TODO 1]: calculate spring force vector based on Hooke's law F = -k delta_x
	// 1.1 calculate the length change delta_len
	// replace 0 with your formula
	float delta_len = 0;

	// 1.2 Use Hooke's law F = -k * delta_len * vSpringUnit
	// k : spring_factor
	// replace glm::vec3(0.0) with your formula 
	glm::vec3 spring_force = glm::vec3(0.0);
	return spring_force;
}

// Method functions
void ClothSim::accumulateForces() {
		
	// F(v) = Mg + Fwind + Fairresistance - k*sum(x_current - x_rest)

	// accumulate spring forces from eight directions
	for (int v = 0; v < mesh->vertices.size(); v++) {
		
		glm::vec3 wind = glm::vec3(0, 0, 0);
		glm::vec3 spring = glm::vec3(0, 0, 0);

		std::vector<glm::vec3> spring_directions;

		if (v % res_x != 0) { // WEST 
			glm::vec3 force = getSpringForce(DIRS.WEST, v);
			spring += force;
			spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.WEST, v)].pos));
		}

		if (v < mesh->vertices.size() - res_x) {// NORTH 

			if (v % res_x != 0) {
				glm::vec3 force = getSpringForce(DIRS.NORTHWEST, v);
				spring += force;
				spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.NORTHWEST, v)].pos));
			}

			glm::vec3 force = getSpringForce(DIRS.NORTH, v);
			spring += force;
			spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.NORTH, v)].pos));
			
			if ((v + 1) % res_x != 0) { 
				glm::vec3 force = getSpringForce(DIRS.NORTHEAST, v);
				spring += force;
				spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.NORTHEAST, v)].pos));
			}
		}
	
		if((v + 1) % res_x != 0){ // EAST 
			glm::vec3 force = getSpringForce(DIRS.EAST, v);
			spring += force;
			spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.EAST, v)].pos));
		}

		if (v > res_x - 1) {// SOUTH 
			
			if ((v + 1) % res_x != 0) {
				glm::vec3 force = getSpringForce(DIRS.SOUTHEAST, v);
				spring += force;
				spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.SOUTHEAST, v)].pos));
			}
			
			glm::vec3 force = getSpringForce(DIRS.SOUTH, v);
			spring += force;
			spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.SOUTH, v)].pos));

			if (v % res_x != 0) {
				glm::vec3 force = getSpringForce(DIRS.SOUTHWEST, v);
				spring += force;
				spring_directions.push_back(glm::normalize(mesh->vertices[v].pos - mesh->vertices[getId(DIRS.SOUTHWEST, v)].pos));
			}
			
		}

		// Calculate new normals
		glm::vec3 normal = glm::vec3(0);
		for (int i = 1; i < spring_directions.size(); i++) {
			normal += glm::cross(spring_directions[i], spring_directions[i-1]);
		}
		mesh->vertices[v].normal = glm::normalize(normal);

		// Wind force
		glm::vec3 pos = mesh->vertices[v].pos;

		wind.x = sin(pos.x*pos.y*glfwGetTime());
		wind.y = cos(pos.z*glfwGetTime());
		wind.z = sin(cos(5* pos.x* pos.y* pos.z));
		wind *= wind_factor;

		// calculate Wind Resistance force
		normal = mesh->vertices[v].normal;
		glm::vec3 F_air_resistance = - air_resist_factor *  velocities[v] * glm::abs(glm::dot(normal, velocities[v]));

		// Advanced: Test Sphere Intersection
		float sphere_radius = 4.0f;
		float sphere_friction = 0.8f;
		glm::vec3 sphere_center = glm::vec3(0, 4.0f, 0.0f);
		
		pos = mesh->vertices[v].pos;

		// [TODO 4]: sphere intesection
		// 4.1 replace false with checking if the vertex position falls into the sphere
		if ( false ) {

			// if it is true: 
			// push back the vertex position back to the sphere surface

			// 4.2 calculate the outward vector direction
			// from the sphere center to the vertex position pos
			// glm::vec3 outDir = ???;

			// 4.3 normalise the vector using glm::normalize(glm::vec3)
			// glm::vec3 outDirUnit = ???;

			// 4.4 push back the vertex position back to the sphere surface
			// using sphere_center, outDirUnit and sphere radius
			// mesh->vertices[v].pos = ???;

			// 4.5 downscale the vertex velocity using sphere_friction (< 1)
			// velocities[v] *= ???;
		}


		if (bWind) {
			wind = dirWind * glm::length(mesh->vertices[v].normal * glm::normalize(dirWind));
		}

		if (bPlaySim) {

			// F(v) = (gravity force) + (wind force) + (airresistance force, optional) + (spring force)
			// [TODO 2]: accumulate gravity, wind, air resistance and spring forces
			// replace 0 with your formula
			forces[v] = glm::vec3(0);
		}

		// Pinned vertices
		if (v > mesh->vertices.size() - res_x - 1) {

			if (v % res_x == 0) {
				forces[v] = glm::vec3(0);
			}

			if ((v + 1) % res_x == 0) {
				forces[v] = glm::vec3(0);
			}
		}
	}
}

void ClothSim::forwardEulerIntegration(float dt) {

	// [TODO 3]: calculate acceleration and velocities using forces
	for (int v = 0; v < mesh->vertices.size(); v++) {
		glm::vec3 acceleration = forces[v] * 1.0f; // mass

		// 3.1 update velocity using acceleration
		// velocities[v] = damping_factor * velocities[v] + ??;

		// 3.2 update position using velocity
		// mesh->vertices[v].pos = ??;
	}
}

/*
void ClothSim::backwardEulerIntegration(float dt) {

	for (int v = 0; v < vertices.size(); v++) {


	}
}
*/
void ClothSim::verletIntegration(float dt, int n_iterations) {
	
	for (int v = 0; v < mesh->vertices.size(); v++) {
		glm::vec3 pos = mesh->vertices[v].pos;
		glm::vec3 position_prim = glm::vec3(0);
		glm::vec3 acceleration = forces[v] * 1.0f; // mass
		for (int i = 0; i < n_iterations; i++) {
			position_prim = 2.0f * pos - prev_positions[v] + acceleration * dt * dt;
			prev_positions[v] = pos;
		}
		mesh->vertices[v].pos = position_prim;
	}
}

/* ============================= //
	UPDATE SIMULATION by one time step
// ============================= */ 
void ClothSim::tick(float dt) {
	accumulateForces();
	
	//verletIntegration(dt, 1);
	forwardEulerIntegration(dt);

	mesh->updateVertexBuffer();

}
