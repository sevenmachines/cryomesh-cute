/*
 * MeshTest.h
 *
 *  Created on: 1 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef MESHTEST_H_
#define MESHTEST_H_

#ifdef ENABLE_ACTIVITYGRID
#include "structures/Mesh.h"
#include "ICuteSuite.h"

namespace cryomesh {

namespace structures {

class MeshTest {
public:
	MeshTest(){}
	virtual ~MeshTest(){}
	static void runSuite();
	static void testCreation();
	static void testUpdate();
	static void testWarpNode();
	static void testWarpImpulse();

};

}

}

#endif /* MESHTEST_H_ */
#endif //ENABLE_ACTIVITYGRID
