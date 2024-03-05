#include <terminals/terminalsBase.h>
#include <terminals/impl/terminalsBaseImpl.h>

namespace iriov2 {

TerminalsBase::TerminalsBase(std::shared_ptr<TerminalsBaseImpl> impl) :
		m_impl(impl) {
}

}  // namespace iriov2
