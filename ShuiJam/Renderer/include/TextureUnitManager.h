/*\file TextureUnitManager.h*/
#pragma once
#include <vector>
#include <unordered_map>

namespace Engine
{
	/*\class TextureUnitManager
	\brief class that handles texture unit bindings*/
	class TextureUnitManager
	{
	private:
		std::vector<uint32_t> m_textureIDs;//!< ID of texture bound to a unit
		std::unordered_map<uint32_t, uint32_t> m_units;//!< Unordered map of texture units
		size_t m_nextunit = 0;//!< Next position to bind to
	public:
		TextureUnitManager(size_t capacity); //!< Constructor that takes in a capacity
		_NODISCARD inline bool full() { return m_nextunit == m_textureIDs.capacity(); } //!< Is the buffer full
		void clear(); //!< Clear and reset the buffer
		_NODISCARD bool getUnit(uint32_t textureID, uint32_t& textureUnit); //!< Returns whether or not the texture ID is bound to that unit
	};
}
