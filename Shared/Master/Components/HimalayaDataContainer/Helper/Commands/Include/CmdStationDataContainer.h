#ifndef CMDSTATIONDATACONTAINER_H
#define CMDSTATIONDATACONTAINER_H
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataManager/Containers/StationsGrid/Include/DataStationMatrix.h"
#include "HimalayaDataManager/Containers/Stations/Include/DataStationList.h"
#include "HimalayaDataManager/Containers/StainingReagents/Include/DataReagentList.h"
namespace DataManager {

/****************************************************************************/
/**
 * \brief This command is sent to scheduler at startup , contains stations
 *        and station grid container.
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CmdStationDataContainer : public Global::Command {
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdStationDataContainer(const CmdStationDataContainer &);                       ///< Not implemented.
    const CmdStationDataContainer & operator = (const CmdStationDataContainer &);   ///< Not implemented.
protected:
public:
    static QString  NAME;           ///< Command name.
    DataManager::CDataStationList m_StationList;
    DataManager::CDataStationMatrix m_StationMatrix;
    DataManager::CDataReagentList m_ReagentList;

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     */
    /****************************************************************************/
    CmdStationDataContainer(CDataStationList StationList, CDataStationMatrix StationMatrix, CDataReagentList ReagentList);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdStationDataContainer();
    /****************************************************************************/
    /**
     * \brief Get command name.
     *
     * \return  Command name.
     */
    /****************************************************************************/
    virtual QString GetName() const {
        return NAME;
    }
}; // end class CmdStationDataContainer
} // end of namespace DataManager
#endif // CMDSTATIONDATACONTAINER_H
