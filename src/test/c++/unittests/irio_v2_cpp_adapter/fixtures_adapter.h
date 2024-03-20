#pragma once

#include "fixtures.h"

class BaseTestsAdapter: public BaseTests {
 public:
	BaseTestsAdapter(const std::string &projectName,
			const std::string &bitfileDir);

	const std::string projectName;
	const std::string bitfileDir;
};

