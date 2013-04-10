#include "HimalayaDataContainer/Containers/ProgramSettings/Commands/Include/CmdGetProgramSettingsDataContainer.h"

namespace DataManager
{

QString CmdGetProgramSettingsDataContainer::NAME             = "DataManager::CmdGetProgramSettingsDataContainer";
int     CmdGetProgramSettingsDataContainer::m_CmdTimeout     = 3000;

CmdGetProgramSettingsDataContainer::CmdGetProgramSettingsDataContainer(CProgramSettings ProgramSettings) :
    Global::Command(m_CmdTimeout),
    m_ProgramSettings(ProgramSettings)
{

}

CmdGetProgramSettingsDataContainer::~CmdGetProgramSettingsDataContainer()
{

}

}
