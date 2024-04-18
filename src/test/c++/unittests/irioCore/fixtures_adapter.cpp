#include "fixtures_adapter.h"
#include "irioResourceFinder.h"

BaseTestsAdapter::BaseTestsAdapter(const std::string &bitfileDir,
								   const std::string &projectName,
								   const bool includeSG)
	: BaseTests(bitfileDir + "/" + STRINGNAME_PREFIX + projectName +
				STRINGNAME_BITFILEEXT, includeSG),
	  projectName(projectName),
	  bitfileDir(bitfileDir) {}
