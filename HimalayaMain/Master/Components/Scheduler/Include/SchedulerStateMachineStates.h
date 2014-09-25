#ifndef SCHEDULERSTATEMACHINESTATES_H
#define SCHEDULERSTATEMACHINESTATES_H
namespace Scheduler{
#define STATE_UNDEFINED                     (0x0)
#define L1_INIT                             (0x1)
#define L1_IDLE                             (0x2)
#define L1_BUSY                             (0x3)
#define L1_ERROR                            (0x4)

#define L2_INIT_SELFTEST                    (0x1)

#define L2_PRO_INIT                           (0x1)
#define L2_PRO_PRETEST                        (0x2)
#define L2_PRO_FILLING_RVROD_HEATING          (0x3)
#define L2_PRO_FILLING_LEVELSENSOR_HEATING    (0x4)
#define L2_PRO_FILLING                        (0x5)
#define L2_PRO_RV_MOVE_TO_SEAL                (0x6)
#define L2_PRO_PROCESSING                     (0x7)
#define L2_PRO_PROCESSING_SR                  (0x8)
#define L2_PRO_RV_MOVE_TO_TUBE                (0x9)
#define L2_PRO_DRAINING                       (0xA)
#define L2_PRO_RV_POS_CHANGE                  (0xB)
#define L2_PRO_STEP_PROGRAM_FINISH            (0xC)
#define L2_PRO_PROGRAM_FINISH                 (0xD)
#define L2_PRO_PAUSE                          (0xE)
#define L2_PRO_PAUSE_DRAIN                    (0xF)
#define L2_PRO_ABORTING                       (0x10)
#define L2_PRO_ABORTED                        (0x11)
#define L2_PRO_CLEANING_DRY_STEP              (0x12)

#define L2_ERR_WAIT                                 (0x1)
#define L2_ERR_RS_RV_GETORIGINALPOSITIONAGAIN       (0x2)
#define L2_ERR_RS_STANDBY                           (0x3)
#define L2_ERR_RC_LEVELSENSOR_HEATING_OVERTIME      (0x4)
#define L2_ERR_RS_STANDBY_WITHTISSUE                (0x5)
#define L2_ERR_RC_RESTART                           (0x6)
#define L2_ERR_RS_HEATINGERR30SRETRY                (0x7)
#define L2_ERR_RS_PRESSUREOVERRANGE3SRETRY          (0x8)
#define L2_ERR_RS_TSENSORERR3MINRETRY               (0x9)
#define L2_ERR_RC_PRESSURE                          (0xA)
#define L2_ERR_RC_VACUUM                            (0xB)
#define L2_ERR_RC_FILLING                           (0xC)
#define L2_ERR_RC_DRAINING                          (0xD)
#define L2_ERR_RC_DRAINATONCE                       (0xE)
#define L2_ERR_RC_BOTTLECHECK_I                     (0xF)
#define L2_ERR_RS_FILLINGAFTERFLUSH                 (0x10)
#define L2_ERR_RS_CHECK_BLOCKAGE                    (0x11)
#define L2_ERR_RS_PAUSE                             (0x12)
#define L2_ERR_RS_RVWAITINGTEMPUP                   (0x13)
#define L2_ERR_RS_TISSUE_PROTECT                    (0x14)
#define L2_ERR_RC_CHECK_RTLOCK                      (0x15)

typedef enum
{
    // Layer one state
    SM_UNDEF = (STATE_UNDEFINED),
    SM_INIT = (L1_INIT),
    SM_IDLE = (L1_IDLE),
    SM_BUSY = (L1_BUSY),
    SM_ERROR = (L1_ERROR),

    //Layer two states (for init)
    SM_INIT_SELFTEST = ((L2_INIT_SELFTEST << 8) | L1_INIT),

    // Layer two states (for SM_BUSY)
    PSSM_INIT = ((L2_PRO_INIT << 8) | L1_BUSY),
    PSSM_PRETEST = ((L2_PRO_PRETEST << 8) | L1_BUSY),
    PSSM_FILLING_RVROD_HEATING = ((L2_PRO_FILLING_RVROD_HEATING << 8) | L1_BUSY),
    PSSM_FILLING_LEVELSENSOR_HEATING = ((L2_PRO_FILLING_LEVELSENSOR_HEATING << 8) | L1_BUSY),
    PSSM_FILLING = ((L2_PRO_FILLING << 8) | L1_BUSY),
    PSSM_RV_MOVE_TO_SEAL = ((L2_PRO_RV_MOVE_TO_SEAL << 8) | L1_BUSY),
    PSSM_PROCESSING_SR = ((L2_PRO_PROCESSING_SR << 8) | L1_BUSY),
    PSSM_PROCESSING = ((L2_PRO_PROCESSING << 8) | L1_BUSY),
    PSSM_RV_MOVE_TO_TUBE = ((L2_PRO_RV_MOVE_TO_TUBE << 8) | L1_BUSY),
    PSSM_DRAINING = ((L2_PRO_DRAINING << 8) | L1_BUSY),
    PSSM_RV_POS_CHANGE = ((L2_PRO_RV_POS_CHANGE << 8) | L1_BUSY),
    PSSM_STEP_PROGRAM_FINISH = ((L2_PRO_STEP_PROGRAM_FINISH << 8) | L1_BUSY),
    PSSM_PROGRAM_FINISH = ((L2_PRO_PROGRAM_FINISH << 8) | L1_BUSY),
    PSSM_PAUSE = ((L2_PRO_PAUSE << 8) | L1_BUSY),
    PSSM_PAUSE_DRAIN = ((L2_PRO_PAUSE_DRAIN << 8) | L1_BUSY),
    PSSM_ABORTING = ((L2_PRO_ABORTING << 8) | L1_BUSY),
    PSSM_ABORTED = ((L2_PRO_ABORTED << 8) | L1_BUSY),
    PSSM_CLEANING_DRY_STEP = ((L2_PRO_CLEANING_DRY_STEP << 8) | L1_BUSY),
     // Layer two states (for SM_ERROR)
    SM_ERR_WAIT = ((L2_ERR_WAIT << 8) | L1_ERROR),
    SM_ERR_RS_RV_GETORIGINALPOSITIONAGAIN = ((L2_ERR_RS_RV_GETORIGINALPOSITIONAGAIN << 8) | L1_ERROR),
    SM_ERR_RS_STANDBY = ((L2_ERR_RS_STANDBY << 8) | L1_ERROR),
    SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME = ((L2_ERR_RC_LEVELSENSOR_HEATING_OVERTIME << 8) | L1_ERROR),
    SM_ERR_RS_STANDBY_WITH_TISSUE = ((L2_ERR_RS_STANDBY_WITHTISSUE << 8) | L1_ERROR),
    SM_ERR_RC_RESTART = ((L2_ERR_RC_RESTART << 8) | L1_ERROR),
    SM_ERR_RS_HEATINGERR30SRETRY = ((L2_ERR_RS_HEATINGERR30SRETRY << 8 ) | L1_ERROR),
    SM_ERR_RS_PRESSUREOVERRANGE3SRETRY = ((L2_ERR_RS_PRESSUREOVERRANGE3SRETRY << 8) | L1_ERROR),
    SM_ERR_RS_TSENSORERR_3MINRETRY = ((L2_ERR_RS_TSENSORERR3MINRETRY << 8) | L1_ERROR),
    SM_ERR_RC_PRESSURE = ((L2_ERR_RC_PRESSURE << 8) | L1_ERROR) ,
    SM_ERR_RC_VACUUM = ((L2_ERR_RC_VACUUM << 8) | L1_ERROR),
    SM_ERR_RC_FILLING = ((L2_ERR_RC_FILLING << 8) | L1_ERROR),
    SM_ERR_RC_DRAINING = ((L2_ERR_RC_DRAINING << 8) | L1_ERROR),
    SM_ERR_RS_DRAINATONCE = ((L2_ERR_RC_DRAINATONCE << 8) | L1_ERROR),
    SM_ERR_RS_BOTTLECHECK_I = ((L2_ERR_RC_BOTTLECHECK_I << 8) | L1_ERROR),
    SM_ERR_RS_FILLINGAFTERFFLUSH = ((L2_ERR_RS_FILLINGAFTERFLUSH << 8) | L1_ERROR),
    SM_ERR_RS_CHECK_BLOCKAGE = ((L2_ERR_RS_CHECK_BLOCKAGE << 8) | L1_ERROR),
    SM_ERR_RS_PS_PAUSE = ((L2_ERR_RS_PAUSE << 8) | L1_ERROR),
    SM_ERR_RS_RV_WAITINGTEMPUP = ((L2_ERR_RS_RVWAITINGTEMPUP << 8) | L1_ERROR),
    SM_ERR_RS_TISSUE_PROTECT = ((L2_ERR_RS_TISSUE_PROTECT << 8) | L1_ERROR),
    SM_ERR_RC_CHECK_RTLOCK = ((L2_ERR_RC_CHECK_RTLOCK << 8) | L1_ERROR)
} SchedulerStateMachine_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of enum ControlCommandType_t
 */
/****************************************************************************/
typedef enum
{
    CTRL_CMD_START,
    CTRL_CMD_PAUSE,
    CTRL_CMD_ABORT,
    CTRL_CMD_SET_REMOTE_ALARM,
    CTRL_CMD_CLEAR_REMOTE_ALARM,
    CTRL_CMD_DRAIN,
    CTRL_CMD_DRAIN_SR,
    CTRL_CMD_OVEN_COVER_OPEN,
    CTRL_CMD_RETORT_COVER_OPEN,
    CTRL_CMD_QUITAPP,
    CTRL_CMD_SHUTDOWN,
    CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN,
    CTRL_CMD_RS_STANDBY,
    CTRL_CMD_RS_STANDBY_WITHTISSUE,
    CTRL_CMD_RS_HEATINGERR30SRETRY,
    CTRL_CMD_RS_PRESSUREOVERRANGE_3SRETRY,
    CTRL_CMD_RS_TSENSORERR3MINRETRY,
    CTRL_CMD_RC_RESTART,
    CTRL_CMD_RC_REPORT,
    CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME,
    CTRL_CMD_RC_PRESSURE,
    CTRL_CMD_RC_VACUUM,
    CTRL_CMD_RC_FILLING,
    CTRL_CMD_RC_DRAINING,
    CTRL_CMD_RS_DRAINATONCE,
    CTRL_CMD_RC_BOTTLECHECK_I,
    CTRL_CMD_RS_FILLINGAFTERFLUSH,
    CTRL_CMD_RS_CHECK_BLOCKAGE,
    CTRL_CMD_RS_PAUSE,
    CTRL_CMD_RS_RV_WAITINGTEMPUP,
    CTRL_CMD_RS_TISSUE_PROTECT,
    CTRL_CMD_USER_RESPONSE_PAUSE_ALARM,
    CTRL_CMD_RC_CHECK_RTLOCK,
    CTRL_CMD_ALARM_RMT_ON,
    CTRL_CMD_ALARM_RMT_OFF,
    CTRL_CMD_ALARM_LOC_ON,
    CTRL_CMD_ALARM_LOC_OFF,
    CTRL_CMD_ALARM_ALL_OFF,
    CTRL_CMD_NONE,
    CTRL_CMD_UNKNOWN
}ControlCommandType_t;

}
#endif // SCHEDULERSTATEMACHINESTATES_H
