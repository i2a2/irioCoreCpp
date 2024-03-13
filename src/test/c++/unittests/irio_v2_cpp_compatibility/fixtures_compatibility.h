#pragma once

#include "fixtures.h"

class BaseTestsCompatibility: public BaseTests {
 public:
	BaseTestsCompatibility(const std::string &projectName,
			const std::string &bitfileDir);

	const std::string projectName;
	const std::string bitfileDir;
};

