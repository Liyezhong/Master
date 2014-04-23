#ifndef DATAMANAGER_CMDSETPROGRAMSETTINGSDATACONTAINER_H
#define DATAMANAGER_CMDSETPROGRAMSETTINGSDATACONTAINER_H

#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"

namespace DataManager
{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdSetProgramSettingsDataContainer
 */
/****************************************************************************/
class CmdSetProgramSettingsDataContainer : public Global::Command
{
private:
    static int  m_CmdTimeout;   ///< The allowed timeout for this command.
    /****************************************************************************/
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdSetProgramSettingsDataContainer
     *
     *  \return from CmdSetProgramSettingsDataContainer
     */
    /****************************************************************************/
    CmdSetProgramSettingsDataContainer(const CmdSetProgramSettingsDataContainer &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function operator=
     *
     *  \return from operator=
     */
    /****************************************************************************/
    const CmdSetProgramSettingsDataContainer & operator = (const CmdSetProgramSettingsDataContainer &);

protected:

public:
    static QString  NAME;           ///< Command name.
    DataManager::CProgramSettings m_ProgramSettings;       ///<  Definition/Declaration of variable m_ProgramSettings

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
