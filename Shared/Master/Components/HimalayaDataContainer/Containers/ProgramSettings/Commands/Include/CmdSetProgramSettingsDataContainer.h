#ifndef DATAMANAGER_CMDSETPROGRAMSETTINGSDATACONTAINER_H
#define DATAMANAGER_CMDSETPROGRAMSETTINGSDATACONTAINER_H

#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"

namespace DataManager
{

class CmdSetProgramSettingsDataContainer : public Global::Command
{
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    CmdSetProgramSettingsDataContainer(const CmdSetProgramSettingsDataContainer &);
    const CmdSetProgramSettingsDataContainer & operator = (const CmdSetProgramSettingsDataContainer &);

protected:

public:
    static QString  NAME;           ///< Command name.
    DataManager::CProgramSettings m_ProgramSettings;

    /****************************************************************************/
    /**
     * \brief Constructor.
     *
     */
    /****************************************************************************/
    CmdSetProgramSettingsDataContainer(CProgramSettings ProgramSettings);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdSetProgramSettingsDataContainer();

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
};

}

#endif // DATAMANAGER_CMDSETPROGRAMSETTINGSDATACONTAINER_H
