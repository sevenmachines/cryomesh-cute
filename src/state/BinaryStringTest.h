/*
 * BinaryStringTest.h
 *
 *  Created on: 26 Nov 2010
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef BINARYSTRINGTEST_H_
#define BINARYSTRINGTEST_H_
namespace cryomesh {
namespace state {
class BinaryStringTest {
public:
	static void runSuite();

	static void BinaryStringString() ;
	static void BinaryStringPattern() ;
	//static void BinaryStringInt() ;
	 static void testisValidBinary();
	// static void testtoInt();
	static void testresize() ;
	//static void testformatBinaryStringToInt();
	static void testformatTextToInt();
	static void testformatTextToBinaryString();
	static void testgetBools();

};

}
}
#endif /* BINARYSTRINGTEST_H_ */
