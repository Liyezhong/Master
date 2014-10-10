#ifndef DATAMANAGER_CMDGETPROGRAMSETTINGSDATACONTAINER_H
#define DATAMANAGER_CMDGETPROGRAMSETTINGSDATACONTAINER_H

#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"

namespace DataManager
{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdGetProgramSettingsDataContainer
 */
/****************************************************************************/
class CmdGetProgramSettingsDataContainer : public Global::Command
{
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    /**
    * \brief copy constructor
    */
    CmdGetProgramSettingsDataContainer(const CmdGetProgramSettingsDataContainer &);
    /**
    * \brief operator
    */
    const CmdGetProgramSettingsDataContainer & operator = (const CmdGetProgramSettingsDataContainer &);   ///< Not implemented.

protected:

public:
    static QString  NAME;           ///< Command name.
    DataManager::CProgramSettings m_ProgramSettings;       ///<  Definition/Declaration of variable m_ProgramSettings

    /****************************************************************************/
    /**
     * \brief Constructor.
     * \param ProgramSettings program settings
     */
    /****************************************************************************/
    CmdGetProgramSettingsDataContainer(CProgramSettings ProgramSettings);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CmdGetProgramSettingsDataContainer();

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

#endif // DATAMANAGER_CMDGETPROGRAMSETTINGSDATACONTAINER_H
