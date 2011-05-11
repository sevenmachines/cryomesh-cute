/*
 * ClusterMapTest.cpp
 *
 *  Created on: 11 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ClusterMapTest.h"
#include "structures/ClusterMap.h"

namespace cryomesh {

namespace structures {

void ClusterMapTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ClusterMapTest::testUpdateEnergyFractions));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ClusterMapTest");
}

void ClusterMapTest::testUpdateEnergyFractions() {
	const int NODE_COUNT = 2;
	const int CLUSTER_COUNT = 4;
	const int TOTAL_ENERGY = 10;
	const double DELTA = 0.00001;
	boost::shared_ptr<Cluster> cluster1(new Cluster(1 * NODE_COUNT, 1));
	boost::shared_ptr<Cluster> cluster2(new Cluster(2 * NODE_COUNT, 1));
	boost::shared_ptr<Cluster> cluster3(new Cluster(3 * NODE_COUNT, 1));
	boost::shared_ptr<Cluster> cluster4(new Cluster(4 * NODE_COUNT, 1));
	ClusterMap cluster_map;

	cluster_map.add( { cluster1, cluster2, cluster3, cluster4 });
	cluster_map.updateMapData();
	// check size
	{
		ASSERT_EQUAL(CLUSTER_COUNT, cluster_map.getSize());
	}

	// test by cluster
	{
		cluster_map.setEnergyFractionMethod(ClusterMap::ENERGY_FRACTION_BY_CLUSTER_COUNT);
		cluster_map.updateClusterEnergies(TOTAL_ENERGY);
		double ENERGY = (double)TOTAL_ENERGY/4.0;
		ASSERT_EQUAL_DELTA(ENERGY, cluster1->getEnergy(), DELTA);
		ASSERT_EQUAL_DELTA(ENERGY, cluster2->getEnergy(), DELTA);
		ASSERT_EQUAL_DELTA(ENERGY, cluster3->getEnergy(), DELTA);
		ASSERT_EQUAL_DELTA(ENERGY, cluster4->getEnergy(), DELTA);
	}
	// test by node count
	{
		cluster_map.setEnergyFractionMethod(ClusterMap::ENERGY_FRACTION_BY_NODE_COUNT);
		cluster_map.updateClusterEnergies(TOTAL_ENERGY);
		ASSERT_EQUAL_DELTA(1.0, cluster1->getEnergy(), DELTA);
		ASSERT_EQUAL_DELTA(2.0, cluster2->getEnergy(), DELTA);
		ASSERT_EQUAL_DELTA(3.0, cluster3->getEnergy(), DELTA);
		ASSERT_EQUAL_DELTA(4.0, cluster4->getEnergy(), DELTA);
	}
}

}//NAMESPACE

}//NAMESPACE
