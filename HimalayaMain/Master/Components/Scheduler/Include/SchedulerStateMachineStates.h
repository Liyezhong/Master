#ifndef SCHEDULERSTATEMACHINESTATES_H
#define SCHEDULERSTATEMACHINESTATES_H
namespace Scheduler{
#define STATE_UNDEFINED                     (0x0)
#define L1_INIT                             (0x1)
#define L1_IDLE                             (0x2)
#define L1_BUSY                             (0x3)
#define L1_ERROR                            (0x4)

#define L2_PRO_INIT                           (0x1)
#define L2_PRO_PRETEST                        (0x2)
#define L2_PRO_FILLING_RVROD_HEATING          (0x3)
#define L2_PRO_FILLING_LEVELSENSOR_HEATING    (0x5)
#define L2_PRO_FILLING                        (0x6)
#define L2_PRO_RV_MOVE_TO_SEAL                (0x7)
#define L2_PRO_PROCESSING                     (0x8)
#define L2_PRO_RV_MOVE_TO_TUBE                (0x9)
#define L2_PRO_DRAINING                       (0xA)
#define L2_PRO_RV_POS_CHANGE                  (0xB)
#define L2_PRO_STEP_PROGRAM_FINISH            (0xC)
#define L2_PRO_PROGRAM_FINISH                 (0xD)
#define L2_PRO_PAUSE                          (0xE)
#define L2_PRO_PAUSE_DRAIN                    (0xF)
#define L2_PRO_ABORTING                       (0x10)
#define L2_PRO_ABORTED                        (0x11)

#define L2_ERR_WAIT                                 (0x1)
#define L2_ERR_RS_RV_GETORIGINALPOSITIONAGAIN       (0x2)
#define L2_ERR_RS_STANDBY                           (0x3)
#define L2_ERR_RC_LEVELSENSOR_HEATING_OVERTIME      (0x4)
#define L2_ERR_RS_STANDBY_WITHTISSUE                (0x5)
#define L2_ERR_RC_RESTART                           (0x6)
#define L2_ERR_RS_HEATINGERR30SRETRY                (0x7)
#define L2_ERR_RS_TSENSORERR3MINRETRY               (0x8)
#define L2_ERR_RC_PRESSURE                          (0x9)
#define L2_ERR_RC_VACUUM                            (0x10)
#define L2_ERR_RC_FILLING                           (0x11)
#define L2_ERR_RC_DRAINING                          (0x12)
#define L2_ERR_RC_DRAINATONCE                       (0x13)
#define L2_ERR_RC_BOTTLECHECK_I                     (0x14)

typedef enum
{
    // Layer one state
    SM_UNDEF = (STATE_UNDEFINED),
    SM_INIT = (L1_INIT),
    SM_IDLE = (L1_IDLE),
    SM_BUSY = (L1_BUSY),
    SM_ERROR = (L1_ERROR),

    // Layer two states (for SM_BUSY)
    PSSM_INIT = ((L2_PRO_INIT << 8) | L1_BUSY),
    PSSM_PRETEST = ((L2_PRO_PRETEST << 8) | L1_BUSY),
    PSSM_FILLING_RVROD_HEATING = ((L2_PRO_FILLING_RVROD_HEATING << 8) | L1_BUSY),
    PSSM_FILLING_LEVELSENSOR_HEATING = ((L2_PRO_FILLING_LEVELSENSOR_HEATING << 8) | L1_BUSY),
    PSSM_FILLING = ((L2_PRO_FILLING << 8) | L1_BUSY),
    PSSM_RV_MOVE_TO_SEAL = ((L2_PRO_RV_MOVE_TO_SEAL << 8) | L1_BUSY),
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

     // Layer two states (for SM_ERROR)
    SM_ERR_WAIT = ((L2_ERR_WAIT << 8) | L1_ERROR),
    SM_ERR_RS_RV_GETORIGINALPOSITIONAGAIN = ((L2_ERR_RS_RV_GETORIGINALPOSITIONAGAIN << 8) | L1_ERROR),
    SM_ERR_RS_STANDBY = ((L2_ERR_RS_STANDBY_WITHTISSUE << 8) | L1_ERROR),
    SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME = ((L2_ERR_RC_LEVELSENSOR_HEATING_OVERTIME << 8) | L1_ERROR),
    SM_ERR_RS_STANDBY_WITH_TISSUE = ((L2_ERR_RS_STANDBY_WITHTISSUE << 8) | L1_ERROR),
    SM_ERR_RC_RESTART = ((L2_ERR_RC_RESTART << 8) | L1_ERROR),
    SM_ERR_RS_HEATINGERR30SRETRY = ((L2_ERR_RS_HEATINGERR30SRETRY << 8 ) | L1_ERROR),
    SM_ERR_RS_TSENSORERR_3MINRETRY = ((L2_ERR_RS_TSENSORERR3MINRETRY << 8) | L1_ERROR),
    SM_ERR_RC_PRESSURE = ((L2_ERR_RC_PRESSURE << 8) | L1_ERROR) ,
    SM_ERR_RC_VACUUM = ((L2_ERR_RC_VACUUM << 8) | L1_ERROR),
    SM_ERR_RC_FILLING = ((L2_ERR_RC_FILLING << 8) | L1_ERROR),
    SM_ERR_RC_DRAINING = ((L2_ERR_RC_DRAINING << 8) | L1_ERROR),
    SM_ERR_RS_DRAINATONCE = ((L2_ERR_RC_DRAINATONCE << 8) | L1_ERROR),
    SM_ERR_RS_BOTTLECHECK_I = ((L2_ERR_RC_BOTTLECHECK_I << 8) | L1_ERROR)
} SchedulerStateMachine_t;
}
#endif // SCHEDULERSTATEMACHINESTATES_H
