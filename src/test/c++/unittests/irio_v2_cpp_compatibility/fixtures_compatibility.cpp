#include "fixtures_compatibility.h"
#include "irioResourceFinder.h"

BaseTestsCompatibility::BaseTestsCompatibility
	(const std::string &bitfileDir, const std::string &projectName):
		BaseTests(bitfileDir+"/"+STRINGNAME_PREFIX+
				projectName+STRINGNAME_BITFILEEXT),
		projectName(projectName),
		bitfileDir(bitfileDir){
}
