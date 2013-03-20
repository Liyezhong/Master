
#include "HimalayaDataManager/Commands/Include/CmdStationDataContainer.h"
namespace DataManager {

QString CmdStationDataContainer::NAME             = "DataManager::CmdStationDataContainer";
int     CmdStationDataContainer::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdStationDataContainer::CmdStationDataContainer(CDashboardDataStationList StationList, CDataReagentList ReagentList)
    : Global::Command(m_CmdTimeout)
    , m_StationList(StationList)
    , m_ReagentList(ReagentList)
{
}

/****************************************************************************/
CmdStationDataContainer::~CmdStationDataContainer() {
}
} // end namespace DataManager

