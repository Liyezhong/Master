#include "HimalayaDataContainer/Containers/ProgramSettings/Commands/Include/CmdSetProgramSettingsDataContainer.h"

namespace DataManager
{

QString CmdSetProgramSettingsDataContainer::NAME             = "DataManager::CmdSetProgramSettingsDataContainer";
int     CmdSetProgramSettingsDataContainer::m_CmdTimeout     = 3000;

CmdSetProgramSettingsDataContainer::CmdSetProgramSettingsDataContainer(CProgramSettings ProgramSettings) :
    Global::Command(m_CmdTimeout),
    m_ProgramSettings(ProgramSettings)
{

}

CmdSetProgramSettingsDataContainer::~CmdSetProgramSettingsDataContainer()
{

}

}
