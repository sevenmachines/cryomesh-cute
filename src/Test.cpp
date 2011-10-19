#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

/*
#define DATAOBJECTS_TEST_SUITE
#define STATE_TEST_SUITE
#define COMMON_TEST_SUITE
#define UTILITIES_TEST_SUITE
#define USECASES_TEST_SUITE
#define MANAGER_TEST_SUITE
#define COMPONENTS_TEST_SUITE
#define STRUCTURES_TEST_SUITE
*/
#define MANIPULATORS_TEST_SUITE

// manipulators
#include "manipulators/ClusterArchitectTest.h"
#include "manipulators/ClusterAnalyserDataTest.h"
#include "manipulators/ClusterAnalyserBasicTest.h"

//manager
#include "manager/DatabaseManagerTest.h"
#include "manager/DatabaseObjectsTest.h"
#include "manager/CreatorTest.h"

//state
#include "state/BinaryStringTest.h"
#include "state/PatternChannelTest.h"
#include "state/PatternTest.h"
#include "state/SequenceTest.h"
#include "state/PatternChannelMapTest.h"

// common
#include "common/ConnectorTest.h"
#include "common/TimeKeeperTest.h"
#include "common/CycleTest.h"

//components
#include "components/NodeTest.h"
#include "components/ConnectionTest.h"
#include "components/ImpulseCollectionTest.h"
#include "components/ImpulseTest.h"
#include "components/ActivityTimerTest.h"

// dataobjects
#include "dataobjects/DataObjectTest.h"

//structures
#include "structures/ClusterTest.h"
#include "structures/ClusterMapTest.h"
#include "structures/BundleTest.h"
#include "structures/FibreTest.h"
#include "structures/MeshTest.h"
#include "structures/NodeMeshTest.h"

//utilities
#include "utilities/SequencerChannelsTest.h"
#include "utilities/SequencerGenericTest.h"

// use cases
#include "usecases/CycleTests.h"
#include "usecases/PropagationTests.h"
#include "usecases/FullSystemTests.h"
#include "usecases/MinimumNodeSystemTest.h"
using namespace cryomesh;

void runManipulatorsSuite() {
	//manipulators::ClusterAnalyserDataTest::runSuite();
	manipulators::ClusterAnalyserBasicTest::runSuite();
	//manipulators::ClusterArchitectTest::runSuite();
}

void runUtilitiesSuite() {
	utilities::SequencerGenericTest::runSuite();
	utilities::SequencerChannelsTest::runSuite();
}
void runManagerSuite() {
	manager::DatabaseObjectsTest::runSuite();
	manager::DatabaseManagerTest::runSuite();
	manager::CreatorTest::runSuite();
}

void runUseCasesSuite() {
	usecases::CycleTests::runSuite();
	usecases::PropagationTests::runSuite();
	usecases::FullSystemTests::runSuite();
	usecases::MinimumNodeSystemTest::runSuite();
}

void runStateSuite() {
	state::BinaryStringTest::runSuite();
	state::PatternTest::runSuite();
	state::SequenceTest::runSuite();
	state::PatternChannelTest::runSuite();
	state::PatternChannelMapTest::runSuite();
}
void runCommonSuite() {
	common::ConnectorTest::runSuite();
	common::TimeKeeperTest::runSuite();
	//	common::SimpleCollectionTest::runSuite();
	//	common::MiscTest::runSuite();
	//	common::MathsTest::runSuite();
	//	common::ContainersTest::runSuite();
	//	common::KeyStoreTest::runSuite();
	common::CycleTest::runSuite();
}

void runComponentsSuite() {
	components::ImpulseCollectionTest::runSuite();
	components::ImpulseTest::runSuite();
	components::ActivityTimerTest::runSuite();
	components::ConnectionTest::runSuite();
	components::NodeTest::runSuite();
}

void runDataObjectsSuite() {
	dataobjects::DataObjectTest::runSuite();
}

void runStructuresSuite() {
	structures::FibreTest::runSuite();
	structures::ClusterMapTest::runSuite();
	structures::BundleTest::runSuite();
	structures::ClusterTest::runSuite();
	//structures::MeshTest::runSuite();
	structures::NodeMeshTest::runSuite();
}

int main() {
#ifdef DATAOBJECTS_TEST_SUITE
	runDataObjectsSuite();
#endif
#ifdef STATE_TEST_SUITE
	runStateSuite();
#endif
#ifdef COMMON_TEST_SUITE
	runCommonSuite();
#endif
#ifdef UTILITIES_TEST_SUITE
	runUtilitiesSuite();
#endif
#ifdef USECASES_TEST_SUITE
	runUseCasesSuite();
#endif
#ifdef MANAGER_TEST_SUITE
	runManagerSuite();
#endif
#ifdef COMPONENTS_TEST_SUITE
	runComponentsSuite();
#endif
#ifdef STRUCTURES_TEST_SUITE
	runStructuresSuite();
#endif
#ifdef MANIPULATORS_TEST_SUITE
	runManipulatorsSuite();
#endif
	return 0;
}

