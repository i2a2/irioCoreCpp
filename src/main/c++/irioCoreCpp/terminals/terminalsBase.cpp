#include <terminals/terminalsBase.h>
#include <terminals/impl/terminalsBaseImpl.h>

namespace irio {

TerminalsBase::TerminalsBase(std::shared_ptr<TerminalsBaseImpl> impl) :
		m_impl(impl) {
}

}  // namespace iriov2
