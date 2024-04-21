#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

#include "terminals/terminals.h"
#include "profilesTypes.h"


namespace irio {

/**
 * Base profile class.
 *
 * Allows the user to access terminals for read/write operations.
 * If a terminal group is not in the profile,
 * an \ref irio::errors::TerminalNotImplementedError exception will be thrown.
 *
 * @ingroup Profiles
 */
class ProfileBase {
 public:
	/**
	 * Allows the user to access terminals for read/write operations.
	 *
	 * @param id	Identification of the profile
	 */
	explicit ProfileBase(ParserManager *parserManager,
		const NiFpga_Session &session, const PROFILE_ID &id);

	virtual ~ProfileBase() = default;

	/**
	 * Returns the specified terminal if it is present in the current profile
	 *
	 * @throw irio::errors::TerminalNotImplementedError	Terminals group not present for the current profile
	 *
	 * @tparam T	Terminals to get
	 * @return		Requested terminals
	 */
	template<typename T>
	T getTerminal() const;

	/**
	 * Profile type
	 */
	const PROFILE_ID profileID;

 protected:
	/**
	 * @brief Adds a terminal to the profile.
	 *
	 * @param terminal The terminal to be added.
	 * @tparam T The type of the terminal.
	 */
	template<typename T>
	void addTerminal(T terminal);

 private:
	/// Associates a Terminal type to the actual instance of the terminal
	std::unordered_map<std::type_index,
			std::unique_ptr<TerminalsBase>> m_mapTerminals;
};

}  // namespace irio
