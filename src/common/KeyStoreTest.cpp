/*
 * KeyStoreTest.cpp
 *
 *  Created on: 13 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "KeyStoreTest.h"
#include "common/KeyStore.h"

namespace cryomesh {

namespace common {

void KeyStoreTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE( KeyStoreTest::testCreate));
	s.push_back(CUTE(KeyStoreTest::testAddRemove));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "KeyStoreTest");
}

void KeyStoreTest::testCreate() {
	// test null entry is automatically created

	KeyStore<std::string, std::string> stringKeyStore;
	ASSERT_EQUAL(0, stringKeyStore.size());
	ASSERT_EQUAL("", stringKeyStore.get("")->second);

	stringKeyStore.clear();
	ASSERT_EQUAL(0, stringKeyStore.size());
	ASSERT_EQUAL("", stringKeyStore.get("")->second);

	// test null
	ASSERT_EQUAL("", stringKeyStore.getNullEntry()->first);
	ASSERT_EQUAL("", stringKeyStore.getNullEntry()->second);

}
void KeyStoreTest::testAddRemove() {
	KeyStore<std::string, std::string> stringKeyStore;
	stringKeyStore.add({std::pair<std::string, std::string>("help", "me")});
	stringKeyStore.add("help2", "me2");
	ASSERT_EQUAL(2, stringKeyStore.size());

	stringKeyStore.remove({"help", "help2"});
	ASSERT_EQUAL(0, stringKeyStore.size());

}

}//NAMESPACE

}//NAMESPACE
