/****************************************************************************/
/*! \file Include/HimalayaProcessExitCodes.h
 *
 *  \brief Definition of event groups for process exit codes.
 *
 *  Version:    0.1
 *  Date:       2012-10-22
 *  Author:     Ramya GJ
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef HIMALAYAPROCESSEXITCODES_H
#define HIMALAYAPROCESSEXITCODES_H



//exit codes for the BLG process
const qint32 EXIT_CODE_BLG_GENERATION_SUCCESS                      = 0x0001;    //!< Self explaining
const qint32 EXIT_CODE_BLG_READING_INPUTFILE_FAILED                = 0x0002;    //!< Self explaining
const qint32 EXIT_CODE_BLG_USED_PROGRAM_LIST_EMPTY                 = 0x0003;    //!< Self explaining
const qint32 EXIT_CODE_BLG_EMPTY_BLGRULES_LIST                     = 0x0004;    //!< Self explaining
const qint32 EXIT_CODE_BLG_WRITING_OUTPUT_FILES_FAILED             = 0x0005;    //!< Self explaining
const qint32 EXIT_CODE_BLG_BATHLAYOUT_SELECTION_FAILED_TREE_EMPTY  = 0x0006;    //!< Self explaining
const qint32 EXIT_CODE_BLG_GENERATION_FAILURE                      = 0x0007;    //!< Self explaining

#endif // HIMALAYAPROCESSEXITCODES_H
