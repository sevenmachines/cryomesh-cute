/*
 * NodeMeshTest.h
 *
 *  Created on: 26 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef NODEMESHTEST_H_
#define NODEMESHTEST_H_


namespace cryomesh {

namespace structures {

class NodeMeshTest {
public:
	NodeMeshTest(){}
	virtual ~NodeMeshTest(){}

	static void runSuite();
	static void testRegenerateNeighbourhoods();
	static void testRegenerateActivities();
	static void testWarpNodes();
};

}

}

#endif /* NODEMESHTEST_H_ */
