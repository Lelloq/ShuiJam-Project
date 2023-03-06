#pragma once
//!<Directory strings "../ShuiJamGame/" only there for testing once in release mode and is an exe, this is no longer needed
const std::string SJFOLDER = "../ShuiJamGame/";
const std::string SOUNDS = "./Sounds/";
const std::string IMAGES = "./Images/";
const std::string SONGINPUT = "./Input/";
const std::string SONGS = "./Songs/";
const std::string SHADER = "./Shader/";

const float SCR_WIDTH = 1366.f;//Default screen width
const float SCR_HEIGHT = 768.f;//Default screen height
const float VPORT_WIDTH = 1280.f;//Default viewport width (used for UI click position)
const float VPORT_HEIGHT = 720.f;//Default viewport height (used for UI click position)

//Globals (identified with g_)
inline std::string g_CurrentScene = "title";
