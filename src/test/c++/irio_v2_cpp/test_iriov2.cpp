#include <gtest/gtest.h>
#include <iostream>
#include "irio_v2.h"


using namespace bfp;

std::map<ElemTypes, std::string> t = {
		{ElemTypes::Bool, "Bool"},
		{ElemTypes::U8, "U8"},
		{ElemTypes::I8, "I8"},
		{ElemTypes::U16, "U16"},
		{ElemTypes::I16, "I16"},
		{ElemTypes::U32, "U32"},
		{ElemTypes::I32, "I32"},
		{ElemTypes::U64, "U64"},
		{ElemTypes::I64, "I64"},
		{ElemTypes::Unsupported, "UNSUPPORTED"},

};

TEST(BFP, 1000){
   std::string bitfile = "../../resources/NiFpga_FlexRIOonlyResources_7966.lvbitx";
   BFP parsedBitfile(bitfile);

   auto m = parsedBitfile.getRegisters();

   for(auto it = m.begin(); it != m.end(); ++it){
	   std::cout << it->second.name << "-" << t[it->second.elemType] << std::endl;
   }

   auto d = parsedBitfile.getDMAs();
   for(auto it = d.begin(); it != d.end(); ++it){
	   std::cout << it->second.name << "-" << t[it->second.elemType] << std::endl;
   }
   std::cout << m.size() << std::endl;

}

