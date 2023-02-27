/*\file TextureUnitManager.cpp*/
#include "engine_pch.h"
#include "rendering/TextureUnitManager.h"

namespace Engine
{
	TextureUnitManager::TextureUnitManager(size_t capacity)
	{
		m_textureIDs.resize(capacity);
		std::fill(m_textureIDs.begin(), m_textureIDs.end(), 0xFFFFFFFF);
		m_units.reserve(capacity * 2);
	}

	void TextureUnitManager::clear()
	{
		std::fill(m_textureIDs.begin(), m_textureIDs.end(), 0xFFFFFFFF);
		m_units.clear();
		m_nextunit = 0;
	}
	bool TextureUnitManager::getUnit(uint32_t textureID, uint32_t& textureUnit)
	{
		auto it = m_units.find(textureID);
		if(it != m_units.end())
		{
			textureUnit = it->second;
			return false;
		}

		if(full())
		{
			textureUnit = -1;
			return true;
		}
		else
		{
			textureUnit = m_nextunit;

			m_textureIDs[m_nextunit] = textureID;
			m_units[textureID] = m_nextunit;

			m_nextunit++;

			return true;
		}
	}
}
