#ifndef __CLOTH_SIM_MESH__
#define __CLOTH_SIM_MESH__

#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "PlaneMesh.h"

class ClothSim
{
public:
	ClothSim(std::shared_ptr<PlaneMesh> pMesh);
	~ClothSim();
	
	// simulation step
	void tick(float dt);

	void setPlaySim(bool b) {
		bPlaySim = b;
	}

	bool getPlaySim() { return bPlaySim; }

	void setInverseGravity() { gravity *= -1; }

	void reInit();

	void setWindDir(glm::vec3 dir) {
		dirWind = dir;

		if (glm::length(dirWind) > 1e-5)
			bWind = true;
		else
			bWind = false;
	}

	bool isWindEnabled() { return bWind; }

private:

    std::shared_ptr<PlaneMesh> mesh;

	void accumulateForces();
	void verletIntegration(float dt, int n_iterations);
	void forwardEulerIntegration(float dt);
	//void backwardEulerIntegration(float dt);

	// spring network neighbour directions
	struct Directions {
		const int WEST = 0;
		const int NORTHWEST = 1;
		const int NORTH = 2;
		const int NORTHEAST = 3;
		const int EAST = 4;
		const int SOUTHEAST = 5;
		const int SOUTH = 6;
		const int SOUTHWEST = 7;
	};

	Directions DIRS;

	glm::vec3 dirWind = glm::vec3(0, 0, 0);
	bool bWind = false;

	int getId(int direction, int id);
	glm::vec3 getSpringForce(int direction, int id);
	
	void init();

	// Properties
	std::vector<glm::vec3> forces;
	std::vector<glm::vec3> velocities;

	std::vector<glm::vec3> init_positions;
	std::vector<glm::vec3> prev_positions;
	std::vector<glm::vec3> prev_velocities;
	std::vector<glm::vec3> prev_accelerations;
	
	bool bPlaySim = false;

	// rest spring length
	float restLengthX, restLengthZ, restLengthXZ = 0;

	float spring_factor = 800.9f;
	float damping_factor = .9960;
	glm::vec3 gravity = glm::vec3(0, -9.82f, 0);

	float wind_factor = 0.0f;
	float air_resist_factor = .08f;
	
	// cloth width and length
	int width, length;
	// number of vertices on x and z (resolution)
	int res_x, res_z;
};

#endif