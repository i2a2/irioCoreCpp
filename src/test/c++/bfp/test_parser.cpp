#include <gtest/gtest.h>
#include <iostream>
#include "bfp.h"
#include "errorsIrio.h"

using namespace iriov2::bfp;

std::map<ElemTypes, std::string> t = { { ElemTypes::Bool, "Bool" }, { ElemTypes::U8, "U8" }, {
		ElemTypes::I8, "I8" }, { ElemTypes::U16, "U16" }, { ElemTypes::I16, "I16" }, {
		ElemTypes::U32, "U32" }, { ElemTypes::I32, "I32" }, { ElemTypes::U64, "U64" }, {
		ElemTypes::I64, "I64" }, { ElemTypes::Unsupported, "UNSUPPORTED" },

};

TEST(BFP, Parse) {
	std::string bitfile = "../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx";
	BFP parsedBitfile(bitfile);
}

TEST(BFP, Basic) {
	std::string bitfile = "../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx";
	BFP parsedBitfile(bitfile, false);

	EXPECT_EQ(parsedBitfile.getBitfilePath(), bitfile);
	EXPECT_EQ(parsedBitfile.getBitfileVersion(), "4.0");
	EXPECT_EQ(parsedBitfile.getSignature(), "F8D0486B2C90CB55C8A88E01FF18F295");
}

TEST(BFP, InvalidBitfile) {
	std::string bitfile = "DOESNOTEXIST.lvbitx";
	EXPECT_THROW(BFP parsedBitfile(bitfile, false); // @suppress("Goto statement used")
	, iriov2::errors::BFPParseBitfileError);
}

TEST(BFP, Registers) {
	std::string bitfile = "../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx";
	BFP parsedBitfile(bitfile, false);

	auto registers = parsedBitfile.getRegisters();
	EXPECT_EQ(registers.size(), 241) << "Unexpected number or registers found";

	{
		auto reg = parsedBitfile.getRegister("AOEnable0");
		EXPECT_EQ(reg.name, "AOEnable0") << "Incorrect register's name";
		EXPECT_EQ(reg.address, 2) << "Incorrect register's address";
		EXPECT_EQ(reg.elemType, ElemTypes::Bool) << "Incorrect register's element type";
		EXPECT_EQ(reg.fpgaType, FpgaTypes::FpgaType_Control) << "Incorrect register's fpag type";
		EXPECT_EQ(reg.numElem, 1) << "Incorrect register's number of elements";
		EXPECT_FALSE(reg.isArray()) << "Register is an array when it should not be";
		EXPECT_TRUE(reg.isControl()) << "Register is not a control register when it should";
		EXPECT_FALSE(reg.isIndicator()) << "Register is an indicator register when it should not be";
	}
	{
		auto reg = parsedBitfile.getRegister("DevTemp");
		EXPECT_EQ(reg.name, "DevTemp") << "Incorrect register's name";
		EXPECT_EQ(reg.address, 78) << "Incorrect register's address";
		EXPECT_EQ(reg.elemType, ElemTypes::I16) << "Incorrect register's element type";
		EXPECT_EQ(reg.fpgaType, FpgaTypes::FpgaType_Indicator) << "Incorrect register's fpag type";
		EXPECT_EQ(reg.numElem, 1) << "Incorrect register's number of elements";
		EXPECT_FALSE(reg.isArray()) << "Register is an array when it should not be";
		EXPECT_FALSE(reg.isControl()) << "Register is a control register when it should not be";
		EXPECT_TRUE(reg.isIndicator()) << "Register is not a indicator register";
	}
	{
		auto reg = parsedBitfile.getRegister("FPGAVIversion");
		EXPECT_EQ(reg.name, "FPGAVIversion") << "Incorrect register's name";
		EXPECT_EQ(reg.address, 934) << "Incorrect register's address";
		EXPECT_EQ(reg.elemType, ElemTypes::U8) << "Incorrect register's element type";
		EXPECT_EQ(reg.fpgaType, FpgaTypes::FpgaType_ArrayIndicator)
				<< "Incorrect register's fpag type";
		EXPECT_EQ(reg.numElem, 2) << "Incorrect register's number of elements";
		EXPECT_TRUE(reg.isArray()) << "Register is not an array when it should";
		EXPECT_FALSE(reg.isControl()) << "Register is a control register when it should not be";
		EXPECT_TRUE(reg.isIndicator()) << "Register is not a indicator register";
	}
}

TEST(BFP, InvalidRegister){
	std::string bitfile = "../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx";
	BFP parsedBitfile(bitfile, false);

	EXPECT_THROW(parsedBitfile.getRegister("NOTEXIST");, // @suppress("Goto statement used")
		iriov2::errors::ResourceNotFoundError);
}

TEST(BFP, DMAs){
	std::string bitfile = "../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx";
	BFP parsedBitfile(bitfile, false);

	auto dmas = parsedBitfile.getDMAs();
	EXPECT_EQ(dmas.size(), 1);

	auto dma = parsedBitfile.getDMA("DMATtoHOST0");
	EXPECT_EQ(dma.name, "DMATtoHOST0") << "Incorrect DMA's name";
	EXPECT_EQ(dma.getDMANumber(), 0) << "Incorrect DMA's address";
	EXPECT_EQ(dma.elemType, ElemTypes::U64) << "Incorrect DMA's element type";
	EXPECT_EQ(dma.fpgaType, FpgaTypes::FpgaType_DMATtH) << "Incorrect DMA's fpag type";
	EXPECT_EQ(dma.numElem, 1023) << "Incorrect DMA's number of elements";

	EXPECT_TRUE(dma.isTargetToHost());
	EXPECT_FALSE(dma.isHostToTarget());
}

TEST(BFP, InvalidDMA){
	std::string bitfile = "../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx";
	BFP parsedBitfile(bitfile, false);

	EXPECT_THROW(parsedBitfile.getDMA("NOTEXIST");, // @suppress("Goto statement used")
		iriov2::errors::ResourceNotFoundError);
}

