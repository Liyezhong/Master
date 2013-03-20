
#include "HimalayaDataManager/Templates/Commands/Include/CmdStationDataContainer.h"
namespace DataManager {

QString CmdStationDataContainer::NAME             = "DataManager::CmdStationDataContainer";
int     CmdStationDataContainer::m_CmdTimeout     = 3000;

/****************************************************************************/
CmdStationDataContainer::CmdStationDataContainer(CDataStationList StationList, CDataStationMatrix StationMatrix, CDataReagentList ReagentList)
    : Global::Command(m_CmdTimeout)
    , m_StationList(StationList)
    , m_StationMatrix(StationMatrix)
    , m_ReagentList(ReagentList)
{
}

/****************************************************************************/
CmdStationDataContainer::~CmdStationDataContainer() {
}
} // end namespace DataManager

