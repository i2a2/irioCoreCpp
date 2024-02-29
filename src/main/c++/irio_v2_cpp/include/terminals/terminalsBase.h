#pragma once

#include <memory>

#include <terminals/impl/terminalsBaseImpl.h>

namespace iriov2{

class TerminalsBase {
public:
	TerminalsBase(std::shared_ptr<TerminalsBaseImpl> impl);

protected:
	std::shared_ptr<TerminalsBaseImpl> m_impl;
};
}
