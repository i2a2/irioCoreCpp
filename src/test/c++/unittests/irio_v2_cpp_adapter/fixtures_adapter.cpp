#include "fixtures_adapter.h"
#include "irioResourceFinder.h"

BaseTestsAdapter::BaseTestsAdapter
	(const std::string &bitfileDir, const std::string &projectName):
		BaseTests(bitfileDir+"/"+STRINGNAME_PREFIX+
				projectName+STRINGNAME_BITFILEEXT),
		projectName(projectName),
		bitfileDir(bitfileDir){
}
