#include <terminals/terminalsBase.h>

namespace iriov2{

TerminalsBase::TerminalsBase(std::shared_ptr<TerminalsBaseImpl> impl) {
	m_impl = impl;
}

}
