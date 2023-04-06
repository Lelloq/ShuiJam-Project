#include "Utils/OsuParser.h"
#include "Utils/Properties.h"

namespace SJ
{
	std::array<std::vector<Note>, 7> OsuParser::parse(std::wstring& dirName, std::wstring& osuName)
	{
		std::array<std::vector<Note>, 7> notes;
		std::fstream file;
		file.open(m_songsFolder + L'/' + dirName + L'/' + osuName);

		bool object = false;
		for(std::string line; std::getline(file,line);)
		{
			if(line.find("[HitObjects]") != std::string::npos)
			{
				object = true;
			}
			else if(object)
			{
				//Create a note object to be pushed into the vector later
				Note hitObject;
				//Find which column it belongs 0 - 6
				std::string noteColumn;
				noteColumn = line.substr(0, line.find_first_of(','));
				hitObject.column = floor((std::stoi(noteColumn) * 7) / 512);
				//Find timing point of the note
				std::string timingPoint = line;
				for(int i = 0; i < 2; i++)
				{
					timingPoint = timingPoint.substr(timingPoint.find_first_of(',')+1);
				}
				timingPoint = timingPoint.substr(0, timingPoint.find_first_of(','));
				hitObject.timingPoint = std::stoi(timingPoint);
				//Find release point of the note
				std::string releasePoint;
				releasePoint = line.substr(line.find_last_of(',') + 1, line.find_first_of(':') - (line.find_last_of(',') + 1));
				hitObject.releasePoint = std::stoi(releasePoint);
				//Push the note object into the right column
				notes.at(hitObject.column).push_back(hitObject);
			}
		}
		file.close();
		return notes;
	}
}
