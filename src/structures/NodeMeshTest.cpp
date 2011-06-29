/*
 * NodeMeshTest.cpp
 *
 *  Created on: 26 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "NodeMeshTest.h"
#include "structures/NodeMesh.h"
#include "ICuteSuite.h"
#include "structures/Cluster.h"

namespace cryomesh {

namespace structures {

void NodeMeshTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(NodeMeshTest::testRegenerateNeighbourhoods));
	s.push_back(CUTE(NodeMeshTest::testRegenerateActivities));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "NodeMeshTest");
}

void NodeMeshTest::testRegenerateNeighbourhoods() {
	const int NODECOUNT = 10;
	const double BOUNDINGBOXMAX = 10;
	const double BOUNDINGBOXMAXRADIUS = std::sqrt(3 * (BOUNDINGBOXMAX * BOUNDINGBOXMAX));
	// set up test neighbourhood
	boost::shared_ptr<Cluster> cluster(new Cluster(NODECOUNT, 1, spacial::Point(10, 10, 10)));
	// make radius large enough so we all have neighbours
	NodeMesh nodemesh(*cluster, 5);
	nodemesh.regenerateNeighbourhoods();

	NodeMesh::NeighbourhoodRanges minmaxes = nodemesh.getNeighbourRanges();
	ASSERT(minmaxes.minimumNeighbourCount>0);
	ASSERT(minmaxes.maximumNeighbourCount<NODECOUNT);
	ASSERT(minmaxes.minimumNeighbourDistance>0);
	ASSERT(minmaxes.maximumNeighbourDistance<BOUNDINGBOXMAXRADIUS);

}

void NodeMeshTest::testRegenerateActivities() {
	// set up cluster
	const int NODECOUNT = 10;
	const double BOUNDINGBOXMAX = 0.5;
	// set up test neighbourhood
	boost::shared_ptr<Cluster> cluster(
			new Cluster(NODECOUNT, 1, spacial::Point(BOUNDINGBOXMAX, BOUNDINGBOXMAX, BOUNDINGBOXMAX)));
	// make radius large enough so we all have neighbours
	NodeMesh nodemesh(*cluster, 5);
	nodemesh.regenerateNeighbourhoods();

	// give all nodes an activity in a range
	cluster->getMutableNodeMap().addRandomImpulses(1);
	common::TimeKeeper::getTimeKeeper().update();
	// regenerate activities from nieghbours
	nodemesh.regenerateActivities();
	// ensure neighbour interpolated activities are sane
	const std::map<boost::shared_ptr<components::Node>, double> & acts = nodemesh.getNeighbourhoodActivities();
	// forall in acts
	{
		std::map<boost::shared_ptr<components::Node>, double>::const_iterator it_acts = acts.begin();
		const  std::map<boost::shared_ptr<components::Node>, double>::const_iterator it_acts_end = acts.end();
		while (it_acts != it_acts_end) {
			ASSERT(it_acts->second>0);
			++it_acts;
		}
	}
}

}//NAMESPACE
}//NAMESPACE
