#ifndef CMDSTATIONDATACONTAINER_H
#define CMDSTATIONDATACONTAINER_H
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
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
    CDashboardDataStationList m_StationList;
    CDataReagentList m_ReagentList;

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     */
    /****************************************************************************/
    CmdStationDataContainer(CDashboardDataStationList StationList, CDataReagentList ReagentList);
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
