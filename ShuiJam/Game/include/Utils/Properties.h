#pragma once
//!<Directory strings "../ShuiJamGame/" only there for testing once in release mode and is an exe, this is no longer needed
const std::string SJFOLDER = "../ShuiJamGame/";
const std::string SOUNDS = "./Sounds/";
const std::string IMAGES = "./Images/";
const std::string SONGINPUT = "./Input/";
const std::string SONGS = "./Songs/";
const std::string SHADER = "./Shader/";
const std::string FONTS = "./Fonts/";

inline float SCR_WIDTH = 1280.f;//Default screen width
inline float SCR_HEIGHT = 720.f;//Default screen height
const float VPORT_WIDTH = 1280.f;//Default viewport width (used for UI click position)
const float VPORT_HEIGHT = 720.f;//Default viewport height (used for UI click position)

//Globals (identified with g_)
inline std::string g_CurrentScene = "title";
inline std::wstring g_CurrentOsuDir = L"";
inline std::wstring g_CurrentDifficulty = L"";
inline std::wstring g_CurrentSong = L"";
inline std::wstring g_CurrentBG = L"";
inline std::wstring g_CurrentTitle = L"";
inline std::wstring g_CurrentDiffName = L"";

//file status
inline bool g_filesChanged = false;

//Gameplay
inline int g_perfCount = 0;
inline int g_greatCount = 0;
inline int g_goodCount = 0;
inline int g_badCount = 0;
inline int g_missCount = 0;
inline int g_highestCombo = 0;
inline float g_accuracy = 0;
inline bool g_failed = false;
