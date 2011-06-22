/*
 * MeshTest.cpp
 *
 *  Created on: 1 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "MeshTest.h"
#include "structures/Mesh.h"
#include "structures/Cluster.h"
#include "spacial/Point.h"
using namespace cryomesh::spacial;

namespace cryomesh {

namespace structures {

void MeshTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(MeshTest::testCreation));
	s.push_back(CUTE(MeshTest::testUpdate));
	s.push_back(CUTE(MeshTest::testWarpNode));
	s.push_back(CUTE(MeshTest::testWarpImpulse));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "MeshTest");
}

void MeshTest::testCreation() {
	const double DELTA = 0.00001;
	Cluster cluster(10, 2);
	boost::shared_ptr<Mesh> mesh = cluster.getMesh();
	//std::cout<<"MeshTest::testCreation: "<<""<<std::endl;
	//std::cout <<*(mesh.getActivityGrid())<<std::endl;
	// make sure grid is created and updated as an initialisation
	// check distance of bouding boxes
	{
		const spacial::Point & cluster_bb = cluster.getMaxBoundingBox();
		const spacial::Point mesh_bb = Point(mesh->getActivityGrid()->getBoundingUnitCoordinates());
		double dist = mesh_bb.getDistance(cluster_bb);
		ASSERT_EQUAL_DELTA(0.0, dist, DELTA);
	}
}

void MeshTest::testUpdate() {
	common::TimeKeeper::getTimeKeeper().update();
	Cluster cluster(10, 2);
	Point new_point(4.5, 4.5, 4.5);
	const double TEST_ACTIVITY = 3.9;
	boost::shared_ptr<components::Node> random_node = cluster.getMutableNodeMap().getMutableRandomObject();
	random_node->setPosition(new_point);

	boost::shared_ptr<components::Impulse> imp1(new components::Impulse(1.97, 10));
	boost::shared_ptr<components::Impulse> imp2(new components::Impulse(-1.4, 10));
	random_node->addImpulse(imp1);
	random_node->addImpulse(imp2);

	std::cout << "MeshTest::testUpdate: " << "" << std::endl;
	std::cout << *random_node << std::endl;
	common::TimeKeeper::getTimeKeeper().update();
	cluster.update();

	double interpolated_act = cluster.getMesh()->getActivityGrid()->getInterpolatedActivity(random_node->getPosition(),
			1);

	ASSERT(interpolated_act>0);
}

void MeshTest::testWarpNode() {
	ASSERTM("TODO", false);
}

void MeshTest::testWarpImpulse() {
	ASSERTM("TODO", false);
}
}//NAMESPACE

}//NAMESPACE
